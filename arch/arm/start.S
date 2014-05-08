/*
 * ARM entry point, and initialization code.
 */

#include <asm.h>
#include <arm.h>

#if ARMV < 6
    #error ARM versions below ARMv6 not supported yet.
#endif

#define STACK_SIZE     0x1000

// Text section.
.section ".text.boot"

/*
 * Entry point for the kernel.
 *     r15 -> entry point.
 *     r0  -> 0x00000000.
 *     r1  -> machine type number.
 *     r2  -> start address of ATAGS.
 */
GLOBAL(_start)
FUNCTION(_start)

# Since these can only branch to 32MiB from instruction, put the address
# of the handlers nearby, and copy them too.
.vectors:
    # Execution starts here too, so we set this to "Start."
    ldr pc, reset
    ldr pc, ud
    ldr pc, swi
    ldr pc, prefetch_abort
    ldr pc, data_abort
    ldr pc, unused
    ldr pc, irq
    ldr pc, fiq

reset:              .word start
ud:                 .word exception_ud
swi:                .word exception_swi
prefetch_abort:     .word exception_prefetch_abort
data_abort:         .word exception_data_abort
unused:             .word exception_unused
irq:                .word exception_irq
fiq:                .word exception_fiq

start:
    // Move vectors to 0x00000000.
    mov r3, r0
    ldr r4, =.vectors
    
    // 64 bytes.
    ldmia r4!, {r5-r12}
    stmia r3!, {r5-r12}

    ldmia r4!, {r5-r12}
    stmia r3!, {r5-r12}

    .stack_init:
#if ARMV >= 6
        // Switch to FIQ mode. 
        cpsid if, #FIQ_MODE
        ldr sp, =fiq_stack + (STACK_SIZE)

        cpsid if, #IRQ_MODE
        ldr sp, =abort_stack + (STACK_SIZE)

        cpsid if, #ABT_MODE
        ldr sp, =abort_stack + (STACK_SIZE)

        cpsid if, #UND_MODE
        ldr sp, =abort_stack + (STACK_SIZE)

        cpsid if, #SYS_MODE
        ldr sp, =abort_stack + (STACK_SIZE)

        cpsid if, #SVC_MODE
        ldr sp, =abort_stack + (STACK_SIZE)
#endif

    // Clear out bss.
    ldr r3, =bss_start
    ldr r4, =bss_end

    .bss_zero:
        // If lower than end, continue.
        cmp r3, r4
        strlt r0, [r3], #4
        blt .bss_zero

    // Jump to init().
    blx init

    // DO NOT RETURN HERE.
    b .

// The stacks in the BSS section.
.bss

// Stack for abort.
DATA(abort_stack)
    .skip STACK_SIZE

// Stack for FIQ.
DATA(fiq_stack)
    .skip STACK_SIZE
