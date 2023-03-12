#pragma once
#include "../pci.h"

namespace AHCI
{
    class AHCIDriver{
        public:
        AHCIDriver(PCI::PCIDeviceHeader* pciBaseAddress);
        ~AHCIDriver();
        PCI::PCIDeviceHeader* PCIBaseAddress;
    };
    
} // namespace AHCI
