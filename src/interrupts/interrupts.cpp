#include "interrupts.h"
#include "../BasicRenderer.h"
#include "../panic.h"
#include "../cstr.h"
#include "../userinput/keyboard.h"
#include "../userinput/mouse.h"
#include "../scheduling/pit/pit.h"

FAULT_HANDLER(Page)
{

    PanicData pd = {
        /*  REGISTERS  */
        frame->edi,
        frame->esi,
        frame->ebp,
        frame->esp_dummy,
        frame->ebx,
        frame->edx,
        frame->ecx,
        frame->eax,
        frame->gs,
        frame->fs,
        frame->es,
        frame->ds,

        /* ERROR & FRAME */

        frame->error_code,

        frame->frame_pointer,

        /*  BY CPU  */
        frame->eip,
        frame->cs,
        frame->eflags,
        frame->esp,
        frame->ss
    };

    Panic("Page Fault!\n", &pd);

    while (true);
}
FAULT_HANDLER(Double)
{

    PanicData pd = {
        /*  REGISTERS  */
        frame->edi,
        frame->esi,
        frame->ebp,
        frame->esp_dummy,
        frame->ebx,
        frame->edx,
        frame->ecx,
        frame->eax,
        frame->gs,
        frame->fs,
        frame->es,
        frame->ds,

        /* ERROR & FRAME */

        frame->error_code,

        frame->frame_pointer,

        /*  BY CPU  */
        frame->eip,
        frame->cs,
        frame->eflags,
        frame->esp,
        frame->ss
    };

    Panic("Double Fault!\n", &pd);

    while (true);

}
FAULT_HANDLER(GP)
{

    PanicData pd = {
        /*  REGISTERS  */
        frame->edi,
        frame->esi,
        frame->ebp,
        frame->esp_dummy,
        frame->ebx,
        frame->edx,
        frame->ecx,
        frame->eax,
        frame->gs,
        frame->fs,
        frame->es,
        frame->ds,

        /* ERROR & FRAME */

        frame->error_code,

        frame->frame_pointer,

        /*  BY CPU  */
        frame->eip,
        frame->cs,
        frame->eflags,
        frame->esp,
        frame->ss
    };

    Panic("Gernal Protection Fault!\n", &pd);

    while (true);
}
INT_HANDLER(DivisionError)
{

    PanicData pd = {
        /*  REGISTERS  */
        frame->edi,
        frame->esi,
        frame->ebp,
        frame->esp_dummy,
        frame->ebx,
        frame->edx,
        frame->ecx,
        frame->eax,
        frame->gs,
        frame->fs,
        frame->es,
        frame->ds,

        /* ERROR & FRAME */

        frame->error_code,

        frame->frame_pointer,

        /*  BY CPU  */
        frame->eip,
        frame->cs,
        frame->eflags,
        frame->esp,
        frame->ss };

    Panic("Division Error!\n", &pd);

    while (true);
}
INT_HANDLER(Debug)
{

    PanicData pd = {
        /*  REGISTERS  */
        frame->edi,
        frame->esi,
        frame->ebp,
        frame->esp_dummy,
        frame->ebx,
        frame->edx,
        frame->ecx,
        frame->eax,
        frame->gs,
        frame->fs,
        frame->es,
        frame->ds,

        /* ERROR & FRAME */

        frame->error_code,

        frame->frame_pointer,

        /*  BY CPU  */
        frame->eip,
        frame->cs,
        frame->eflags,
        frame->esp,
        frame->ss };

    Panic("Debug!\n", &pd);

    while (true)
        ;
}
INT_HANDLER(Breakpoint)
{

    PanicData pd = {
        /*  REGISTERS  */
        frame->edi,
        frame->esi,
        frame->ebp,
        frame->esp_dummy,
        frame->ebx,
        frame->edx,
        frame->ecx,
        frame->eax,
        frame->gs,
        frame->fs,
        frame->es,
        frame->ds,

        /* ERROR & FRAME */

        frame->error_code,

        frame->frame_pointer,

        /*  BY CPU  */
        frame->eip,
        frame->cs,
        frame->eflags,
        frame->esp,
        frame->ss };

    Panic("Breakpoint!\n", &pd);

    while (true)
        ;
}
INT_HANDLER(Overflow)
{

    PanicData pd = {
        /*  REGISTERS  */
        frame->edi,
        frame->esi,
        frame->ebp,
        frame->esp_dummy,
        frame->ebx,
        frame->edx,
        frame->ecx,
        frame->eax,
        frame->gs,
        frame->fs,
        frame->es,
        frame->ds,

        /* ERROR & FRAME */

        frame->error_code,

        frame->frame_pointer,

        /*  BY CPU  */
        frame->eip,
        frame->cs,
        frame->eflags,
        frame->esp,
        frame->ss };

    Panic("Overflow!\n", &pd);

    while (true)
        ;
}
INT_HANDLER(BoundRangeExceeded)
{

    PanicData pd = {
        /*  REGISTERS  */
        frame->edi,
        frame->esi,
        frame->ebp,
        frame->esp_dummy,
        frame->ebx,
        frame->edx,
        frame->ecx,
        frame->eax,
        frame->gs,
        frame->fs,
        frame->es,
        frame->ds,

        /* ERROR & FRAME */

        frame->error_code,

        frame->frame_pointer,

        /*  BY CPU  */
        frame->eip,
        frame->cs,
        frame->eflags,
        frame->esp,
        frame->ss };

    Panic("Bound Range Exceeded!\n", &pd);

    while (true)
        ;
}
INT_HANDLER(InvalidOpcode)
{

    PanicData pd = {
        /*  REGISTERS  */
        frame->edi,
        frame->esi,
        frame->ebp,
        frame->esp_dummy,
        frame->ebx,
        frame->edx,
        frame->ecx,
        frame->eax,
        frame->gs,
        frame->fs,
        frame->es,
        frame->ds,

        /* ERROR & FRAME */

        frame->error_code,

        frame->frame_pointer,

        /*  BY CPU  */
        frame->eip,
        frame->cs,
        frame->eflags,
        frame->esp,
        frame->ss };

    Panic("Invalid Opcode!\n", &pd);

    while (true)
        ;
}
INT_HANDLER(DeviceNotAvailable)
{

    PanicData pd = {
        /*  REGISTERS  */
        frame->edi,
        frame->esi,
        frame->ebp,
        frame->esp_dummy,
        frame->ebx,
        frame->edx,
        frame->ecx,
        frame->eax,
        frame->gs,
        frame->fs,
        frame->es,
        frame->ds,

        /* ERROR & FRAME */

        frame->error_code,

        frame->frame_pointer,

        /*  BY CPU  */
        frame->eip,
        frame->cs,
        frame->eflags,
        frame->esp,
        frame->ss };

    Panic("Device Not Available!\n", &pd);

    while (true)
        ;
}
INT_HANDLER(InvalidTSS)
{

    PanicData pd = {
        /*  REGISTERS  */
        frame->edi,
        frame->esi,
        frame->ebp,
        frame->esp_dummy,
        frame->ebx,
        frame->edx,
        frame->ecx,
        frame->eax,
        frame->gs,
        frame->fs,
        frame->es,
        frame->ds,

        /* ERROR & FRAME */

        frame->error_code,

        frame->frame_pointer,

        /*  BY CPU  */
        frame->eip,
        frame->cs,
        frame->eflags,
        frame->esp,
        frame->ss };

    Panic("Invalid TSS!\n", &pd);

    while (true)
        ;
}
INT_HANDLER(SegmentNotPresent)
{

    PanicData pd = {
        /*  REGISTERS  */
        frame->edi,
        frame->esi,
        frame->ebp,
        frame->esp_dummy,
        frame->ebx,
        frame->edx,
        frame->ecx,
        frame->eax,
        frame->gs,
        frame->fs,
        frame->es,
        frame->ds,

        /* ERROR & FRAME */

        frame->error_code,

        frame->frame_pointer,

        /*  BY CPU  */
        frame->eip,
        frame->cs,
        frame->eflags,
        frame->esp,
        frame->ss };

    Panic("Segment Not Present!\n", &pd);

    while (true)
        ;
}
FAULT_HANDLER(StackSegment)
{

    PanicData pd = {
        /*  REGISTERS  */
        frame->edi,
        frame->esi,
        frame->ebp,
        frame->esp_dummy,
        frame->ebx,
        frame->edx,
        frame->ecx,
        frame->eax,
        frame->gs,
        frame->fs,
        frame->es,
        frame->ds,

        /* ERROR & FRAME */

        frame->error_code,

        frame->frame_pointer,

        /*  BY CPU  */
        frame->eip,
        frame->cs,
        frame->eflags,
        frame->esp,
        frame->ss };

    Panic("Stack Segment Fault!\n", &pd);

    while (true)
        ;
}
INT_HANDLER(AlignmentCheck)
{

    PanicData pd = {
        /*  REGISTERS  */
        frame->edi,
        frame->esi,
        frame->ebp,
        frame->esp_dummy,
        frame->ebx,
        frame->edx,
        frame->ecx,
        frame->eax,
        frame->gs,
        frame->fs,
        frame->es,
        frame->ds,

        /* ERROR & FRAME */

        frame->error_code,

        frame->frame_pointer,

        /*  BY CPU  */
        frame->eip,
        frame->cs,
        frame->eflags,
        frame->esp,
        frame->ss };

    Panic("Alignment Check!\n", &pd);

    while (true)
        ;
}
INT_HANDLER(MachineCheck)
{

    PanicData pd = {
        /*  REGISTERS  */
        frame->edi,
        frame->esi,
        frame->ebp,
        frame->esp_dummy,
        frame->ebx,
        frame->edx,
        frame->ecx,
        frame->eax,
        frame->gs,
        frame->fs,
        frame->es,
        frame->ds,

        /* ERROR & FRAME */

        frame->error_code,

        frame->frame_pointer,

        /*  BY CPU  */
        frame->eip,
        frame->cs,
        frame->eflags,
        frame->esp,
        frame->ss };

    Panic("Machine Check!\n", &pd);

    while (true)
        ;
}
INT_HANDLER(VirtualizationException)
{

    PanicData pd = {
        /*  REGISTERS  */
        frame->edi,
        frame->esi,
        frame->ebp,
        frame->esp_dummy,
        frame->ebx,
        frame->edx,
        frame->ecx,
        frame->eax,
        frame->gs,
        frame->fs,
        frame->es,
        frame->ds,

        /* ERROR & FRAME */

        frame->error_code,

        frame->frame_pointer,

        /*  BY CPU  */
        frame->eip,
        frame->cs,
        frame->eflags,
        frame->esp,
        frame->ss };

    Panic("Virtualization Exception!\n", &pd);

    while (true)
        ;
}
INT_HANDLER(ControlProtectionException)
{

    PanicData pd = {
        /*  REGISTERS  */
        frame->edi,
        frame->esi,
        frame->ebp,
        frame->esp_dummy,
        frame->ebx,
        frame->edx,
        frame->ecx,
        frame->eax,
        frame->gs,
        frame->fs,
        frame->es,
        frame->ds,

        /* ERROR & FRAME */

        frame->error_code,

        frame->frame_pointer,

        /*  BY CPU  */
        frame->eip,
        frame->cs,
        frame->eflags,
        frame->esp,
        frame->ss };

    Panic("Control Protection Exception!\n", &pd);

    while (true)
        ;
}
INT_HANDLER(HypervisorInjectionException)
{

    PanicData pd = {
        /*  REGISTERS  */
        frame->edi,
        frame->esi,
        frame->ebp,
        frame->esp_dummy,
        frame->ebx,
        frame->edx,
        frame->ecx,
        frame->eax,
        frame->gs,
        frame->fs,
        frame->es,
        frame->ds,

        /* ERROR & FRAME */

        frame->error_code,

        frame->frame_pointer,

        /*  BY CPU  */
        frame->eip,
        frame->cs,
        frame->eflags,
        frame->esp,
        frame->ss };

    Panic("Hypervisor Injection Exception!\n", &pd);

    while (true)
        ;
}
INT_HANDLER(VMMCommunicationException)
{

    PanicData pd = {
        /*  REGISTERS  */
        frame->edi,
        frame->esi,
        frame->ebp,
        frame->esp_dummy,
        frame->ebx,
        frame->edx,
        frame->ecx,
        frame->eax,
        frame->gs,
        frame->fs,
        frame->es,
        frame->ds,

        /* ERROR & FRAME */

        frame->error_code,

        frame->frame_pointer,

        /*  BY CPU  */
        frame->eip,
        frame->cs,
        frame->eflags,
        frame->esp,
        frame->ss };

    Panic("VMM Communication Exception!\n", &pd);

    while (true)
        ;
}
INT_HANDLER(SecurityException)
{

    PanicData pd = {
        /*  REGISTERS  */
        frame->edi,
        frame->esi,
        frame->ebp,
        frame->esp_dummy,
        frame->ebx,
        frame->edx,
        frame->ecx,
        frame->eax,
        frame->gs,
        frame->fs,
        frame->es,
        frame->ds,

        /* ERROR & FRAME */

        frame->error_code,

        frame->frame_pointer,

        /*  BY CPU  */
        frame->eip,
        frame->cs,
        frame->eflags,
        frame->esp,
        frame->ss };

    Panic("Security Exception!\n", &pd);

    while (true)
        ;
}
FAULT_HANDLER(Triple)
{

    PanicData pd = {
        /*  REGISTERS  */
        frame->edi,
        frame->esi,
        frame->ebp,
        frame->esp_dummy,
        frame->ebx,
        frame->edx,
        frame->ecx,
        frame->eax,
        frame->gs,
        frame->fs,
        frame->es,
        frame->ds,

        /* ERROR & FRAME */

        frame->error_code,

        frame->frame_pointer,

        /*  BY CPU  */
        frame->eip,
        frame->cs,
        frame->eflags,
        frame->esp,
        frame->ss };

    Panic("Triple Fault!\n", &pd);

    while (true)
        ;
}

__attribute__((interrupt)) void UtilCall_handler(interrupt_frame* frame){
    Serial::WriteFormat(COM1, "Util Interrupt was called with ebx=%x\n", frame->ebx);

    frame->eax = 0xAA32;
}

INT_HANDLER(Keyboard)
{

    uint8_t scancode = inb(0x60);

    HandleKeyboard(scancode);

    PIC_EndMaster();
}

INT_HANDLER(Mouse)
{
    uint8_t mouseData = inb(0x60);

    HandlePS2Mouse(mouseData);

    PIC_EndSlave();
}

INT_HANDLER(PIT)
{
    PIT::Tick();

    PIC_EndMaster();
}

void _InterruptHandler(interrupt_frame* frame)
{
    PINTERRUPT_VECTOR pUserVector = nullptr;

    pUserVector = KeGetInterruptVector(frame->vec_no);

    KeInvokeRawVector(pUserVector, frame);
}

/* PIC Functions */
void PIC_EndMaster()
{
    outb(PIC1_COMMAND, PIC_EOI);
}

void PIC_EndSlave()
{
    outb(PIC2_COMMAND, PIC_EOI);
    outb(PIC1_COMMAND, PIC_EOI);
}

void RemapPIC()
{
    uint8_t a1, a2;

    a1 = inb(PIC1_DATA);
    io_wait();
    a2 = inb(PIC2_DATA);
    io_wait();

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();

    outb(PIC1_DATA, 0x20);
    io_wait();
    outb(PIC2_DATA, 0x28);
    io_wait();

    outb(PIC1_DATA, 4);
    io_wait();
    outb(PIC2_DATA, 2);
    io_wait();

    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    outb(PIC1_DATA, a1);
    io_wait();
    outb(PIC2_DATA, a2);
}

void DisablePIC() {
    cli();

    RemapPIC();

    outb(PIC1_DATA, 0);
    outb(PIC2_DATA, 0);

    sti();
}

INTERRUPT_VECTOR* InterruptVectors;
size_t InterruptVectorCompacity = 0;
size_t InterruptCount = 0;

ANTSTATUS KeInitializeInterrupts()
{
    

    return STATUS_SUCCESS;
}


PINTERRUPT_VECTOR KeGetInterruptVector(uint32_t num)
{
    return PINTERRUPT_VECTOR();
}
void KeSetInterruptVector(uint32_t num, PINTERRUPT_VECTOR pVector)
{
}

void KeInvokeRawVector(PINTERRUPT_VECTOR pVector, interrupt_frame* _Frame)
{
    if (pVector == nullptr) return;

    if (pVector->CallbackCount == 0){
        Serial::WriteFormat(COM1, "WARNING: Tried to call Raw Interrupt Vector with no callbacks attached to it!\n    ¦ Vector=%p;CallbackBase=%p;VectorNumber=%d\n\n", pVector, pVector->Callbacks, pVector->Number);
    }

    for (size_t i = 0; i < pVector->CallbackCount; i++)
    {
        if (pVector->Callbacks[i] == nullptr){
            Serial::WriteFormat(COM1, "WARNING: Invalid Callback at index %d for Vector(0x%p)!\n", i, pVector);
        }else
            __UNSAFE {
                ANTSTATUS result = pVector->Callbacks[i](_Frame); /* This is UNSAFE! Just calling into unknow, unchecked code! */

                /* We really need a logger with level and muti stream support! */
                Serial::WriteFormat(COM1, "INFO: The Interrupt Callback at 0x%p returned 0x%x", pVector->Callbacks[i], result);
            }
    }
    
    
}
