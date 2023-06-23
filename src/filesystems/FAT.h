#pragma once
#include <stdint.h>
#include "../ahci/ahci.h"
#include "../memory.h"
#include "../memory/heap.h"
#include "../math.h"
#include "../cstr.h"
#include <ant/cstring.h>

typedef void* buffer_t;

#define SECTOR_SIZE             512
#define MAX_FILE_HANDLES        10
#define ROOT_DIRECTORY_HANDLE   -1
#define FAT_CACHE_SIZE          5
#define MAX_PATH_SIZE           256

namespace FAT{

    typedef struct
    {
        // extended boot record
        uint8_t DriveNumber;
        uint8_t _Reserved;
        uint8_t Signature;
        uint32_t VolumeId;          // serial number, value doesn't matter
        uint8_t VolumeLabel[11];    // 11 bytes, padded with spaces
        uint8_t SystemId[8];
    } __attribute__((packed)) ExtendedBootRecord;

    typedef struct
    {
        uint32_t SectorsPerFat;
        uint16_t Flags;
        uint16_t FatVersion;
        uint32_t RootDirectoryCluster;
        uint16_t FSInfoSector;
        uint16_t BackupBootSector;
        uint8_t _Reserved[12];
        ExtendedBootRecord EBR;

    } __attribute((packed)) ExtendedBootRecord32;

    typedef struct
    {
        uint8_t BootJumpInstruction[3];
        uint8_t OemIdentifier[8];
        uint16_t BytesPerSector;
        uint8_t SectorsPerCluster;
        uint16_t ReservedSectors;
        uint8_t FatCount;
        uint16_t DirEntryCount;
        uint16_t TotalSectors;
        uint8_t MediaDescriptorType;
        uint16_t SectorsPerFat;
        uint16_t SectorsPerTrack;
        uint16_t Heads;
        uint32_t HiddenSectors;
        uint32_t LargeSectorCount;

        union {
            ExtendedBootRecord EBR1216;
            ExtendedBootRecord32 EBR32;
        };

        // ... we don't care about code ...

    } __attribute__((packed)) BootSector;

    typedef struct
    {
        int Handle;
        bool IsDirectory;
        uint32_t Position;
        uint32_t Size;
    } File;

    enum Attributes
    {
        ATTRIBUTE_READ_ONLY = 0x01,
        ATTRIBUTE_HIDDEN = 0x02,
        ATTRIBUTE_SYSTEM = 0x04,
        ATTRIBUTE_VOLUME_ID = 0x08,
        ATTRIBUTE_DIRECTORY = 0x10,
        ATTRIBUTE_ARCHIVE = 0x20,
        ATTRIBUTE_LFN = ATTRIBUTE_READ_ONLY | ATTRIBUTE_HIDDEN | ATTRIBUTE_SYSTEM | ATTRIBUTE_VOLUME_ID
    };

    typedef struct
    {
        uint8_t Buffer[SECTOR_SIZE];
        File Public;
        bool Opened;
        uint32_t FirstCluster;
        uint32_t CurrentCluster;
        uint32_t CurrentSectorInCluster;

    } FileData;

    typedef struct
    {
        uint8_t Name[11];
        uint8_t Attributes;
        uint8_t _Reserved;
        uint8_t CreatedTimeTenths;
        uint16_t CreatedTime;
        uint16_t CreatedDate;
        uint16_t AccessedDate;
        uint16_t FirstClusterHigh;
        uint16_t ModifiedTime;
        uint16_t ModifiedDate;
        uint16_t FirstClusterLow;
        uint32_t Size;
    } __attribute__((packed)) DirectoryEntry;


    class Filesystem{
        private:
            AHCI::Port* m_port;
            uint8_t m_bootSector[512];
            uint8_t Type;
            uint8_t Cache[FAT_CACHE_SIZE * SECTOR_SIZE];
            uint32_t CachePosition;
            uint32_t TotalSectors;
            uint32_t SectorsPerFat;
            FileData RootDirectory;
            FileData OpenedFiles[MAX_FILE_HANDLES];
            uint32_t DataSectionLba;

        public:
            Filesystem(AHCI::Port*);
            ~Filesystem();

            bool Initialize();
            void Detect();
            uint16_t GetSize();
            char* GetType();
            char* GetVolumeLable();
            uint8_t GetDriveNumber();
            bool IsBootSectorValid();
            uint16_t GetTotalSectors();
            BootSector* GetBootSector();
            uint32_t ClusterToLba(uint32_t);
            File* OpenEntry(DirectoryEntry*);
            bool ReadFat(uint32_t);
            uint32_t GetOpenFileHandleCount();
            uint32_t NextCluster(uint32_t);
            uint32_t Read(File*, uint32_t, buffer_t);
            bool ReadEntry(FAT::File*, DirectoryEntry*);
            void GetShortName(const char*, char[12]);
            void Close(File*);
            bool FindFile(File*, const char*,  DirectoryEntry*);
            File* Open(const char*);
            FileData* GetFileData(File* file);
            File* GetRoot();
    };

}