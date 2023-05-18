#pragma once

#ifndef _RENDERER_HEADER
#define _RENDERER_HEADER "BasicRenderer.h"
#endif

#include _RENDERER_HEADER

#ifndef _CONSOLE_PRINT
#define _CONSOLE_PRINT GlobalRenderer->Print
#endif

#ifndef _CONSOLE_PUTCHAR
#define _CONSOLE_PUTCHAR GlobalRenderer->PutChar
#endif

#ifndef _CONSOLE_NEXT
#define _CONSOLE_NEXT GlobalRenderer->Next
#endif

#ifndef _CONSOLE_CLEAR
#define _CONSOLE_CLEAR GlobalRenderer->Clear
#endif

#ifndef _CONSOLE_BACK
#define _CONSOLE_BACK GlobalRenderer->ClearChar
#endif

#ifndef _CONSOLE_CURSOR
#define _CONSOLE_CURSOR GlobalRenderer->CursorPosition
#endif

namespace Console{

    void Write(char c);
    void WriteString(const char* s);
    void WriteFormat(const char* fmt, ...);

}