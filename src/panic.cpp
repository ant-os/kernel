#include "panic.h"

void Panic(
    const char *panicMessage,
    PanicData* panicData)
{
    GlobalRenderer->ClearC(0x00ff0000);

    GlobalRenderer->CursorPosition = {0,0};

    GlobalRenderer->Colour = 0;

    GlobalRenderer->Print("[Kernel Panic]");

    GlobalRenderer->Next();

    PRINT_MSG("Error Code", to_hstring(panicData->error_code));
    PRINT_MSG("EIP", to_hstring((unsigned long)panicData->eip));

    PRINT_MSG("EDI", to_string((uint64_t)panicData->edi));
    PRINT_MSG("ESI", to_string((uint64_t)panicData->esi));
    PRINT_MSG("EBP", to_string((uint64_t)panicData->ebp));
    PRINT_MSG("EBX", to_string((uint64_t)panicData->ebx));
    PRINT_MSG("EDX", to_string((uint64_t)panicData->edx));
    PRINT_MSG("ECX", to_string((uint64_t)panicData->ecx));
    PRINT_MSG("EAX", to_string((uint64_t)panicData->eax));

    PRINT_MSG("GS", to_string((uint64_t)panicData->gs));
    PRINT_MSG("FS", to_string((uint64_t)panicData->gs));
    PRINT_MSG("ES", to_string((uint64_t)panicData->gs));
    PRINT_MSG("DS", to_string((uint64_t)panicData->gs));

    GlobalRenderer->Print(panicMessage);
}