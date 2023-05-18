#pragma once

#include <stdint.h>
#include "efiMemory.h"
#include <stddef.h>

uint64_t GetMemorySize(EFI_MEMORY_DESCRIPTOR* mMap, uint64_t mMapEntries, uint64_t mMapDescSize);
void memset(void* start, uint8_t value, uint64_t num);
void memcpy(void* source, void* dest, size_t len);
int memcmp(void* ptr1, void* ptr2, size_t num);

/* Set's a region of memory to ZERO. */
#define ZeroMemory(base, num) memset((void*)base, 0, num)

/* Cobine the LOW an the HIGH part of an address. */
#define COMBINE(low, high) ((uint64_t)low + ((uint64_t)high << 32))