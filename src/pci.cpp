#include "pci.h"

namespace PCI{

    void EnumerateFunction(uint64_t deviceAddress, uint64_t function){
        uint64_t offset = function << 12;

        uint64_t functionAddress = deviceAddress + offset;

        g_PageTableManager.MapMemory((void*)functionAddress, (void*)functionAddress);


        PCIDeviceHeader* pciDeviceHeader = (PCIDeviceHeader*)functionAddress;

        if (pciDeviceHeader->DeviceID == 0) return;
        if (pciDeviceHeader->DeviceID == 0xFFFF) return;

        Serial::WriteString(COM1, NEW_LINE);
        Serial::WriteString(COM1, GetVendorName(
            pciDeviceHeader->VendorID
        ));
        Serial::WriteString(COM1, " / ");
        Serial::WriteString(COM1, GetDeviceName(
            pciDeviceHeader->VendorID,
            pciDeviceHeader->DeviceID
        ));
        Serial::WriteString(COM1, " / ");
        Serial::WriteString(COM1, DeviceClasses[pciDeviceHeader->Class]);
        Serial::WriteString(COM1, " / ");
        Serial::WriteString(COM1, GetSubclassName(
            pciDeviceHeader->Class,
            pciDeviceHeader->Subclass
        ));
        Serial::WriteString(COM1, " / ");
        Serial::WriteString(COM1, GetProgIFName(
            pciDeviceHeader->Class,
            pciDeviceHeader->Subclass,
            pciDeviceHeader->ProgIF
        ));
        Serial::WriteString(COM1, NEW_LINE);
    }

    void EnumerateDevice(uint64_t busAddress, uint64_t device){
        uint64_t offset = device << 15;

        uint64_t deviceAddress = busAddress + offset;

        g_PageTableManager.MapMemory((void*)deviceAddress, (void*)deviceAddress);


        PCIDeviceHeader* pciDeviceHeader = (PCIDeviceHeader*)deviceAddress;

        if (pciDeviceHeader->DeviceID == 0) return;
        if (pciDeviceHeader->DeviceID == 0xFFFF) return;

        for (uint64_t function = 0; function < 8; function++)
        {
            EnumerateFunction(deviceAddress, function);
        }
    }

    void EnumerateBus(uint64_t baseAddress, uint64_t bus){
        uint64_t offset = bus << 20;

        uint64_t busAddress = baseAddress + offset;

        g_PageTableManager.MapMemory((void*)busAddress, (void*)busAddress);


        PCIDeviceHeader* pciDeviceHeader = (PCIDeviceHeader*)busAddress;

        if (pciDeviceHeader->DeviceID == 0) return;
        if (pciDeviceHeader->DeviceID == 0xFFFF) return;

        for (uint64_t device = 0; device < 32; device++)
        {
            EnumerateDevice(busAddress, device);
        }
        
    }


    void EnumeratePCI(ACPI::MCFGHeader* mcfg){
        int entries = ((mcfg->Header.Length) - sizeof(ACPI::MCFGHeader)) / sizeof(ACPI::DeviceConfig);

        for (int t = 0; t < entries; t++)
        {
            ACPI::DeviceConfig* newDeviceConfig = (ACPI::DeviceConfig*)((uint64_t)mcfg + sizeof(ACPI::MCFGHeader) + (sizeof(ACPI::DeviceConfig) * t));

            for (int bus = newDeviceConfig->StartBus; bus < newDeviceConfig->EndBus; bus++)
            {
                EnumerateBus(newDeviceConfig->BaseAddress, bus);
            }
            
        }
        
    }

}