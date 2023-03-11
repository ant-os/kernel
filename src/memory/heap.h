#pragma once
#include <stdint.h>
#include <stddef.h>
#include "../paging/PageTableManager.h"
#include "../paging/PageFrameAllocator.h"
#include "../memory.h"

struct HeapSegHdr {
    size_t length;
    HeapSegHdr* next;
    HeapSegHdr* last;
    bool free;
    void CombineForward();
    void CombineBackward();
    HeapSegHdr* Split(size_t splitLength);
};


void InitializeHeap(void* heapAddress, size_t heapLenght);

void* malloc(size_t size);
void* realloc(void* address, size_t oldSize, size_t size);
void free(void* address);

void ExpandHeap(size_t lenght);