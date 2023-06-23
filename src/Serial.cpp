#include "Serial.h"

int Serial::InitPort(SerialPort addr)
{
    outb(addr + 1, 0x00); // Disable all interrupts
    outb(addr + 3, 0x80); // Enable DLAB (set baud rate divisor)
    outb(addr + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
    outb(addr + 1, 0x00); //                  (hi byte)
    outb(addr + 3, 0x03); // 8 bits, no parity, one stop bit
    outb(addr + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
    outb(addr + 4, 0x0B); // IRQs enabled, RTS/DSR set
    outb(addr + 4, 0x1E); // Set in loopback mode, test the serial chip
    outb(addr + 0, 0xAE); // Test serial chip (send byte 0xAE and check if serial returns same byte)

    // Check if serial is faulty (i.e: not same byte as sent)
    if (inb(addr + 0) != 0xAE)
    {
        return 1;
    }

    // If serial is not faulty set it in normal operation mode
    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
    outb(addr + 4, 0x0F);
    return 0;
}

int Serial::Received(SerialPort port)
{
    return inb(port + 5) & 1;
}

inline char Serial::ForceRead(SerialPort port)
{
    return inb(port);
}

char Serial::Read(SerialPort port)
{
    while (Received(port) == 0);

    return ForceRead(port);
}

int Serial::IsTransmitEmpty(SerialPort port)
{
    return inb(port + 5) & 0x20;
}

void Serial::Write(SerialPort port, char a)
{
    while (IsTransmitEmpty(port) == 0);

    outb(port, a);
}

void Serial::WriteString(SerialPort port, const char *str)
{
    while (*str != '\0'){
        Write(port, *str);

        str++;
    }
}

void Serial::WriteNumber(SerialPort port, uint64_t num)
{
    WriteString(port, to_string(num));
}

void Serial::WriteAddress(SerialPort port, unsigned long addr)
{
    WriteString(port, "0x");
    WriteString(port, to_hstring(addr));
}

enum FROMAT_STATE {
    FSTATE_NORMAL = 0,
    FSTATE_FORMAT = 1
};

void Serial::WriteFormat(SerialPort port, const char* fmt, ...)
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
            default:
                Write(port, *fmt);
                break;
            }
            break;

        case FSTATE_FORMAT:
            switch (*fmt)
            {
            case 'c':
                Write(port, (char)va_arg(args, int));
                break;
            case 'f':
                WriteString(port, to_string(va_arg(args, double)));
                break;
            case 's':
                WriteString(port, va_arg(args, const char*));
                break;
            case 'd':
                WriteString(port, to_string(va_arg(args, int64_t)));
                break;
            case 'u':
                WriteString(port, to_string(va_arg(args, uint64_t)));
                break;
            case 'x':
                WriteString(port, to_hstring(va_arg(args, uint32_t)));
                break;
            case 'p':
                WriteString(port, to_hstring(va_arg(args, uint64_t)));
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
