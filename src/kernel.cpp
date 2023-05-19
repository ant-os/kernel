#include "kernelUtil.h"
#include "RealTimeClock.h"
#include "DriveList.h"
#include "Console.h"
#include "userinput/keyboard.h"
#include "panic.h"

static uint32_t currentDrive = 0; 

void PrintMemory(uint64_t bytes){
    if (bytes >= 1099511627776)
        printf("%f TB", (double)bytes / 1099511627776);
    else if (bytes >= 1073741824)
        printf("%f GB", (double)bytes / 1073741824);
    else if (bytes >= 1048576)
        printf("%f MB", (double)bytes / 1048576);
    else if (bytes >= 1024)
        printf("%f KB", (double)bytes / 1024);
    else
        printf("%u bytes", bytes);
}

/// @brief UNSAFE!
/// @param raw 
/// @param index 
/// @return  String
void extract_arg(char* raw, uint64_t index, uint64_t maxOffset){
    uint64_t offset = 0;

    if (index == 0) return;
    
    for (; index > 0 && offset < maxOffset; --index)
    {
        offset = (uint64_t)raw + offset + (strlen((const char*)((uint64_t)raw + offset)) + 1);
    }

    Serial::WriteNumber(COM1, offset);
    
}


void CountDown(uint64_t seconds, char* text, uint64_t delay){

    for (; seconds != 0; --seconds)
    {
        GlobalRenderer->Clear();
        GlobalRenderer->CursorPosition = {0,0};

        printf("%s in %u Seconds....", text, seconds);
        PIT::Sleepd((double)delay);
    }
}

struct CommandListEntry
{
    const char* name;
    const char* describtion;
    const char* arguments;
};

/// A List of Commands!
static CommandListEntry CommandList[]{
    {"VER", "Displays the current AntOS Version.", "%s"},
    {"CLS", "Clear the Screen.", "%s"},
    {"EXIT", "Exit the Shell.", "%s"},
    {"DATE", "Displays the current Date.", "%s"},
    {"TIME", "Displays the current Time.", "%s"},
    {"HELP", "Show a list of Commands.", "%s"},
    {"MEM", "Display Memory Info.", "%s /HELP;..."},
    {"DRIVE", "Drive List Tool", "%s list|switch|remove|count"}

    /* {"CRASH", "Crash the Computer.", ""} */
};
/// Amount of Command Entry inside of the Command List!
static size_t CommandCount = sizeof(CommandList) / sizeof(CommandListEntry);
static size_t CommandMaxSize = 0;
constexpr size_t DefaultCommandPadding = 4;

extern "C" void _start(BootInfo * bootInfo)
{

    KernelInfo* kernelInfo = InitializeKernel(bootInfo);
    PageTableManager* pageTableManager = kernelInfo->pageTableManager;

    InitializeDriveList((uint32_t)32);

    AddDrive(Drive(-1, "System"));

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


    
    // Calculate the Maximum Command Name Size to later calculate the Padding for each Command!
    for (size_t i = 0; i < CommandCount; i++)
    {
        if (CommandMaxSize < strlen(CommandList[i].name)) CommandMaxSize = strlen(CommandList[i].name);
    }

    printf("Type `HELP` for a list of Available Commands!\n");
    
    // Reset Cursor!
    printf("\n\xE0");


    GlobalRenderer->SetColor(0xC0C0C0);
    
    PIT::Sleep(683);

    // {DriveLetter}:/>{Command} ==> Run(Command) ==> OutputOf(Command)

    bool isRunning = true;

    while (isRunning)
    {
        char drive_letter = GetDrive(currentDrive)->DriveLetter();

        // e.g. A:/>
        if (drive_letter != '?')
            printf("\xE0%c:/>", GetDrive(currentDrive)->DriveLetter());
        else printf("\n\xE0%s:/>", "<the current drive is no longer valid>");


        // We need to clear the Input Buffer before we read an new command!
        ClearInputBuffer();

        // Activate writing to the Input Buffer!
        InputBufferPosition = INPUT_ACTIVE_POS;
        
        // We wait for the Input Buffer to be ready (when enter is pressed or when InputBufferPosition == INPUT_INACTIVE_POS)
        while (InputBufferPosition != -1);


        // We only want the command!
        /// TODO: Arguments and other things like the PIPE( | ) symbole.
        
        if (strlen(InputBuffer) == 0) continue; 

        int amount = 0;

        for (size_t i = 0; i < INPUT_BUFFER_SIZE; i++)
        {
            if (InputBuffer[i] == ' ') {
                InputBuffer[i] = '\0';
                ++amount;
            }
        }
        
        char* args[5];
        int count = 0;

        for (char* arg = InputBuffer;count < 5; arg+=(strlen(arg)+1))
        {
            args[count] = arg;

            ++count;
        }

        string command = string(args[0]);
    
        command.to_upper();

        
        // If the entered command is `ver` we display the AntOS version plus the compile date & time!
        if (command == "VER"){
            printf("AntOS %s [ %s ]\n", __GIT_VERSION__, __COMPILE_DATETIME__);
        }
        // If the entered command is `help` we display a list of available commands.
        else if (command == "HELP") {
            for (size_t i = 0; i < CommandCount; i++)
            {
                size_t CommandPadding = (CommandMaxSize - strlen(CommandList[i].name));

                printf("\xE0%s", CommandList[i].name);
                for (size_t i = 0; i < CommandPadding + DefaultCommandPadding; i++)
                {
                    printf(" ");
                }
                printf("%s\n", CommandList[i].describtion);

            }
        }
        // If the entered command is `exit` we terminate the shell.
        else if (command == "EXIT") {
            ShowInfo(COM1, "The SHELL is being terminated by USER!");
            isRunning = false;
        }
        // If the entered command is `time` we display the current time.
        else if (command == "TIME") {
            RTC::ClockStatus clock = RTC::Read();

            printf("Current Time: %u:%u:%u\n", (uint64_t)clock.hour, (uint64_t)clock.minute, (uint64_t)clock.second);
        }     
        // If the entered command is `mem` we display total & free & reserved & used.
        else if (command == "MEM") {
            RTC::ClockStatus clock = RTC::Read();

            if (strlen(args[1]) == 0) {

            PrintMemory(GetMemorySize(bootInfo->mMap, bootInfo->mMapSize, bootInfo->mMapDescSize));
            printf(" Total, ");
            PrintMemory(GlobalAllocator.GetFreeRAM());
            printf(" Free, ");
            PrintMemory(GlobalAllocator.GetUsedRAM());
            printf(" Used, ");
            PrintMemory(GlobalAllocator.GetReservedRAM());
            printf(" Reserved Memory");
            
            }else{
                string type = string(args[1]);
                type.to_upper();

                if (type == "/HELP" || type == "/H") printf("Sintax: %s (free|reserved|used|total).", command.as_ptr());
                else if(type == "FREE") PrintMemory(GlobalAllocator.GetFreeRAM());
                else if (type == "USED") PrintMemory(GlobalAllocator.GetUsedRAM());
                else if (type == "RESERVED") PrintMemory(GlobalAllocator.GetReservedRAM());
                else if (type == "TOTAL") PrintMemory(GetMemorySize(bootInfo->mMap, bootInfo->mMapSize, bootInfo->mMapDescSize));
                else {
                    type.to_lower();
                    printf("Expected free|reserved|used|total, found \"%s\"", type.as_ptr());
                }
            }

            printf("\n");

        }
        // If the entered command is `date` we display the current date.
        else if (command == "DATE") {
            RTC::ClockStatus clock = RTC::Read();

            printf("Current Date: %u/%u/%u\n", (uint64_t)clock.month, (uint64_t)clock.day, clock.year);
        }
        // If the entered command is `date` we display the current date.
        else if (command == "CRASH") {
            CountDown(10, "Crahing", 1);
            return;
        }
        // If the entered command is `cls` we clear the screen!
        else if (command == "CLS") {
            Console::WriteFormat("\xEC\xEE");
        }
        // If the entered command is `drive` we run the drive list tool!
        else if (command == "DRIVE") {
            if (strlen(args[1]) != 0){
                string action = string(args[1]);

                if (action == "list"){
                    for (size_t driveIdx = 0; driveIdx < DriveCount; driveIdx++)
                    {
                        printf("%c(%d): %s\n", GetDrive(driveIdx)->DriveLetter(), (int64_t)GetDrive(driveIdx)->DriveID(), GetDrive(driveIdx)->Label());
                    }
                }
                else if (action == "switch") {
                    if (strlen(args[2]) != 0){

                        char letter = args[2][0];
                        bool sucess = false;

                        for (size_t driveIdx = 0; driveIdx < DriveCount; driveIdx++)
                        {
                            if (GetDrive(driveIdx)->DriveLetter() == letter){
                                currentDrive = driveIdx;
                                sucess = true;
                                printf("Sucessfuly switched to Drive %d with Lable %s!\n", GetDrive(driveIdx)->DriveID(), GetDrive(driveIdx)->Label());
                                break;
                            }
                        }

                        if (!sucess) printf("Failed to switch to Drive!\nNot Found!\n");
                    } else printf("Syntax %s %s <DriveLetter>", command.as_ptr(), action.as_ptr());
                }
                else if (action == "remove"){
                    if (strlen(args[2]) != 0) {

                        char letter = args[2][0];
                        bool sucess = false;

                        for (size_t driveIdx = 0; driveIdx < DriveCount; driveIdx++)
                        {
                            if (GetDrive(driveIdx)->DriveLetter() == letter) {
                                sucess = true;
                                if (!RemoveDrive(driveIdx)) printf("Failed to remove Drive!\nRemoveDrive() returned an Error\n");
                                else printf("Sucessfuly removed Drive %d with Letter %c!\n", driveIdx,  letter);
                                break;
                            }
                        }

                       if (!sucess) printf("Failed to remove Drive!\nNot Found!\n");
                    }
                    else printf("Syntax %s %s <DriveLetter>", command.as_ptr(), action.as_ptr());
                }
                else if (action == "count"){
                    printf("Drive Count: %u", DriveCount);
                }
                else printf("Expected list|switch|remove|count, found \"%s\"\n", action);
            } else printf("Syntax: %s <Action>", command);
        }

        /// TODO: If there's a Executable file in the PATH or current directory(./ prefix) the run that Executable.  

        // If the command cannot be found display an error message!
        else{
            printf("The command \"%s\" is invalid!\n", command.as_ptr());
        }

    }

    GlobalRenderer->SetColor(0xFFFFFF);

    if (DriveList[currentDrive].DriveLetter() == '?'){
        Panic("The current Drive is invalid", {0});
    }
    
    // We don't have the ability of a shutdown yet, so we just halt the CPU!
    printf("\n\nHalting System...!\n");


    /// TODO: Here we sould request a System Shutdown!
    halt();
}