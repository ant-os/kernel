#pragma once
#include <stdint.h>
#include "IO.h"
#include "cstr.h"

#define Log(title,text) \
    Serial::WriteString(COM1, NEW_LINE); \
    Serial::WriteString(COM1,title); \
    Serial::WriteString(COM1, ": "); \
    Serial::WriteString(COM1, (const char*)text)

#define LogAddress(title, addr) \
    Serial::WriteString(COM1, NEW_LINE); \
    Serial::WriteString(COM1,title); \
    Serial::WriteString(COM1, ": 0x"); \
    Serial::WriteString(COM1, (const char*)to_hstring((unsigned long) addr))

#pragma GCC diagnostic push
typedef enum SerialPort
{
    COM1 = 0x3F8,
    COM2 = 0x2F8,
    COM3 = 0x3E8,
    COM4 = 0x2E8,
    COM5 = 0x5F8,
    COM6 = 0x4F8,
    COM7 = 0x5E8,
    COM8 = 0x4E8
};
#pragma GCC diagnostic pop

namespace Serial
{

    int InitPort(SerialPort addr);
    int Received(SerialPort port);
    char Read(SerialPort port);
    int IsTransmitEmpty(SerialPort port);
    void Write(SerialPort port, char a);
    void WriteString (SerialPort port, const char* str);
    void WriteNumber(SerialPort port, uint64_t num);
    void WriteAddress(SerialPort port, unsigned long addr);

    #define ShowError(port, err) Serial::WriteString(port, ("\033[31;1;11m" \
                                                            err       \
                                                            "\033[0m"));

    #define ShowInfo(port, info) Serial::WriteString(port, ("\033[33;1;11m" \
                                                            info      \
                                                            "\033[0m"));

    #define ShowDebug(port, dbg) Serial::WriteString(port, ("\033[34;1;11m" \
                                                            dbg      \
                                                            "\033[0m"));

    #define ShowSucess(port, s) Serial::WriteString(port, ("\033[32;1;11m" \
                                                            s \
                                                        "\033[0m"));

    #define ShowMessage(port, msg) Serial::WriteString(port, ("\033[37;1;11m" \
                                                            msg      \
                                                            "\033[0m"));
} // namespace Serial

