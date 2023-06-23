#include "kernelUtil.h"
#include "RealTimeClock.h"
#include "DriveList.h"
#include "Console.h"
#include "userinput/keyboard.h"
#include "panic.h"
#include "CommandHandlers.h"
#include "antstatus.h"
#include "driver/DriverManager.h"
#include "Debugger.h"

#include <ant/core.h> // The Core Features of the Ant C/C++ Standard Library.
#include <ant/debug.h> // Debug Functions like dbg_print(), etc.
#include <ant/stdio.h> // Standard Input/Output Functions like printf(), etc.
#include <ant/memory.h> // Memory Management Functions like malloc(), free(), etc.
#include <ant/dynamic.h> // Dynamic Memory Types like dynamic_allocator<T>, etc.
#include <ant/string.h> // String Functions(cstring.h) and an own implementation of `std::string`.

extern "C" void _start(BootInfo * bootInfo)
{
    ANTSTATUS status;

    KernelInfo* kernelInfo = InitializeKernel(bootInfo);
    PageTableManager* pageTableManager = kernelInfo->pageTableManager;


    GlobalRenderer->CursorPosition = { 0, 0 };

    printf("Kernel Initialized Sucessfully!\n");
    printf("\nAntOS %s [ %s ]\n", __GIT_VERSION__, __COMPILE_DATETIME__);
    printf("(c) 2022-2023 Joscha Egloff\n\n");

    // PIT::SetDivisor(20000);

    /* Enables Components */
    EnableKeyboard();
    EnablePIT();
    
    GlobalRenderer->SetColor(0xE5A908);
    printf("THIS VERSION IS UNSTABLE AND A DEVELOPER TESTING BUILD!\n\n");
    GlobalRenderer->SetColor(0xFFFFFF);

    GlobalRenderer->SetColor(0xFFFFFF);
    
    // We don't have the ability of a shutdown yet, so we just halt the CPU!
    printf("\n\nHalting System...!\n");


    /// TODO: Here we sould request a System Shutdown!
    halt();
}