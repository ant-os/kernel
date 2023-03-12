#include "ahci.h"
#include "../Serial.h"

namespace AHCI{
    AHCIDriver::AHCIDriver(PCI::PCIDeviceHeader* pciBaseAddress)
    {
        this->PCIBaseAddress = pciBaseAddress;
        
        ShowSucess(COM1, "\r\nAHCI Driver instance initialized\r\n");
    }

    AHCIDriver::~AHCIDriver(){
    
    }
} // namespace AHCI
