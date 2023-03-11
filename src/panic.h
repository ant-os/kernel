#pragma once
#include "BasicRenderer.h"
#include <stdint.h>
#include <stddef.h>
#include "cstr.h"
#include "Serial.h"

#define PRINT_MSG(title, msg) GlobalRenderer->Print(title); GlobalRenderer->Print(": "); GlobalRenderer->Print(msg); GlobalRenderer->Next()

struct PanicData
{
    
    /*  These are the interrupted task's saved registers. */
    uint32_t edi;       /**< Saved EDI. */
    uint32_t esi;       /**< Saved ESI. */
    uint32_t ebp;       /**< Saved EBP. */
    uint32_t esp_dummy; /**< Not used. */
    uint32_t ebx;       /**< Saved EBX. */
    uint32_t edx;       /**< Saved EDX. */
    uint32_t ecx;       /**< Saved ECX. */
    uint32_t eax;       /**< Saved EAX. */
    uint16_t gs, : 16;  /**< Saved GS segment register. */
    uint16_t fs, : 16;  /**< Saved FS segment register. */
    uint16_t es, : 16;  /**< Saved ES segment register. */
    uint16_t ds, : 16;  /**< Saved DS segment register. */

    /* 
       Sometimes pushed by the CPU,
       otherwise for consistency pushed as 0.
       The CPU puts it just under `eip', but we move it here. */
    uint32_t error_code; /**< Error code. */

    /* This frame pointer eases interpretation of backtraces. */
    void *frame_pointer; /**< Saved EBP (frame pointer). */

    /* Pushed by the CPU.
       These are the interrupted task's saved registers. */
    void (*eip)(void); /**< Next instruction to execute. */
    uint16_t cs, : 16; /**< Code segment for eip. */
    uint32_t eflags;   /**< Saved CPU flags. */
    void *esp;         /**< Saved stack pointer. */
    uint16_t ss, : 16; /**< Data segment for esp. */
};

void Panic(const char *panicMessage, PanicData* panicData);