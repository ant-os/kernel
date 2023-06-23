#include "DriverManager.h"
#include <ant/core.h>

/// TODO: Use something like std::list
static DRIVER_DATA DriverHandleList[MAX_DRIVER_HANDLERS]{0};
static int DriverHandleCount = 0;

ANTSTATUS KeLoadDriver(void* Image, char* Name, PHDRIVER HandleOut)
{
    if (HandleOut == nullptr) return STATUS_INVALID_PARAMETERS;
    if (DriverHandleCount == MAX_DRIVER_HANDLERS) return STATUS_OUT_OF_HANDLES;

    Elf64_Ehdr* ehdr = (Elf64_Ehdr*)Image;
    if (ehdr->e_ident[EI_MAG0] != ELFMAG0 ||
        ehdr->e_ident[EI_MAG1] != ELFMAG1 ||
        ehdr->e_ident[EI_MAG2] != ELFMAG2 ||
        ehdr->e_ident[EI_MAG3] != ELFMAG3)
            return STATUS_INVALID_FORMAT;

    ANTSTATUS(*DriverEntry)(PDRIVER_OBJECT DriverObject, void(*Callback)(void)) = (ANTSTATUS(*)(PDRIVER_OBJECT DriverObject, void(*Callback)(void))) (((Elf64_Addr)Image) + ehdr->e_entry);

    DRIVER_DATA data{};
    data.Entry = DriverEntry;
    data.Name = Name;
    data.Size = (size_t)ehdr->e_ehsize;

    HANDLE newDriverHandle = DriverHandleCount + 1;
    
    DriverHandleList[newDriverHandle] = data;
    *HandleOut = newDriverHandle;
    DriverHandleCount = newDriverHandle;

    return STATUS_SUCCESS;
}

ANTSTATUS KeUnloadDriver(HDRIVER hDriver)
{
    if (DriverHandleList[hDriver].Size == 0) return STATUS_INVALID_HANDLE;
    if (DriverHandleList[hDriver].DriverObject == nullptr) return STATUS_DRV_NOT_INITIALIZED;

    if (DriverHandleList[hDriver].DriverObject->Unload != nullptr){
        DriverHandleList[hDriver].DriverObject->Unload(DriverHandleList[hDriver].DriverObject);
    }

    free(DriverHandleList[hDriver].DriverObject);

    DriverHandleList[hDriver] = {0};

    return STATUS_SUCCESS;
}

ANTSTATUS KeQueryDriverData(HDRIVER hDriver, PDRIVER_DATA* DataOut)
{
    if (DataOut == nullptr) return STATUS_INVALID_PARAMETERS;
    if (DriverHandleList[hDriver].Size == 0) return STATUS_INVALID_HANDLE;

    *DataOut = &DriverHandleList[hDriver];

    return STATUS_SUCCESS;
}
