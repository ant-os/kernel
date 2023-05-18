#include "kernelUtil.h"
#include "RealTimeClock.h"
#include "DriveList.h"
#include "Console.h"
#include "userinput/keyboard.h"

static uint32_t currentDrive = 0; 

int extract_args(const cstring rawString, cstring* outArgs){
    
}

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

    /* Enables Components */
    EnableKeyboard();
    EnablePIT();
    
    GlobalRenderer->SetColor(0xE5A908);
    printf("THIS VERSION IS UNSTABLE, CONTAINS BUGS, A PLACEHOLDER SHELL, NO DISK SUPPORT, ETC. !\n\n");
    GlobalRenderer->SetColor(0xFFFFFF);

    PIT::Sleep(343);

    // Reset Cursor!

    printf("Available Commands are: \nVER    Show AntOS Version.\nCLS    Clear the Screen.\nEXIT   Exit the Shell.\nDATE   Display the current Date.\nTIME   Display the current Time.\nHELP   Display a list of Available Commands.\n\n");
    GlobalRenderer->SetColor(0xC0C0C0);
    
    PIT::Sleep(683);

    // {DriveLetter}:/>{Command} ==> Run(Command) ==> OutputOf(Command)

    bool isRunning = true;


    while (isRunning)
    {
        // e.g. A:/>
        printf("\n\xE0%c:/>", GetDrive(currentDrive)->DriveLetter());

        // We need to clear the Input Buffer before we read an new command!
        ClearInputBuffer();

        // Activate writing to the Input Buffer!
        InputBufferPosition = INPUT_ACTIVE_POS;
        
        // We wait for the Input Buffer to be ready (when enter is pressed or when InputBufferPosition == INPUT_INACTIVE_POS)
        while (InputBufferPosition != -1);


        // We only want the command!
        /// TODO: Arguments and other things like the PIPE( | ) symbole.
        for (size_t i = 0; i < strlen(InputBuffer); i++)
        {
            if (InputBuffer[i] == ' '){
                InputBuffer[i] = '\0';
            }else{
                InputBuffer[i] = (char)toupper(InputBuffer[i]);
            }
        }
        
        // If the entered command is `ver` we display the AntOS version plus the compile date & time!


        if (strcmp(InputBuffer, "VER") == 0){
            printf("AntOS %s [ %s ]\n", __GIT_VERSION__, __COMPILE_DATETIME__);
        }
        // If the entered command is `help` we display a list of available commands.
        else if (strcmp(InputBuffer, "HELP") == 0) {
            printf("Available Commands are: \nVER    Show AntOS Version.\nCLS    Clear the Screen.\nEXIT   Exit the Shell.\nDATE   Display the current Date.\nTIME   Display the current Time.\nHELP   Display this List.\n\n");
        }
        // If the entered command is `exit` we terminate the shell.
        else if (strcmp(InputBuffer, "EXIT") == 0) {
            ShowInfo(COM1, "The SHELL is being terminated by USER!");
            isRunning = false;
        }
        // If the entered command is `time` we display the current time.
        else if (strcmp(InputBuffer, "TIME") == 0) {
            RTC::ClockStatus clock = RTC::Read();

            printf("Current Time: %u:%u:%u\n", (uint64_t)clock.hour, (uint64_t)clock.minute, (uint64_t)clock.second);
        }     
        // If the entered command is `date` we display the current date.
        else if (strcmp(InputBuffer, "DATE") == 0) {
            RTC::ClockStatus clock = RTC::Read();

            printf("Current Date: %u/%u/%u\n", (uint64_t)clock.month, (uint64_t)clock.day, clock.year);
        }
        // If the entered command is `cls` we clear the screen!
        else if (strcmp(InputBuffer, "CLS") == 0) {
            Console::WriteFormat("\xEC\xEE");
        }
        // If we didn't input anything e.g. The Input Buffe ris empty. Then we do nothfing.
        else if (strlen(InputBuffer) == 0){ }

        /// TODO: If there's a Executable file in the PATH or current directory(./ prefix) the run that Executable.  

        // If the command cannot be found display an error message!
        else{
            printf("The command \"%s\" is invalid!\n", InputBuffer);
        }

    }

    GlobalRenderer->SetColor(0xFFFFFF);
    
    // We don't have the ability of a shutdown yet, so we just halt the CPU!
    printf("\n\nHalting System...!\n");


    /// TODO: Here we sould request a System Shutdown!
    halt();
}