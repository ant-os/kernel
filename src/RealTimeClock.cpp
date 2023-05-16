#include "RealTimeClock.h"

int century_register = 0x00;                                // Set by ACPI table parsing code if possible

/// @brief Reads a Register
/// @param reg The Register to read.
/// @return The Value of the Register
byte RTC::ReadRegister(int reg)
{ 
    outb(CMOS_ADDRESS, reg);
    return (byte)inb(CMOS_DATA);
}

int RTC::IsUpdateInProcess()
{
    outb(CMOS_ADDRESS, 0x0A);
    return (byte)(inb(CMOS_DATA) & 0x80);
}

RTC::ClockStatus RTC::Read()
{
    RTC::ClockStatus status = RTC::ClockStatus();

    byte century;
    byte last_second;
    byte last_minute;
    byte last_hour;
    byte last_day;
    byte last_month;
    byte last_year;
    byte last_century;
    byte registerB;

    // Note: This uses the "read registers until you get the same values twice in a row" technique
    //       to avoid getting dodgy/inconsistent values due to RTC updates

    while (IsUpdateInProcess());                // Make sure an update isn't in progress
    status.second = ReadRegister(0x00);
    status.minute = ReadRegister(0x02);
    status.hour =   ReadRegister(0x04);
    status.day =    ReadRegister(0x07);
    status.month =  ReadRegister(0x08);
    status.year =   ReadRegister(0x09);
    if (century_register != 0) {
        century = ReadRegister(century_register);
    }

    do {
        last_second = status.second;
        last_minute = status.minute;
        last_hour =   status.hour;
        last_day =    status.day;
        last_month =  status.month;
        last_year =   status.year;
        last_century = century;

        while (IsUpdateInProcess());           // Make sure an update isn't in progress
        status.second = ReadRegister(0x00);
        status.minute = ReadRegister(0x02);
        status.hour =   ReadRegister(0x04);
        status.day =    ReadRegister(0x07);
        status.month =  ReadRegister(0x08);
        status.year =   ReadRegister(0x09);
        if (century_register != 0) {
            century = ReadRegister(century_register);
        }
    } while ((last_second != status.second) || (last_minute != status.minute) || (last_hour != status.hour) ||
        (last_day != status.day) || (last_month != status.month) || (last_year != status.year) ||
        (last_century != century));

    registerB = ReadRegister(0x0B);

    // Convert BCD to binary values if necessary

    if (!(registerB & 0x04)) {
        status.second = (status.second & 0x0F) + ((status.second / 16) * 10);
        status.minute = (status.minute & 0x0F) + ((status.minute / 16) * 10);
        status.hour = ((status.hour & 0x0F) + (((status.hour & 0x70) / 16) * 10)) | (status.hour & 0x80);
        status.day = (status.day & 0x0F) + ((status.day / 16) * 10);
        status.month = (status.month & 0x0F) + ((status.month / 16) * 10);
        status.year = (status.year & 0x0F) + ((status.year / 16) * 10);
        if (century_register != 0) {
            century = (century & 0x0F) + ((century / 16) * 10);
        }
    }

    // Convert 12 hour clock to 24 hour clock if necessary

    if (!(registerB & 0x02) && (status.hour & 0x80)) {
        status.hour = ((status.hour & 0x7F) + 12) % 24;
    }

    // Calculate the full (4-digit) year

    if (century_register != 0) {
        status.year += century * 100;
    }
    else {
        status.year += (CURRENT_YEAR / 100) * 100;
        if (status.year < CURRENT_YEAR) status.year += 100;
    }

    return status;
}
