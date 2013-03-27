/*
 * Start for ARM.
 *
 * Copyright (c) 2013, Shikhin Sethi
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software
 * is furnished to do so, subject to the following conditions:
 *
 *     * The above copyright notice and this permission notice shall be included in
 *       all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
 * OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <asm.h>

#if ARMV < 6
    #error ARM versions below ARMv6 not supported yet.
#endif

#define STACK_SIZE     4096

// Text section.
.section ".text.boot"

// ARM processor modes.
#define FIQ_MODE       #0b10001 
#define IRQ_MODE       #0b10010 
#define SVC_MODE       #0b10011 
#define ABT_MODE       #0b10111 
#define UND_MODE       #0b11011 
#define SYS_MODE       #0b11111

// Basic architecture initialization (stacks and modes).
.macro BASIC_ARCH_INIT

// For ARMv6 and above.
#if ARMV >= 6
    // Enable Imprecise Data Aborts, and switch to FIQ mode. 
    cpsie a, FIQ_MODE
    ldr sp, =FIQStack + (STACK_SIZE/4)

    // Switch to abort mode.
    cps ABT_MODE
    ldr sp, =AbortStack + STACK_SIZE

    // Switch back to supervisor mode.
    cps SVC_MODE
#endif

.endm

/*
 * Entry point for the kernel.
 *     r15 -> entry point.
 *     r0  -> 0x00000000.
 *     r1  -> machine type number.
 *     r2  -> start address of ATAGS.
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
    // Move vectors to 0x00000000.
    mov r3, r0
    ldr r4, =.Vectors
    
    // 64 bytes.
    ldmia r4!, {r5-r8}
    stmia r3!, {r5-r8}

    ldmia r4!, {r5-r8}
    stmia r3!, {r5-r8}
    
    ldmia r4!, {r5-r8}
    stmia r3!, {r5-r8}
    
    ldmia r4!, {r5-r8}
    stmia r3!, {r5-r8}

    // Setup the stack (for supervisor mode).
    ldr sp, =Stack + STACK_SIZE

    BASIC_ARCH_INIT

    // Clear out bss.
    ldr r3, =BSSStart
    ldr r4, =KernelEnd

    // Clear out from r5 to r8.
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
    ldr r0, =Init
    bx r0

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

// Stack for abort mode.
DATA(AbortStack)
    .skip STACK_SIZE

// Stack for FIQ (1/4th the standard stack size).
DATA(FIQStack)
    .skip STACK_SIZE/4