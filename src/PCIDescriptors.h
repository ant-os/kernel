#pragma once
#include <stdint.h>
#include "cstr.h"

namespace PCI
{
    const char* GetClassName(uint8_t classCode);
    const char* GetVendorName(uint16_t vendorID);
    const char* GetDeviceName(uint16_t vendorID, uint16_t deviceID);
    const char* GetSubclassName(uint8_t classCode, uint8_t subclassCode);
    const char* GetProgIFName(uint8_t classCode, uint8_t subclassCode, uint8_t progIF);
} // namespace PCI