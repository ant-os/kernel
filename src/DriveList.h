#pragma once
#include "cstr.h"
#include <stdint.h>
#include "memory/heap.h"

#define Err()   return false
#define Ok()    return true
#define Some(x) return x


class Drive{
    public:
        Drive(int32_t id, cstring lable, size_t size);

        int32_t DriveID();
        char DriveLetter();
        cstring Label();
        size_t Size();

        void _ConfigureLetter(char _letter);

    private:
        int32_t m_DriveID;
        cstring m_Label;
        char m_DriveLetter;
        size_t m_Size;
};

extern uint32_t DriveCount;
extern Drive *DriveList;

void InitializeDriveList(uint32_t maxDrives);
Drive* GetDrive(uint32_t offset);
bool RemoveDrive(uint32_t offset);
bool AddDrive(Drive newDrive);
