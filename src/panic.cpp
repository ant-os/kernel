#include "panic.h"
#include <ant/cstring.h>
#include "util.h"

unsigned long p_hex(char* str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;

std::range KernelBinary((uint64_t)&_KernelStart, (uint64_t)&_KernelEnd);

void Panic(
    const char *panicMessage,
    PanicData* panicData)
{
    GlobalRenderer->ClearC(0x00ff0000);

    GlobalRenderer->CursorPosition = {0,0};

    GlobalRenderer->Colour = 0;

    GlobalRenderer->Print("[Kernel Panic]");
    Serial::WriteString(COM1, "\n\n===== Begin Kernel Panic =====\n\n");

    GlobalRenderer->Next();

    ((char*)panicMessage)[strlen(panicMessage) - 1] = '\0';

    Serial::WriteFormat(COM1, "Error: 0x%x (%s)\n", p_hex((char*)panicMessage), panicMessage);
    PRINT_MSG("Frame Pointer", panicData->frame_pointer);
    PRINT_MSG("Error Code", to_hstring(panicData->error_code));
    PRINT_MSG("EIP", (unsigned long)panicData->eip);

    

    PRINT_MSG("EDI", panicData->edi);
    PRINT_MSG("ESI", panicData->esi);
    PRINT_MSG("EBP", panicData->ebp);
    PRINT_MSG("EBX", panicData->ebx);
    PRINT_MSG("EDX", panicData->edx);
    PRINT_MSG("ECX", panicData->ecx);
    PRINT_MSG("EAX", panicData->eax);

    PRINT_MSG("GS", panicData->gs);
    PRINT_MSG("FS", panicData->gs);
    PRINT_MSG("ES", panicData->gs);
    PRINT_MSG("DS", panicData->gs);

    Serial::WriteFormat(COM1, "* ... ");

    for (size_t i = ((uint64_t)(panicData->eip)) - 5; i < ((uint64_t)(panicData->eip)) + 5; i++)
    {
        if (i == ((uint64_t)(panicData->eip))) Serial::WriteFormat(COM1, "\033[31;1;11m!");
        Serial::WriteFormat(COM1, "%s\033[0m ", to_hstring(*((uint8_t*)i)));
    }
    
    Serial::WriteFormat(COM1, "... *\n");

    Serial::WriteFormat(COM1, "Binary: %s", KernelBinary.contains((uint64_t)(panicData->eip)) ? "<UNKNOWN>" : "<KERNEL>");

    Serial::WriteString(COM1, "\n===== End Kernel Panic =====\n\n");

    // Temporary....
    Serial::WriteFormat(COM1, "Kernel Size: %u  bytes!\n", KernelBinary.delta());

    GlobalRenderer->Print(panicMessage);
}