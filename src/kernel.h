#ifndef _INC_KERNEL_
#define _INC_KERNEL_ 1

#ifndef GIT_VERSION__
#define GIT_VERSION "<?>"
#else
#define GIT_VERSION __GIT_VERSION__
#endif

#ifndef __FILENAME__
#define FILENAME "<?>"
#else 
#define FILENAME TEXT(__FILENAME__)
#endif

#ifndef __COMPILE_DATETIME__
#define COMPILE_DATETIME "0/0/0000 0:0:0"
#else 
#define COMPILE_DATETIME __COMPILE_DATETIME__
#endif

#define TEXT(expr) #expr 
#define halt_while(con) for(;;(con)) asm("hlt")
#define halt() for(;;) asm("hlt")


#include "Framebuffer.h"
#include "simpleFonts.h"
#include "math.h"
#include "efiMemory.h"
#include "acpi.h"

#define printf Console::WriteFormat
#define puts Console::WriteString
#define putc Console::Write


#define MULTIOR_EQ(expr, val1, val2) (expr == val1 || expr == val2)

#define FLIP(b) b = !b

#define __line #__LINE__
#define assert(expr, msg) if (!(expr)) Serial::WriteFormat(COM1, "\n\033[31;1;11mAssert on line %d in `%s` Failed(%s): %s\033[0m", __LINE__, __FILE__, #expr, msg)
#define _CompilerGetSourceArgs() __FILE__, ":", __LINE__


struct EnvironmentVariable
{
    char* name;
    char* value;
};

#define EXIT_SUCESS 0
#define EXIT_FAILURE 1

typedef struct _DRIVER_IMAGE
{
    uint64_t Status;
    uint64_t Base;
    size_t Size;
} DRIVER_IMAGE;


typedef struct boot_info
{
    Framebuffer* framebuffer;
    PSF1_FONT* BootFont;
    EFI_MEMORY_DESCRIPTOR* mMap;
	uint64_t mMapSize;
	uint64_t mMapDescSize;
    ACPI::RSDP2* rsdp;
    DRIVER_IMAGE MiniFS;
} BootInfo;


#endif