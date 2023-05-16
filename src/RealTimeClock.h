#include <stdint.h>
#include <stdbool.h>
#include "IO.h"

typedef unsigned char byte;

#define CMOS_ADDRESS 0x70
#define CMOS_DATA 0x71

namespace RTC
{
    #define CURRENT_YEAR        2022                            // Change this each year!

    struct ClockStatus
    {
        byte second;
        byte minute;
        byte hour;
        byte day;
        byte month;
        uint64_t year;
    };
    

    byte ReadRegister(int);
    int IsUpdateInProcess();
    RTC::ClockStatus Read();

} // namespace RTC
