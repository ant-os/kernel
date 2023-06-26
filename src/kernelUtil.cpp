#include "kernelUtil.h"
#include "gdt/gdt.h"
#include "interrupts/IDT.h"
#include "bits.h"
#include "DriveList.h"

#include <ant/memory.h>

KernelInfo kernelInfo;
void PrepareMemory(BootInfo *bootInfo)
{
    uint64_t mMapEntries = bootInfo->mMapSize / bootInfo->mMapDescSize;

    GlobalAllocator = PageFrameAllocator();
    GlobalAllocator.ReadEFIMemoryMap(bootInfo->mMap, bootInfo->mMapSize, bootInfo->mMapDescSize);

    uint64_t kernelSize = (uint64_t)&_KernelEnd - (uint64_t)&_KernelStart;
    uint64_t kernelPages = (uint64_t)kernelSize / 4096 + 1;

    GlobalAllocator.LockPages(&_KernelStart, kernelPages);

    PageTable *PML4 = (PageTable *)GlobalAllocator.RequestPage();
    memset(PML4, 0, 0x1000);

    g_PageTableManager = PageTableManager(PML4);

    for (uint64_t t = 0; t < GetMemorySize(bootInfo->mMap, mMapEntries, bootInfo->mMapDescSize); t += 0x1000)
    {
        g_PageTableManager.MapMemory((void*)t, (void*)t);
    }

    uint64_t fbBase = (uint64_t)bootInfo->framebuffer->BaseAddress;
    uint64_t fbSize = (uint64_t)bootInfo->framebuffer->BufferSize + 0x1000;
    GlobalAllocator.LockPages((void *)fbBase, fbSize / 0x1000 + 1);
    for (uint64_t t = fbBase; t < fbBase + fbSize; t += 4096)
    {
        g_PageTableManager.MapMemory((void*)t, (void*)t);
    }

    if (bootInfo->MiniFS.Status == 0) {
        SECTION_OBJECT MiniFSDriverSection{ (void*)bootInfo->MiniFS.Base, bootInfo->MiniFS.Size };

        GlobalAllocator.LockSection(MiniFSDriverSection);
    }


    asm("mov %0, %%cr3"
        :
        : "r"(PML4));

    kernelInfo.pageTableManager = &g_PageTableManager;
}

#define ADD_INTERRUPT_HANDLER(n, t, o)                                                \
    IDTDescEntry *int_##n = (IDTDescEntry *)(idtr.Offset + o * sizeof(IDTDescEntry)); \
    int_##n->SetOffset((uint64_t)n##_handler);                                        \
    int_##n->type_attr = t;                                                           \
    int_##n->selector = 0x08

IDTR idtr;
static uint8_t master_mask = 0b11111001;
static uint8_t slave_mask = 0b11101111;
void PrepareInterrupts()
{
    idtr.Limit = 0x0FFF;
    idtr.Offset = (uint64_t)GlobalAllocator.RequestPage();

// TODO: ADD MORE INT HANDLER...
#pragma region Exceptions

    ADD_INTERRUPT_HANDLER(
        PageFault,
        IDT_TA_InterruptGate,
        0xE);

    ADD_INTERRUPT_HANDLER(
        DoubleFault,
        IDT_TA_InterruptGate,
        0x8);

    ADD_INTERRUPT_HANDLER(
        GPFault,
        IDT_TA_InterruptGate,
        0xD);

    ADD_INTERRUPT_HANDLER(
        DivisionError,
        IDT_TA_InterruptGate,
        0x0);

    ADD_INTERRUPT_HANDLER(
        Debug,
        IDT_TA_InterruptGate,
        0x1);

    ADD_INTERRUPT_HANDLER(
        Breakpoint,
        IDT_TA_InterruptGate,
        0x3);

    ADD_INTERRUPT_HANDLER(
        Overflow,
        IDT_TA_InterruptGate,
        0x4);

    ADD_INTERRUPT_HANDLER(
        BoundRangeExceeded,
        IDT_TA_InterruptGate,
        0x5);

    ADD_INTERRUPT_HANDLER(
        InvalidOpcode,
        IDT_TA_InterruptGate,
        0x6);

    ADD_INTERRUPT_HANDLER(
        DeviceNotAvailable,
        IDT_TA_InterruptGate,
        0x7);

    ADD_INTERRUPT_HANDLER(
        InvalidTSS,
        IDT_TA_InterruptGate,
        0xA);

    ADD_INTERRUPT_HANDLER(
        SegmentNotPresent,
        IDT_TA_InterruptGate,
        0xB);

    ADD_INTERRUPT_HANDLER(
        StackSegmentFault,
        IDT_TA_InterruptGate,
        0xC);

    ADD_INTERRUPT_HANDLER(
        AlignmentCheck,
        IDT_TA_InterruptGate,
        0x11);

    ADD_INTERRUPT_HANDLER(
        MachineCheck,
        IDT_TA_InterruptGate,
        0x12);

    ADD_INTERRUPT_HANDLER(
        VirtualizationException,
        IDT_TA_InterruptGate,
        0x14);

    ADD_INTERRUPT_HANDLER(
        ControlProtectionException,
        IDT_TA_InterruptGate,
        0x15);

    ADD_INTERRUPT_HANDLER(
        HypervisorInjectionException,
        IDT_TA_InterruptGate,
        0x1C);

    ADD_INTERRUPT_HANDLER(
        VMMCommunicationException,
        IDT_TA_InterruptGate,
        0x1D);

    ADD_INTERRUPT_HANDLER(
        SecurityException,
        IDT_TA_InterruptGate,
        0x1E);

    /* ADD_INTERRUPT_HANDLER(
        TripleFault,
        IDT_TA_InterruptGate,
        ...
    ); */

#pragma endregion

    ADD_INTERRUPT_HANDLER(
        Keyboard,
        IDT_TA_InterruptGate,
        0x21
    );

    ADD_INTERRUPT_HANDLER(
        Mouse,
        IDT_TA_InterruptGate,
        0x2C
    );

    ADD_INTERRUPT_HANDLER(
        PIT,
        IDT_TA_InterruptGate,
        0x20
    );

    ADD_INTERRUPT_HANDLER(
        UtilCall,
        IDT_TA_InterruptGate,
        0xF0
    );

    asm("lidt %0"
        :
        : "m"(idtr));
}

void PrepareACPI(BootInfo *bootInfo)
{
    ACPI::SDTHeader *xsdt = (ACPI::SDTHeader *)(bootInfo->rsdp->XSDTAddress);

    ACPI::MCFGHeader* mcfg = (ACPI::MCFGHeader*)ACPI::FindTable(xsdt, (char*)"MCFG");

    ShowInfo(COM1, "[Tables]");

    LogAddress("RSDP Address", bootInfo->rsdp);
    LogAddress("RSDT Address", bootInfo->rsdp->RSDTAddress);
    LogAddress("XSDT Address", xsdt);
    LogAddress("MCFG Address", mcfg);
    
    Serial::WriteString(COM1, NEW_LINE);
    ShowInfo(COM1, "[Devices]");

    PCI::EnumeratePCI(mcfg);
}

BasicRenderer r = BasicRenderer(NULL, NULL);
KernelInfo *InitializeKernel(BootInfo *bootInfo)
{
    r = BasicRenderer(bootInfo->framebuffer, bootInfo->BootFont);
    GlobalRenderer = &r;

    GDTDescriptor gdtDescriptor;
    gdtDescriptor.Size = sizeof(GDT) - 1;
    gdtDescriptor.Offset = (uint64_t)&DefaultGDT;
    LoadGDT(&gdtDescriptor);

    PrepareMemory(bootInfo);

    memset(bootInfo->framebuffer->BaseAddress, 0, bootInfo->framebuffer->BufferSize);

    InitializeHeap((void*)0x0000100000000000, 0x10);


    PrepareInterrupts();
    RemapPIC();
    Serial::InitPort(COM1);


    PrepareACPI(bootInfo);

    outb(PIC1_DATA, master_mask);
    outb(PIC2_DATA, slave_mask);

    sti();


    return &kernelInfo;
}

void EnableKeyboard()
{
    ShowInfo(COM1, "\r\nKeyboard is now Enabled...\n\r");

    master_mask = BIT_CLEAR(master_mask, 1);

    cli();

    RemapPIC();

    outb(PIC1_DATA, master_mask);
    outb(PIC2_DATA, slave_mask);

    sti();
}

void DisableKeyboard()
{
    ShowInfo(COM1, "\r\nKeyboard is now Disabled...\n\r");

    master_mask = BIT_CLEAR(master_mask, 1);

    cli();

    RemapPIC();

    outb(PIC1_DATA, master_mask);
    outb(PIC2_DATA, slave_mask);

    sti();
}

void EnableMouse()
{
    ShowInfo(COM1, "\r\nMouse is now Enabled...\n\r");

    master_mask = BIT_CLEAR(master_mask, 2);
    slave_mask = BIT_CLEAR(slave_mask, 5);

    cli();

    RemapPIC();

    outb(PIC1_DATA, master_mask);
    outb(PIC2_DATA, slave_mask);

    sti();
}

void DisableMouse()
{
    ShowInfo(COM1, "\r\nMouse is now Disabled...\n\r");

    master_mask = BIT_SET(master_mask, 2);
    slave_mask = BIT_SET(slave_mask, 5);

    cli();

    RemapPIC();

    outb(PIC1_DATA, master_mask);
    outb(PIC2_DATA, slave_mask);

    sti();
}

void EnablePIT()
{
    ShowInfo(COM1, "\r\nPIT is Enabled...\n\r");

    master_mask = BIT_CLEAR(master_mask, 0);

    cli();

    RemapPIC();

    outb(PIC1_DATA, master_mask);
    outb(PIC2_DATA, slave_mask);

    sti();
}

void DisablePIT()

{
    ShowInfo(COM1, "\r\nPIT is Disabled...\n\r");
    master_mask = BIT_SET(master_mask, 0);

    cli();

    RemapPIC();

    outb(PIC1_DATA, master_mask);
    outb(PIC2_DATA, slave_mask);

    sti();
}