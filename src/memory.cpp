#include "memory.h"

/// @param mMap 
/// @param mMapEntries 
/// @param mMapDescSize 
/// @return Amount of Computer Memory in Bytes
uint64_t GetMemorySize(EFI_MEMORY_DESCRIPTOR *mMap, uint64_t mMapEntries, uint64_t mMapDescSize)
{
    static uint64_t memorySizeBytes = 0;
    if (memorySizeBytes > 0) return memorySizeBytes;

    for (int i = 0; i < mMapEntries; i++)
    {
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)mMap + (i * mMapDescSize));
        memorySizeBytes += desc->numPages * 4096;
    }

    return memorySizeBytes;
}

void memset(void* start, uint8_t value, uint64_t num){
    for (uint64_t i = 0; i < num; i++){
        *(uint8_t*)((uint64_t)start + i) = value;
    }
}

void memcpy(void* source, void* dest, size_t len)
{
    int i;
    //declare string and type casting 
    char* t = (char*)dest;
    char* s = (char*)source;
    //copying "n" bytes of source to target
    for (i = 0;i < len;i++)
        t[i] = s[i];
}

int memcmp(void* ptr1, void* ptr2, size_t num) {
    int* p1 = (int*)ptr1;
    int* p2 = (int*)ptr2;

    for (size_t i = 0; i < num; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] - p2[i];
        }
    }

    return 0;
}