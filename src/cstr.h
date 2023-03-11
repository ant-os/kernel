#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stddef.h>
#include <stdarg.h>
#include <limits.h>

static char* _;

#define NEW_LINE "\n"
#define STR_TERMINATE "\0";
#define CHAR_NULL '\0';

#define EXTRACT_STRING(buf, len) buf[len] = 0;

const char* to_string(uint64_t value);
const char* to_string(int64_t value);
const char* to_hstring(uint64_t value);
const char* to_hstring(uint32_t value);
const char* to_hstring(uint16_t value);
const char* to_hstring(uint8_t value);
const char* to_string(double value, uint8_t decimalPlaces);
const char* to_string(double value);
const char* format(const char* fmt, ...);
size_t strlen(char*);