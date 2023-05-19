#include "Console.h"
#include "types.h"
#include "cstr.h"
#include <stdarg.h>


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
