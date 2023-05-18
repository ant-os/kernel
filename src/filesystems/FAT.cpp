#include "FAT.h"

FAT::Filesystem::Filesystem(AHCI::Port* port)
{
    Log(, "Creating Filesystem object for Port Number ");
    Serial::WriteNumber(COM1, port->portNumber);

    // Prepare all members...
    this->m_port = port;
}

FAT::Filesystem::~Filesystem()
{
    /* 
        We don't have any dynamicly allocated memory, so this function dosen't do anything!
    */

   ShowDebug(COM1, "Distroying Filesystem object!\r\n");
}

bool FAT::Filesystem::Initialize()
{
    m_port->Read(0, 1, (void*)m_bootSector);

    

    BootSector* BS = GetBootSector();

    CachePosition = 0xFFFFFFFF;

    TotalSectors =  BS->TotalSectors;
    if (TotalSectors == 0){ // fat32
        TotalSectors = BS->LargeSectorCount; 
    }

    bool isFat32 = false;
    SectorsPerFat = BS->SectorsPerFat;
    if (SectorsPerFat == 0) {         // fat32
        isFat32 = true;
        SectorsPerFat = BS->EBR32.SectorsPerFat;
    }

    uint32_t rootDirLba;
    uint32_t rootDirSize;
    if (isFat32) {
        DataSectionLba = BS->ReservedSectors + SectorsPerFat * BS->FatCount;
    }
    else {
        rootDirLba = BS->ReservedSectors + SectorsPerFat * BS->FatCount;
        rootDirSize = sizeof(DirectoryEntry) * BS->DirEntryCount;
        uint32_t rootDirSectors = (rootDirSize + BS->BytesPerSector - 1) / BS->BytesPerSector;
        DataSectionLba = rootDirLba + rootDirSectors;
    }

    RootDirectory.Public.Handle = ROOT_DIRECTORY_HANDLE;
    RootDirectory.Public.IsDirectory = true;
    RootDirectory.Public.Position = 0;
    RootDirectory.Public.Size = sizeof(DirectoryEntry) * BS->DirEntryCount;
    RootDirectory.Opened = true;
    RootDirectory.FirstCluster = rootDirLba;
    RootDirectory.CurrentCluster = rootDirLba;
    RootDirectory.CurrentSectorInCluster = 0;

    if(!m_port->Read(rootDirLba, 1, RootDirectory.Buffer)){
        ShowError(COM1, "Failed to read root directory!\n\r");
        return false;
    }

    for (int i = 0; i < MAX_FILE_HANDLES; i++)
        OpenedFiles[i].Opened = false;


    Detect();

    return true;
}

void FAT::Filesystem::Detect()
{
    uint32_t dataClusters = (TotalSectors - DataSectionLba) / (GetBootSector())->SectorsPerCluster;
    if (dataClusters < 0xFF5)
        Type = 12;
    else if ((GetBootSector())->SectorsPerFat != 0)
        Type = 16;
    else Type = 32;
}

uint32_t FAT::Filesystem::ClusterToLba(uint32_t cluster)
{
    return DataSectionLba + (cluster - 2) * (GetBootSector())->SectorsPerCluster;
}

FAT::File* FAT::Filesystem::OpenEntry(DirectoryEntry* entry)
{
    int handle = -1;
    for (int i = 0; i < MAX_FILE_HANDLES && handle < 0; i++)
    {
        if (!OpenedFiles[i].Opened)
            handle = i;
    }

    // out of handles
    if (handle < 0)
    {
        ShowError(COM1, "FAT: out of file handles\r\n");
        return NULL;
    }

    // setup vars
    FileData* fd = &OpenedFiles[handle];
    fd->Public.Handle = handle;
    fd->Public.IsDirectory = (entry->Attributes & ATTRIBUTE_DIRECTORY) != 0;
    fd->Public.Position = 0;
    fd->Public.Size = entry->Size;
    fd->FirstCluster = entry->FirstClusterLow + ((uint32_t)entry->FirstClusterHigh << 16);
    fd->CurrentCluster = fd->FirstCluster;
    fd->CurrentSectorInCluster = 0;

    if (!m_port->Read(ClusterToLba(fd->CurrentCluster), 1, fd->Buffer))
    {
        ShowError(COM1, "FAT: <filesystem object>::OpenEntry() failed") // - read error cluster=%u lba=%u\n", fd->CurrentCluster, FAT_ClusterToLba(fd->CurrentCluster));
        for (int i = 0; i < 11; i++)
            Serial::Write(COM1, (char)entry->Name[i]);
        Serial::Write(COM1, '\n\r');
        return NULL;
    }

    fd->Opened = true;
    return &fd->Public;
}


bool FAT::Filesystem::IsBootSectorValid() {
    if (m_bootSector[0] != 0xEB || m_bootSector[1] != 0x58 || m_bootSector[2] != 0x90) {
        return false;
    }
    if (m_bootSector[510] != 0x55 || m_bootSector[511] != 0xAA) {
        return false;
    }
    return true;
}

uint16_t FAT::Filesystem::GetTotalSectors()
{
    return TotalSectors;
}

FAT::BootSector* FAT::Filesystem::GetBootSector()
{
    return (BootSector*)m_bootSector;
}

bool FAT::Filesystem::ReadFat(uint32_t lbaIndex)
{
    return m_port->Read((GetBootSector())->ReservedSectors + lbaIndex, FAT_CACHE_SIZE, Cache);
}

uint32_t FAT::Filesystem::GetOpenFileHandleCount()
{
    uint32_t count;

    for (uint32_t i = 0; i < MAX_FILE_HANDLES; i++)
    {
        if (OpenedFiles[i].Opened)
            count += 1;
    }
    
    return count;
}

uint32_t FAT::Filesystem::NextCluster(uint32_t currentCluster)
{
    // Determine the byte offset of the entry we need to read
    uint32_t fatIndex;
    if (Type == 12) {
        fatIndex = currentCluster * 3 / 2;
    }
    else if (Type == 16) {
        fatIndex = currentCluster * 2;
    }
    else /*if (g_FatType == 32)*/ {
        fatIndex = currentCluster * 4;
    }

    // Make sure cache has the right number
    uint32_t fatIndexSector = fatIndex / SECTOR_SIZE;
    if (
        fatIndexSector < CachePosition
        || fatIndexSector >= CachePosition + FAT_CACHE_SIZE)
    {
        ReadFat(fatIndexSector);
        CachePosition = fatIndexSector;
    }

    fatIndex -= (CachePosition * SECTOR_SIZE);

    uint32_t nextCluster;
    if (Type == 12) {
        if (currentCluster % 2 == 0)
            nextCluster = (*(uint16_t*)(Cache + fatIndex)) & 0x0FFF;
        else
            nextCluster = (*(uint16_t*)(Cache + fatIndex)) >> 4;

        if (nextCluster >= 0xFF8) {
            nextCluster |= 0xFFFFF000;
        }
    }
    else if (Type == 16) {
        nextCluster = *(uint16_t*)(Cache + fatIndex);
        if (nextCluster >= 0xFFF8) {
            nextCluster |= 0xFFFF0000;
        }
    }
    else /*if (Type == 32)*/ {
        nextCluster = *(uint32_t*)(Cache + fatIndex);
    }

    return nextCluster;
}


uint32_t FAT::Filesystem::Read(File* file, uint32_t byteCount, buffer_t dataOut)
{
    BootSector* BS = GetBootSector();

    Log(, "Reading...\n\r");

    // get file data
    FileData* fd = (file->Handle == ROOT_DIRECTORY_HANDLE)
        ? &RootDirectory
        : &OpenedFiles[file->Handle];

    uint8_t* u8DataOut = (uint8_t*)dataOut;

    // don't read past the end of the file
    if (!fd->Public.IsDirectory || (fd->Public.IsDirectory && fd->Public.Size != 0))
        byteCount = (((byteCount) < (fd->Public.Size - fd->Public.Position)) ? (byteCount) : (fd->Public.Size - fd->Public.Position));

    while (byteCount > 0)
    {
        uint32_t leftInBuffer = SECTOR_SIZE - (fd->Public.Position % SECTOR_SIZE);
        uint32_t take = min(byteCount, leftInBuffer);

        memcpy(fd->Buffer + fd->Public.Position % SECTOR_SIZE, u8DataOut, take);
        u8DataOut += take;
        fd->Public.Position += take;
        byteCount -= take;

        Serial::WriteNumber(COM1, take);

        // printf("leftInBuffer=%lu take=%lu\r\n", leftInBuffer, take);
        // See if we need to read more data
        if (leftInBuffer == take)
        {
            // Special handling for root directory
            if (fd->Public.Handle == ROOT_DIRECTORY_HANDLE)
            {
                ++fd->CurrentCluster;

                // read next sector
                if (!m_port->Read(fd->CurrentCluster, 1, fd->Buffer))
                {
                    ShowError(COM1, "FAT: read error!\r\n");
                    break;
                }
            }
            else
            {
                // calculate next cluster & sector to read
                if (++fd->CurrentSectorInCluster >= BS->SectorsPerCluster)
                {
                    fd->CurrentSectorInCluster = 0;
                    fd->CurrentCluster = NextCluster(fd->CurrentCluster);
                }

                if (fd->CurrentCluster >= 0xFFF8)
                {
                    // Mark end of file
                    fd->Public.Size = fd->Public.Position;
                    break;
                }

                // read next sector
                if (!m_port->Read(ClusterToLba(fd->CurrentCluster) + fd->CurrentSectorInCluster, 1, fd->Buffer))
                {
                    ShowError(COM1, "FAT: read error!\r\n");
                    break;
                }
            }
        }
    }

    return u8DataOut - (uint8_t*)dataOut;
}

bool FAT::Filesystem::ReadEntry(FAT::File* file, FAT::DirectoryEntry* dirEntry)
{
    return Read(file, sizeof(FAT::DirectoryEntry), dirEntry) == sizeof(FAT::DirectoryEntry);
}

void FAT::Filesystem::GetShortName(const char* name, char shortName[12])
{
    // convert from name to fat name
    memset(shortName, ' ', 12);
    shortName[11] = '\0';

    const char* ext = strchr(name, '.');
    if (ext == NULL)
        ext = name + 11;

    for (int i = 0; i < 8 && name[i] && name + i < ext; i++)
        shortName[i] = toupper(name[i]);

    if (ext != name + 11)
    {
        for (int i = 0; i < 3 && ext[i + 1]; i++)
            shortName[i + 8] = toupper(ext[i + 1]);
    }
}

void FAT::Filesystem::Close(File* file)
{
    if (file->Handle == ROOT_DIRECTORY_HANDLE)
    {
        file->Position = 0;
        RootDirectory.CurrentCluster = RootDirectory.FirstCluster;
    }
    else
    {
        OpenedFiles[file->Handle].Opened = false;
    }
}

bool FAT::Filesystem::FindFile(File* file, const char* name, DirectoryEntry* entryOut)
{
    char shortName[12];
    //char longName[256];
    DirectoryEntry entry;

    GetShortName(name, shortName);

    while (ReadEntry(file, &entry))
    {
        if (memcmp((void*)shortName, (void*)entry.Name, 11) == 0)
        {
            Serial::WriteString(COM1, (char*)entry.Name);

            *entryOut = entry;
            return true;
        }
    }

    return false;
}

FAT::File* FAT::Filesystem::Open(const char* path)
{
    char name[MAX_PATH_SIZE];

    // ignore leading slash
    if (path[0] == '/')
        path++;

    File* current = &RootDirectory.Public;

    while (*path) {
        // extract next file name from path
        bool isLast = false;
        const char* delim = strchr(path, '/');
        if (delim != NULL)
        {
            memcpy(name, (void*)path, delim - path);
            name[delim - path] = '\0';
            path = delim + 1;
        }
        else
        {
            unsigned len = strlen(path);
            memcpy(name, (void*)path, len);
            name[len + 1] = '\0';
            path += len;
            isLast = true;
        }

        Serial::WriteString(COM1, name);

        // find directory entry in current directory
        DirectoryEntry entry;
        if (FindFile(current, name, &entry))
        {
            Close(current);

            // check if directory
            if (!isLast && entry.Attributes & ATTRIBUTE_DIRECTORY == 0)
            {
                ShowError(COM1, "FAT: not a directory\r\n");
                return NULL;
            }

            // open new directory entry
            current = OpenEntry(&entry);
        }
        else
        {
            Close(current);

            ShowError(COM1, "FAT: not found\r\n");
            return NULL;
        }
    }

    return current;
}

FAT::FileData* FAT::Filesystem::GetFileData(File* file)
{
    int handle = -1;

    for (int i = 0; i < MAX_FILE_HANDLES; i++)
    {
        if (i == file->Handle) return &OpenedFiles[i];
    }

    return &RootDirectory;
}

FAT::File* FAT::Filesystem::GetRoot()
{
    return &RootDirectory.Public;
}

uint16_t FAT::Filesystem::GetSize()
{
    return ((BootSector*)m_bootSector)->SectorsPerFat;
}

char* FAT::Filesystem::GetType()
{
    return (char*)((BootSector*)m_bootSector)->EBR1216.SystemId;
}

char* FAT::Filesystem::GetVolumeLable()
{
    return (char*)((BootSector*)m_bootSector)->EBR1216.VolumeLabel;
}

uint8_t FAT::Filesystem::GetDriveNumber()
{
    return ((BootSector*)m_bootSector)->EBR1216.DriveNumber;
}
