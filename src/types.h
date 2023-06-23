#pragma once
#include <stdint.h>


#pragma static_assert(__STDC_HOSTED__ == 0, "The AntOS Kernel cannot be linked with a Standard Libary!")
#if __STDC_HOSTED__ == 1
#error "The AntOS Kernel cannot be linked with a Standard Libary!"
#endif

typedef uint8_t BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef uint64_t QWORD;

typedef const void* const_ptr;
typedef int HANDLE;


#define __STD_TYPE typedef
#define __STD_CLASS class
#define __stdlib __cplusplus
#define __inline__ inline
#define __public_allocator public Alloc
#define __malloc(__size) malloc(__size)
#define __UNSAFE