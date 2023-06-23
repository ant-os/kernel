#pragma once

#include <stdint.h>
#include "efiMemory.h"
#include <stddef.h>

uint64_t GetMemorySize(EFI_MEMORY_DESCRIPTOR* mMap, uint64_t mMapEntries, uint64_t mMapDescSize);

/* Set's a region of memory to ZERO. */
#define ZeroMemory(base, num) memset((void*)base, 0, num)

/* Cobine the LOW an the HIGH part of an address. */
#define COMBINE(low, high) ((uint64_t)low + ((uint64_t)high << 32))