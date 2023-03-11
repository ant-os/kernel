#include "kernelUtil.h"


extern "C" void _start(BootInfo *bootInfo)
{

    KernelInfo* kernelInfo = InitializeKernel(bootInfo);
    PageTableManager* pageTableManager = kernelInfo->pageTableManager;

    GlobalRenderer->CursorPosition = {0, 0};
    GlobalRenderer->Print("Kernel Initialized Sucessfully");
    GlobalRenderer->Next();
    GlobalRenderer->Print("Welcome to AntOS 1.0!");
    GlobalRenderer->Next();

    //GlobalRenderer->DrawLine(Point{X: 0, Y: 0}, Point{X: (int)GlobalRenderer->GetWidth(), Y: (int)GlobalRenderer->GetHeight()}, GlobalRenderer->Colour);
    //GlobalRenderer->FillRect(CENTER(GlobalRenderer), 100, 100);

    Serial::WriteString(COM1, "\n\r~[ AntOS Logging ]~\n\r");

    while (true);
}