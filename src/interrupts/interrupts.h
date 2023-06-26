#pragma once
#include <stdint.h>
#include <stddef.h>
#include "../types.h"
#include "../antstatus.h"

#define sti() asm("sti")
#define cli() asm("cli")

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1
#define PIC_EOI 0x20

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

#define FAULT_HANDLER(f)                                                                                    \
   __attribute__((interrupt)) void f##Fault_handler(struct interrupt_frame *frame) 
#define INT_HANDLER(i) \
   __attribute__((interrupt)) void i##_handler(struct interrupt_frame *frame)

struct interrupt_frame {
   /* Pushed by intr_entry in intr-stubs.S.
       These are the interrupted task's saved registers. */
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

   /* Pushed by intrNN_stub in intr-stubs.S. */
   uint32_t vec_no; /**< Interrupt vector number. */

   /* Sometimes pushed by the CPU,
      otherwise for consistency pushed as 0 by intrNN_stub.
      The CPU puts it just under `eip', but we move it here. */
   uint32_t error_code; /**< Error code. */

   /* Pushed by intrNN_stub in intr-stubs.S.
      This frame pointer eases interpretation of backtraces. */
   void* frame_pointer; /**< Saved EBP (frame pointer). */

   /* Pushed by the CPU.
      These are the interrupted task's saved registers. */
   void (*eip)(void); /**< Next instruction to execute. */
   uint16_t cs, : 16; /**< Code segment for eip. */
   uint32_t eflags;   /**< Saved CPU flags. */
   void* esp;         /**< Saved stack pointer. */
   uint16_t ss, : 16; /**< Data segment for esp. */
};


FAULT_HANDLER(Page);
FAULT_HANDLER(Double);
FAULT_HANDLER(GP);
INT_HANDLER(DivisionError);
INT_HANDLER(Debug);
INT_HANDLER(Breakpoint);
INT_HANDLER(Overflow);
INT_HANDLER(BoundRangeExceeded);
INT_HANDLER(InvalidOpcode);
INT_HANDLER(DeviceNotAvailable);
INT_HANDLER(InvalidTSS);
INT_HANDLER(SegmentNotPresent);
FAULT_HANDLER(StackSegment);
INT_HANDLER(AlignmentCheck);
INT_HANDLER(MachineCheck);
INT_HANDLER(VirtualizationException);
INT_HANDLER(ControlProtectionException);
INT_HANDLER(HypervisorInjectionException);
INT_HANDLER(VMMCommunicationException);
INT_HANDLER(SecurityException);
FAULT_HANDLER(Triple);

INT_HANDLER(Keyboard);
INT_HANDLER(Mouse);
INT_HANDLER(PIT);
__attribute__((interrupt)) void UtilCall_handler(interrupt_frame* frame);

__attribute__((interrupt)) void _InterruptHandler(struct interrupt_frame* frame);

#define INT(number) asm("int $" #number)

void RemapPIC();
void PIC_EndMaster();
void PIC_EndSlave();
void DisablePIC();

__STD_TYPE ANTSTATUS(*InterruptCallback)(interrupt_frame* frame);

enum InterruptType{
   Exception,
   Hardware,
   Software,
   Unknown
};

/* An Interrupt Vector wiht mutible or single callback(s) and Type, etc.!*/
typedef struct _INTERRUPT_VECTOR {

   uint32_t Number;
   InterruptType Type;
   void* Descriptor;
   
   // For Mutible Callbacks! 
   size_t CallbackCount;
   InterruptCallback* Callbacks;

} INTERRUPT_VECTOR, *PINTERRUPT_VECTOR;



/* ********************************************************************************** */

void KeInvokeRawVector(PINTERRUPT_VECTOR pVector, interrupt_frame* _Frame) __UNSAFE;
void KeSetInterruptVector(uint32_t num, PINTERRUPT_VECTOR pVector) __UNSAFE;
PINTERRUPT_VECTOR KeGetInterruptVector(uint32_t num) __UNSAFE;
void KeInvokeInterrupt(uint32_t num) __UNSAFE;
ANTSTATUS KeInitializeInterrupts();