#include "kernelUtil.h"
#include "RealTimeClock.h"
#include "DriveList.h"
#include "Console.h"

static uint32_t currentDrive = 0; 

extern "C" void _start(BootInfo * bootInfo)
{

    KernelInfo* kernelInfo = InitializeKernel(bootInfo);
    PageTableManager* pageTableManager = kernelInfo->pageTableManager;

    InitializeDriveList((uint32_t)32);

    AddDrive(Drive(1, "System"));

    GlobalRenderer->CursorPosition = { 0, 0 };

    printf("Kernel Initialized Sucessfully!\n");
    printf("\nAntOS %s [ %s ]\n", __GIT_VERSION__, __COMPILE_DATETIME__);
    printf("(c) 2022-2023 Joscha Egloff\n\n");

    PIT::SetDivisor(20000);

    /* Enables Componentes */
    EnableKeyboard();
    EnablePIT();

    PIT::Sleep(1343);
    printf("\xE0\n");

    printf("Label of %c is %s\n", GetDrive(currentDrive)->DriveLetter(), GetDrive(currentDrive)->Label());
    
    PIT::Sleep(983);

    printf("%c:/>", GetDrive(currentDrive)->DriveLetter());

    halt();
}