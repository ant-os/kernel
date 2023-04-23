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

    while (true);
}