#include "FAT.h"



FAT::Filesystem::Filesystem(AHCI::Port* port)
{
    // Prepare all members...
    this->m_port = port;
}

FAT::Filesystem::~Filesystem()
{

}

bool FAT::Filesystem::Initialize()
{
    m_port->Read(0, 1, (void*)m_bootSector);

    return true;
}

uint64_t FAT::Filesystem::GetSize()
{
    return ((BootSector*)m_bootSector)->FATSize;
}

char* FAT::Filesystem::GetType()
{
    return ((BootSector*)m_bootSector)->BS_FileSystemType;
}

char* FAT::Filesystem::GetVolumeLable()
{
    return ((BootSector*)m_bootSector)->BS_VolumeLabel;
}

uint8_t FAT::Filesystem::GetDriveNumber()
{
    return (uint8_t)((BootSector*)m_bootSector)->BS_DriveNumber;
}
