#pragma once
#include <stdint.h>
#include "../ahci/ahci.h"
#include "../memory.h"
#include "../memory/heap.h"

typedef void* buffer_t;

#define SECTOR_SIZE 512

namespace FAT{

    struct BootSector
    {
        unsigned int      FATSize;             // 4
        unsigned short    ExtFlags;              // 2
        unsigned short    FSVersion;             // 2
        unsigned int      RootCluster;           // 4
        unsigned short    FSInfo;                // 2
        unsigned short    BkBootSec;             // 2
        unsigned char     Reserved[12];          // 12
        unsigned char     BS_DriveNumber;            // 1
        unsigned char     BS_Reserved1;              // 1
        unsigned char     BS_BootSig;                // 1
        unsigned int      BS_VolumeID;               // 4
        char     BS_VolumeLabel[11];        // 11
        char     BS_FileSystemType[8];      // 8
    };

    class Filesystem{
        private:
            AHCI::Port* m_port;
            uint8_t m_bootSector[512];

        public:
            Filesystem(AHCI::Port* port);
            ~Filesystem();

            bool Initialize();
            uint64_t GetSize();
            char* GetType();
            char* GetVolumeLable();
            uint8_t GetDriveNumber();
    };

}