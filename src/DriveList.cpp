#include "DriveList.h"



Drive::Drive(int32_t id, cstring label)
{
    m_DriveID = id;
    m_Label = label;
    m_DriveLetter = '?';
}

int32_t Drive::DriveID()
{
    return m_DriveID;
}

char Drive::DriveLetter()
{
    return m_DriveLetter;
}

cstring Drive::Label()
{
    return m_Label;
}

void Drive::_ConfigureLetter(char _letter)
{
    m_DriveLetter = _letter;
}

Drive* DriveList = NULL;
uint32_t DriveCount = 0;
uint32_t MaxDrives = 0;

void InitializeDriveList(uint32_t maxDrives)
{
    MaxDrives = maxDrives;

    DriveList = (Drive*)malloc(MaxDrives * sizeof(Drive));
    DriveCount = 0;
}

Drive* GetDrive(uint32_t offset)
{
    if (DriveList != NULL) 
        Some(&DriveList[offset]);

    return NULL;
};

bool RemoveDrive(uint32_t offset)
{

    DriveList[offset] = {-1, (cstring)"<invalid>"};
    DriveList[offset]._ConfigureLetter('?');

    --DriveCount; 

    Ok();
}

bool AddDrive(Drive newDrive)
{
    if (DriveList == NULL) Err();


    DriveList[DriveCount] = newDrive;

    char new_letter = (char)((uint32_t)'A' + (DriveCount));

    DriveList[DriveCount]._ConfigureLetter(new_letter);

    ++DriveCount;
    Ok();
}
