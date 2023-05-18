#include "ahci.h"
#include "../Serial.h"
#include "../paging/PageTableManager.h"
#include "../memory/heap.h"
#include "../bits.h"
#include "../paging/PageFrameAllocator.h"
#include "../filesystems/FAT.h"

unsigned char buf[520];

namespace AHCI {

    void Port::Configure() {

        StopCMD();

        void* newBase = GlobalAllocator.RequestPage();
        hbaPort->commandListBase = (uint32_t)(uint64_t)newBase;
        hbaPort->commandListBaseUpper = (uint32_t)((uint64_t)newBase >> 32);
        ZeroMemory(hbaPort->commandListBase, 1024);

        void* fisBase = GlobalAllocator.RequestPage();
        hbaPort->fisBaseAddress = (uint32_t)(uint64_t)fisBase;
        hbaPort->fisBaseAddressUpper = (uint32_t)((uint64_t)fisBase >> 32);
        ZeroMemory(hbaPort->fisBaseAddress, 256);

        HBACommandHeader* cmdHeader = (HBACommandHeader*)COMBINE(hbaPort->commandListBase, hbaPort->commandListBaseUpper);

        for (int i = 0; i < 32; i++)
        {
            cmdHeader[i].prdtLength = 8;

            void* cmdTableAddress = GlobalAllocator.RequestPage();
            uint64_t address = (uint64_t)cmdTableAddress + (i << 8);
            cmdHeader[i].commandTableBaseAddress = (uint32_t)address;
            cmdHeader[i].commandTableBaseAddress = (uint32_t)((uint64_t)address >> 32);
            ZeroMemory(cmdTableAddress, 256);

        }


        StartCMD();

    }

    void Port::StopCMD() {
        BITMASK_CLEAR(hbaPort->cmdSts, HBA_PxCMD_ST);
        BITMASK_CLEAR(hbaPort->cmdSts, HBA_PxCMD_FRE);

        while (true)
        {
            if (BITMASK_CHECK_ANY(hbaPort->cmdSts, HBA_PxCMD_FR)) continue;
            if (BITMASK_CHECK_ANY(hbaPort->cmdSts, HBA_PxCMD_CR)) continue;

            break;
        }


    }

    void Port::StartCMD() {
        while (BITMASK_CHECK_ANY(hbaPort->cmdSts, HBA_PxCMD_CR));

        BITMASK_SET(hbaPort->cmdSts, HBA_PxCMD_FRE);
        BITMASK_SET(hbaPort->cmdSts, HBA_PxCMD_ST);

    }

    bool Port::Read(uint64_t sector, uint32_t sectorCount, void* buffer) {
        uint32_t sectorL = (uint32_t)sector;
        uint32_t sectorH = (uint32_t)(sector >> 32);

        hbaPort->interruptStatus = (uint32_t)-1; // Clear pending interrupt bits

        HBACommandHeader* cmdHeader = (HBACommandHeader*)hbaPort->commandListBase;
        cmdHeader->commandFISLength = sizeof(FIS_REG_H2D) / sizeof(uint32_t); //command FIS size;
        cmdHeader->write = 0; //this is a read
        cmdHeader->prdtLength = 1;

        HBACommandTable* commandTable = (HBACommandTable*)(cmdHeader->commandTableBaseAddress);
        memset(commandTable, 0, sizeof(HBACommandTable) + (cmdHeader->prdtLength - 1) * sizeof(HBAPRDTEntry));

        commandTable->prdtEntry[0].dataBaseAddress = (uint32_t)(uint64_t)buffer;
        commandTable->prdtEntry[0].dataBaseAddressUpper = (uint32_t)((uint64_t)buffer >> 32);
        commandTable->prdtEntry[0].byteCount = (sectorCount << 9) - 1; // 512 bytes per sector
        commandTable->prdtEntry[0].interruptOnCompletion = 1;

        FIS_REG_H2D* cmdFIS = (FIS_REG_H2D*)(&commandTable->commandFIS);

        cmdFIS->fisType = FIS_TYPE_REG_H2D;
        cmdFIS->commandControl = 1; // command
        cmdFIS->command = ATA_CMD_READ_DMA_EX;

        cmdFIS->lba0 = (uint8_t)sectorL;
        cmdFIS->lba1 = (uint8_t)(sectorL >> 8);
        cmdFIS->lba2 = (uint8_t)(sectorL >> 16);
        cmdFIS->lba3 = (uint8_t)sectorH;
        cmdFIS->lba4 = (uint8_t)(sectorH >> 8);
        cmdFIS->lba4 = (uint8_t)(sectorH >> 16);

        cmdFIS->deviceRegister = 1 << 6; //LBA mode

        cmdFIS->countLow = sectorCount & 0xFF;
        cmdFIS->countHigh = (sectorCount >> 8) & 0xFF;

        uint64_t spin = 0;

        while ((hbaPort->taskFileData & (ATA_DEV_BUSY | ATA_DEV_DRQ)) && spin < 1000000) {
            spin++;
        }
        if (spin == 1000000) {
            return false;
        }

        hbaPort->commandIssue = 1;

        while (true) {

            if ((hbaPort->commandIssue == 0)) break;
            if (hbaPort->interruptStatus & HBA_PxIS_TFES)
            {
                return false;
            }
        }

        return true;
    }

    uint8_t test_buf[(SECTOR_SIZE * 8) + 1] = { 0 };

    AHCIDriver::AHCIDriver(PCI::PCIDeviceHeader* pciBaseAddress)
    {
        this->PCIBaseAddress = pciBaseAddress;

        ShowSucess(COM1, "\r\nAHCI Driver instance initialized\r\n");

        ABAR = (HBAMemory*)((PCI::PCIHeader0*)pciBaseAddress)->BAR5;

        g_PageTableManager.MapMemory(ABAR, ABAR);

        ProbePorts();

        for (int i = 0; i < portCount; i++)
        {
            Port* port = Ports[i];

            if (port->portType == PortType::SATA) Log(, "SATA Port Found!\n\r");
            if (port->portType == PortType::SATAPI) Log(, "SATAPI Port Found!\n\r");

            port->Configure();
            
            FAT::Filesystem fs = FAT::Filesystem(port);

            if (!fs.Initialize()){
                ShowError(COM1, "Failed to initialize fs!");
            }

        }

    }

    AHCIDriver::~AHCIDriver() {

    }

    void AHCIDriver::ProbePorts()
    {
        uint32_t portsImplemented = ABAR->portsImplemented;
        for (int i = 0; i < 32; i++)
        {
            if (portsImplemented & (1 << i)) {
                PortType portType = CheckPortType(&ABAR->ports[i]);

                if (portType == PortType::SATA || portType == PortType::SATAPI) {
                    Ports[portCount] = new Port();
                    Ports[portCount]->portType = portType;
                    Ports[portCount]->hbaPort = &ABAR->ports[i];

                    portCount++;
                }

            }
        }
    }
    PortType AHCIDriver::CheckPortType(HBAPort* port)
    {
        uint32_t sataStatus = port->sataStatus;

        uint8_t interfacePowerManagement = (sataStatus >> 8) & 0b111;
        uint8_t deviceDetection = sataStatus & 0b111;

        if (deviceDetection != HBA_PORT_DEVICE_PRESENT) return PortType::None;
        if (interfacePowerManagement != HBA_PORT_IPM_ACTIVE) return PortType::None;

        switch (port->signature) {
        case SataSignature::ATAPI:
            return PortType::SATAPI;
        case SataSignature::ATA:
            return PortType::SATA;
        case SataSignature::SEMB:
            return PortType::SEMB;
        case SataSignature::PM:
            return PortType::PM;
        default:
            return PortType::None;
        }
    }
} // namespace AHCI
