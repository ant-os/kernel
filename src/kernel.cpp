#include "kernelUtil.h"


extern "C" void _start(BootInfo * bootInfo)
{

    KernelInfo* kernelInfo = InitializeKernel(bootInfo);
    PageTableManager* pageTableManager = kernelInfo->pageTableManager;

    GlobalRenderer->CursorPosition = { 0, 0 };
    GlobalRenderer->Print("Kernel Initialized Sucessfully");
    GlobalRenderer->Next();
    GlobalRenderer->Print("Welcome to AntOS 1.0!");
    GlobalRenderer->Next();

    PIT::SetDivisor(20000);

    Serial::WriteString(COM1, "PIT Frequency: ");
    Serial::WriteNumber(COM1, PIT::GetFrequency());

    /* Enables Componentes */
    EnableKeyboard();
    EnablePIT();
    Sound::Speakers::Enable();

    Serial::WriteString(COM1, "\n\r~[ AntOS Logging ]~\n\r");

    Sound::Speakers::SetBeepTime(20);
    Sound::Speakers::SetFrequency(200);
    Sound::Speakers::Beep();

    Log("KERNEL", "Hello World");

    uint64_t minutes_since_boot = 0;
    uint64_t seconds_since_boot = 0;

    while (true)
    {
        PIT::Sleep(1000); // Sleep for one second!

        seconds_since_boot = (uint64_t)PIT::TimeSinceBoot;
        minutes_since_boot = (uint64_t)PIT::TimeSinceBoot / 60;

        GlobalRenderer->Clear();
        GlobalRenderer->Print("The OS is running since ");
        GlobalRenderer->Print((const char*)to_string(minutes_since_boot));
        GlobalRenderer->PutChar(':');
        GlobalRenderer->Print((const char*)to_string(seconds_since_boot - (minutes_since_boot * 60)));
        GlobalRenderer->Print(" Seconds");
        GlobalRenderer->CursorPosition = { 0, 0 };
    }


    while (true);
}