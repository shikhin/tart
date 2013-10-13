#include <asm.h>
#include <arm.h>

#if ARMV < 6
    #error ARM versions below ARMv6 not supported yet.
#endif

#define STACK_SIZE     0x1000

// Text section.
.section ".text.boot"

/*
 * Basic architecture initialization (stacks and modes).
 */
.macro BASIC_ARCH_INIT

// For ARMv6 and above.
#if ARMV >= 6
    // Enable Imprecise Data Aborts, and switch to FIQ mode. 
    cpsie a, #FIQ_MODE
    ldr sp, =FIQStack + (STACK_SIZE/4)

    // Switch back to supervisor mode.
    cps #SVC_MODE
#endif

.endm

/*
 * Entry point for the kernel.
 *     r15 -> entry point.
 *     r0  -> 0x00000000.
 *     r1  -> machine type number.
 *     r2  -> start address of ATAGS.
 *
 * ARMv6:
 *     E bit in CPSR is 0 (little endian).
 *     EE (exception endian), A (alignment check) and B (big endian) bit in CP15 register 1 is 0.
 *     U (unaligned access) bit in CP15 register 1 is 1.
 */
GLOBAL(_Start)
FUNCTION(_Start)

# Since these can only branch to 32MiB from instruction, put the address
# of the handlers nearby, and copy them too.
.Vectors:
    # Execution starts here too, so we set this to "Start."
    ldr pc, ResetHandlerP
    ldr pc, UndefinedInstrHandlerP
    ldr pc, SWIHandlerP
    ldr pc, PrefetchAbortHandlerP
    ldr pc, DataAbortHandlerP
    ldr pc, UnusedP
    ldr pc, IRQHandlerP
    ldr pc, FIQHandlerP

ResetHandlerP:              .word Start
UndefinedInstrHandlerP:     .word Stub
SWIHandlerP:                .word Stub
PrefetchAbortHandlerP:      .word Stub
DataAbortHandlerP:          .word Stub
UnusedP:                    .word Stub
IRQHandlerP:                .word Stub
FIQHandlerP:                .word Stub

Start:
    // Read control register (register 1) from CP15.
    mrc p15, 0, r3, c1, c0, 0

#ifndef VECTORED_IRQ
    // If vectored IRQs are not present, enable fixed IRQ vectors.
    bic r3, #(1 << 24)
#endif

    // Enable low vectors.
    bic r3, #(1 << 15)

    // Write r3 back into CP15.
    mcr p15, 0, r3, c1, c0, 0

    // Move vectors to 0x00000000.
    mov r3, r0
    ldr r4, =.Vectors
    
    // 64 bytes.
    ldmia r4!, {r5-r12}
    stmia r3!, {r5-r12}

    ldmia r4!, {r5-r12}
    stmia r3!, {r5-r12}

    // Setup the stack (for supervisor mode).
    ldr sp, =Stack + STACK_SIZE

    BASIC_ARCH_INIT

    // Clear out bss.
    ldr r3, =BSSStart
    ldr r4, =KernelEnd

    // Clear out from r5 to r12.
    mov r5, r0
    mov r6, r0
    mov r7, r0
    mov r8, r0
    mov r9, r0
    mov r10, r0
    mov r11, r0
    mov r12, r0

.ClearBSS:
    // Zero it out.
    stmia r3!, {r5-r12}

    // If lower than end, continue.
    cmp r3, r4
    blt .ClearBSS

    // Jump to Init().
    blx Init
    
// DON'T RETURN HERE.
.Halt:
    wfe
    b .Halt

// Utility functions.

/*
 * Create a delay for 'n' cycles.
 *     r0 -> the number of cycles to delay for.
 */
GLOBAL(Delay)
FUNCTION(Delay)
    // Subtract 1 and keep looping till not 0.
    subs r0, r0, #1
    bne Delay

    bx lr

// The stacks in the BSS section.
.bss

// Stack for supervisor mode.
DATA(Stack)
    .skip STACK_SIZE

// Stack for FIQ (1/4th the standard stack size).
DATA(FIQStack)
    .skip STACK_SIZE/4