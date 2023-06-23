#pragma once

#include <stdint.h>

typedef struct _SECTION_OBJECT
{
    void* Base;
    uint64_t Size;
} SECTION_OBJECT, *PSECTION_OBJECT;