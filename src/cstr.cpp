#include "cstr.h"
#include <ant/cstring.h>
#include <ant/memory.h>

char uintTo_StringOutput[128];
const char* to_string(uint64_t value){
    uint8_t size;
    uint64_t sizeTest = value;
    while (sizeTest / 10 > 0){
        sizeTest /= 10;
        size++;
    }

    uint8_t index = 0;
    while(value / 10 > 0){
        uint8_t remainder = value % 10;
        value /= 10;
        uintTo_StringOutput[size - index] = remainder + '0';
        index++;
    }
    uint8_t remainder = value % 10;
    uintTo_StringOutput[size - index] = remainder + '0';
    uintTo_StringOutput[size + 1] = 0; 
    return uintTo_StringOutput;
}

char hexTo_StringOutput[128];
const char* to_hstring(uint64_t value){
    uint64_t* valPtr = &value;
    uint8_t* ptr;
    uint8_t tmp;
    uint8_t size = 8 * 2 - 1;
    for (uint8_t i = 0; i < size; i++){
        ptr = ((uint8_t*)valPtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        hexTo_StringOutput[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
        tmp = ((*ptr & 0x0F));
        hexTo_StringOutput[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
    }
    hexTo_StringOutput[size + 1] = 0;
    return hexTo_StringOutput;
}

char hexTo_StringOutput32[128];
const char* to_hstring(uint32_t value){
    uint32_t* valPtr = &value;
    uint8_t* ptr;
    uint8_t tmp;
    uint8_t size = 4 * 2 - 1;
    for (uint8_t i = 0; i < size; i++){
        ptr = ((uint8_t*)valPtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        hexTo_StringOutput32[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
        tmp = ((*ptr & 0x0F));
        hexTo_StringOutput32[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
    }
    hexTo_StringOutput32[size + 1] = 0;
    return hexTo_StringOutput32;
}

size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

char* strchr(const char* str, int c) {
    while (*str != '\0') {
        if (*str == c) {
            return (char*)str;
        }
        str++;
    }
    return NULL;
}

int toupper(int c) {
    if (c >= 'a' && c <= 'z') {
        return c - 32;
    }
    return c;
}

int tolower(int c) {
    if (c >= 'A' && c <= 'Z') {
        return c + 32;
    }
    return c;
}

int strcmp(cstring s1, cstring s2)
{
    while (*s1) {
        if (*s1 != *s2) {
            break;
        }

        ++s1;
        ++s2;
    }

    return *s1 - *s2;
}


char hexTo_StringOutput16[128];
const char* to_hstring(uint16_t value){
    uint16_t* valPtr = &value;
    uint8_t* ptr;
    uint8_t tmp;
    uint8_t size = 2 * 2 - 1;
    for (uint8_t i = 0; i < size; i++){
        ptr = ((uint8_t*)valPtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        hexTo_StringOutput16[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
        tmp = ((*ptr & 0x0F));
        hexTo_StringOutput16[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
    }
    hexTo_StringOutput16[size + 1] = 0;
    return hexTo_StringOutput16;
}

char hexTo_StringOutput8[128];
const char* to_hstring(uint8_t value){
    uint8_t* valPtr = &value;
    uint8_t* ptr;
    uint8_t tmp;
    uint8_t size = 1 * 2 - 1;
    for (uint8_t i = 0; i < size; i++){
        ptr = ((uint8_t*)valPtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        hexTo_StringOutput8[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
        tmp = ((*ptr & 0x0F));
        hexTo_StringOutput8[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
    }
    hexTo_StringOutput8[size + 1] = 0;
    return hexTo_StringOutput8;
}

char* strstr(const char* haystack, const char* needle) {
    if (!*needle) return (char*)haystack;
    char* p1 = (char*)haystack;
    while (*p1) {
        char* p1Begin = p1, * p2 = (char*)needle;
        while (*p1 && *p2 && *p1 == *p2) {
            p1++;
            p2++;
        }
        if (!*p2) return p1Begin;
        p1 = p1Begin + 1;
    }
    return NULL;
}

char* strtok(char* str, const char* delimiters) {
    return strtok(str, delimiters);
}

char* strpbrk(const char* str, const char* delimiters) {
    while (*str) {
        const char* d = delimiters;
        while (*d) {
            if (*str == *d) {
                return (char*)str;
            }
            d++;
        }
        str++;
    }

    return NULL;
}

char intTo_StringOutput[128];
const char* to_string(int64_t value){
    uint8_t isNegative = 0;

    if (value < 0){
        isNegative = 1;
        value *= -1;
        intTo_StringOutput[0] = '-';
    }

    uint8_t size;
    uint64_t sizeTest = value;
    while (sizeTest / 10 > 0){
        sizeTest /= 10;
        size++;
    }

    uint8_t index = 0;
    while(value / 10 > 0){
        uint8_t remainder = value % 10;
        value /= 10;
        intTo_StringOutput[isNegative + size - index] = remainder + '0';
        index++;
    }
    uint8_t remainder = value % 10;
    intTo_StringOutput[isNegative + size - index] = remainder + '0';
    intTo_StringOutput[isNegative + size + 1] = 0; 
    return intTo_StringOutput;
}

char doubleTo_StringOutput[128];
const char* to_string(double value, uint8_t decimalPlaces){
    if (decimalPlaces > 20) decimalPlaces = 20;

    char* intPtr = (char*)to_string((int64_t)value);
    char* doublePtr = doubleTo_StringOutput;

    if (value < 0){
        value *= -1;
    }

    while(*intPtr != 0){
        *doublePtr = *intPtr;
        intPtr++;
        doublePtr++;
    }

    *doublePtr = '.';
    doublePtr++;

    double newValue = value - (int)value;

    for (uint8_t i = 0; i < decimalPlaces; i++){
        newValue *= 10;
        *doublePtr = (int)newValue + '0';
        newValue -= (int)newValue;
        doublePtr++;
    }

    *doublePtr = 0;
    return doubleTo_StringOutput;
}

const char* to_string(double value){
    return to_string(value, 2);
}

char* strdup(const char* str) {
    size_t len = strlen(str) + 1;
    char* dup = (char*)malloc(len);
    if (dup) {
        memcpy(dup, (void*)str, len);
    }
    return dup;
}

string::string(const char* str)
{
    m_size = strlen(str);
    m_compacity = m_size + 1;

    // m_buffer = (char*)malloc(m_compacity * sizeof(char));
    m_buffer = (char*)str;
}

void string::to_upper()
{
    for (size_t i = 0; i < strlen(m_buffer); i++)
    {
        m_buffer[i] = toupper(m_buffer[i]);
    }
}

void string::to_lower()
{
    for (size_t i = 0; i < strlen(m_buffer); i++)
    {
        m_buffer[i] = tolower(m_buffer[i]);
    }
}

size_t string::character(size_t startIdx, char c)
{
    for (size_t i = startIdx; i < strlen(m_buffer); i++)
    {
        if(m_buffer[i] == c)
            return i + 1;
    }

    return 0;
}

StringSplit string::split(const char* delimiter, int* count) {
    const char* s = m_buffer;
    int delimiter_count = 0;

    // Count the number of delimiters in the string
    while (*s) {
        if (strstr(s, delimiter) == s) {
            delimiter_count++;
            s += strlen(delimiter);
        }
        else {
            s++;
        }
    }

    // Allocate memory for the array of substrings
    char** substrings = (char**)malloc((delimiter_count + 1) * sizeof(char*));

    // Split the string into substrings
    int index = 0;
    char* buffer = strdup(m_buffer); // Create a mutable copy of the string
    char* token = strtok(buffer, delimiter);
    while (token) {
        substrings[index++] = token;
        token = strtok(NULL, delimiter);
    }

    *count = delimiter_count + 1;

    StringSplit result;
    result.strings = substrings;
    result.count = *count;

    return result;
}


char string::startswith(char* s)
{
    for (size_t i = 0; i < strlen(s); i++)
    {
        if (m_buffer[i] != s[i]){
            return m_buffer[i];
        }
    }
    
    return (char)0;
}

char* string::as_ptr()
{
    return (char*)m_buffer;
}

size_t string::size()
{
    return m_size;
}

bool string::operator==(const char* other){
    return strcmp(m_buffer, (cstring)other) == 0;
}
