#pragma once

#include <stdint.h>
#include "efiMemory.h"
#include <stddef.h>

uint64_t GetMemorySize(EFI_MEMORY_DESCRIPTOR* mMap, uint64_t mMapEntries, uint64_t mMapDescSize);
void memset(void* start, uint8_t value, uint64_t num);
void memcpy(void* source, void* dest, size_t len);

/* Set's a region of memory to ZERO. */
#define ZeroMemory(base, num) memset((void*)base, 0, num)

/* Cobine the LOW an the HIGHT part of an address. */
#define COMBINE(low, hight) ((uint64_t)low + ((uint64_t)hight << 32))