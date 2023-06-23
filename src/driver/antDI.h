#pragma once
#include <stddef.h>
#include "../antstatus.h"
#include "../types.h"

#define _OUT_
#define _IN_
#define _INOUT_


#define __kernel
#define __INTERFACE_FUNCTON

typedef struct _DRIVER_OBJECT {
    unsigned short Size;
    unsigned long Flags;
    void* DriverStart;
    unsigned long DriverSize;
    __kernel void* DriverSection;
    __kernel void* DataBase;
    __kernel size_t DataSize;
    unsigned int Type;
    char DriverName[256];
    ANTSTATUS(*Unload)(struct _DRIVER_OBJECT* driverObject);
} DRIVER_OBJECT, *PDRIVER_OBJECT;