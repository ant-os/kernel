#ifndef _INC_KERNEL_
#define _INC_KERNEL_ 1


#include "Framebuffer.h"
#include "simpleFonts.h"
#include "math.h"
#include "efiMemory.h"
#include "acpi.h"

typedef struct boot_info
{
    Framebuffer* framebuffer;
    PSF1_FONT* BootFont;
    EFI_MEMORY_DESCRIPTOR* mMap;
	uint64_t mMapSize;
	uint64_t mMapDescSize;
    ACPI::RSDP2* rsdp;
} BootInfo;


#endif