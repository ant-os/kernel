#pragma once
#define __ANTOS_HEAP 1

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
void ExpandHeap(size_t lenght);

#if __STDC_HOSTED__ == 0
#define __KERNEL_MODE__ 1
#define __inline inline

extern "C"{

void* malloc(size_t size);
void* realloc(void* address, size_t size);
void free(void* address);

}

__inline void* operator new(size_t size) noexcept  { return malloc(size); }
__inline void* operator new[](size_t size) noexcept {return malloc(size);}
__inline void operator delete(void* p) noexcept { free(p); }
__inline void operator delete[](void* p) noexcept { free(p);}
__inline void* operator new(size_t, void* ptr) noexcept {return ptr;}
__inline void operator delete(void*, void*) noexcept {/* no-op */}


#else
#error "Cannot link with Standart Libary!"
#endif