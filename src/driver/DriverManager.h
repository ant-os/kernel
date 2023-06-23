#pragma once
#include <stdint.h>
#include <stddef.h>
#include <elf.h>
#include "../antstatus.h"
#include "../types.h"

#define MAX_DRIVER_HANDLERS 10
#define KeValidateDriverEntry(data) (data->Entry != nullptr && data->DriverObject != nullptr)

#include "antDI.h"

typedef HANDLE HDRIVER, *PHDRIVER;

/// @brief Basic Driver Data(Loaded Driver)! 
typedef struct _DRIVER_DATA
{
    ANTSTATUS(*Entry)(PDRIVER_OBJECT DriverObject, void(*Callback)(void));
    size_t Size;
    char* Name;
    PDRIVER_OBJECT DriverObject;
} DRIVER_DATA, *PDRIVER_DATA;

ANTSTATUS KeLoadDriver(void* Image, char* Name, PHDRIVER HandleOut);
ANTSTATUS KeUnloadDriver(HDRIVER hDriver);
ANTSTATUS KeQueryDriverData(HDRIVER hDriver, PDRIVER_DATA* DataOut);