#include "Console.h"
#include "types.h"
#include "cstr.h"
#include <stdarg.h>
#include "userinput/keyboard.h"
#include <ant/cstring.h>
#include <ant/memory.h>


void Console::Write(char c)
{
    _CONSOLE_PUTCHAR (c);
}

void Console::WriteString(const char* s)
{
    _CONSOLE_PRINT (s);
}

enum FROMAT_STATE {
    FSTATE_NORMAL = 0,
    FSTATE_FORMAT = 1
};

void Console::WriteFormat(const char* fmt, ...)
{
    va_list args;

    va_start(args, fmt);

    FROMAT_STATE state = FSTATE_NORMAL;

    while (*fmt != '\0')
    {
        switch (state)
        {
        case FSTATE_NORMAL:
            switch (*fmt)
            {
            case '%':
                state = FSTATE_FORMAT;
                break;
            case '\n':
                _CONSOLE_NEXT ();
                break;
            case '\xEC':
                _CONSOLE_CLEAR ();
                break; 
            case '\xE0':
                _CONSOLE_CURSOR.X = 0;
                break;
            case '\xEE':
                _CONSOLE_CURSOR = {0,0};
                break;
            case '\b':
                _CONSOLE_BACK ();
                break;
            default:
                Write(*fmt);
                break;
            }
            break;
        
        case FSTATE_FORMAT:
            switch (*fmt)
            {
            case 'c':
                Write((char)va_arg(args, int));
                break;
            case 'f':
                WriteString(to_string(va_arg(args, double)));
                break;
            case 's':
                WriteString(va_arg(args, const char*));
                break;
            case 'd':
                WriteString(to_string(va_arg(args, int64_t)));
                break;
            case 'u':
                WriteString(to_string(va_arg(args, uint64_t)));
                break;
            case 'x':
                WriteString(to_hstring(va_arg(args, uint32_t)));
                break;
            case 'p':
                WriteString(to_hstring(va_arg(args, uint64_t)));
                break;
            
            default:
                break;
            }
            state = FSTATE_NORMAL;
            break;

        default:
            break;
        }

        ++fmt;
    }
    
    

    va_end(args);

}

/// @warning Not yet Implemented! Becuase the Keyboard dosn't support it!
/// @return 
char Console::Read()
{
    return 0;
}

size_t Console::ReadBuffer(size_t length, char** buff)
{
    size_t bytes;

    InputBufferPosition = INPUT_ACTIVE_POS;

    while (KeyboardInProcess());
    
    bytes = strlen(InputBuffer);

    if (bytes == 0) return 0;

    memcpy(InputBuffer, buff, length);


    return bytes % length;

}

void Console::BlockInput(bool _Blocked)
{
    InputBufferPosition = (_Blocked ? INPUT_BLOCKED_POS : INPUT_ACTIVE_POS);
}
