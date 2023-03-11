#pragma once
#include <stdint.h>
#include "cstr.h"

namespace PCI
{
    const char* DeviceClasses[]{
        "Unclassified",
        "Mass Storage Controller",
        "Network Controller",
        "Display Controller",
        "Multimedia Controller",
        "Memory Controller",
        "Bridge Device",
        "Simple Communication Controller",
        "Base System Peripheral",
        "Input Device Controller",
        "Docking Station",
        "Processor",
        "Serial Bus Controller",
        "Wireless Controller",
        "Intelligent Controller",
        "Satellite Communication Controller",
        "Encryption Controller",
        "Signal Processing Controller",
        "Processing Accelerator",
        "Non Essential Instrumentation"
    };

    const char* GetVendorName(uint16_t vendorID);
    const char* GetDeviceName(uint16_t vendorID, uint16_t deviceID);
    const char* GetSubclassName(uint8_t classCode, uint8_t subclassCode);
    const char* GetProgIFName(uint8_t classCode, uint8_t subclassCode, uint8_t progIF);
} // namespace PCI