#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stddef.h>
#include <stdarg.h>
#include <limits.h>

#include "memory/heap.h"

static void* _;

typedef char* cstring;

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
size_t strlen(const char* str);
char* strchr(const char* str, int c);
int toupper(int c);
int tolower(int c);
char* strstr(const char* haystack, const char* needle);
char* strtok(char* str, const char* delimiters);
char* strpbrk(const char* str, const char* delimiters);
char* strdup(const char* str);
int strcmp(cstring a, cstring b);

struct StringSplit
{
    char** strings;
    size_t count;
};



class string{
    private:
        char* m_buffer;
        size_t m_size;
        size_t m_compacity;

    public:
        string(const char* str);
        void to_upper();
        void to_lower();
        size_t character(size_t startIdx, char c);
        StringSplit split(const char* delimiter, int* count);
        char startswith(char* s);
        

        char* as_ptr();
        size_t size();

        bool operator==(const char* other);
};
