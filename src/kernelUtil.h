#pragma once

#include "kernel.h"
#include "BasicRenderer.h"
#include "efiMemory.h"
#include "cstr.h"
#include "memory.h"
#include "Bitmap.h"
#include "paging/PageFrameAllocator.h"
#include "paging/PageMapIndexer.h"
#include "paging/PageTableManager.h"
#include "interrupts/interrupts.h"
#include "Serial.h"
#include "userinput/mouse.h"
#include "acpi.h"
#include "pci.h"
#include "memory/heap.h"

extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;

struct KernelInfo
{
    PageTableManager* pageTableManager;
};

KernelInfo* InitializeKernel(BootInfo* bootInfo);
void EnableKeyboard();
void DisableKeyboard();
void EnableMouse();
void DisableMouse();