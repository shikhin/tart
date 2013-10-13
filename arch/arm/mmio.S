#include <asm.h>

// Text section.
.text

/*
 * Used to write to a MMIO register.
 *     r0 -> address where to write to.
 *     r1 -> the data to write to the MMIO register.
 */
GLOBAL(MMIORegWrite)
FUNCTION(MMIORegWrite)
    // Just do a plain write. This function ensures the compiler doesn't optimize this out,
    // and that they are perfect 32-bit writes.
    str r1, [r0]
    bx lr

/*
 * Used to read from a MMIO register.
 *     r0 -> address where to read from.
 *
 * Returns:
 *     r0 -> the data read from the MMIO register.
 */
GLOBAL(MMIORegRead)
FUNCTION(MMIORegRead)
    // Just do a plain read. This function ensures the compiler doesn't optimize this out,
    // and that they are perfect 32-bit read.
    ldr r0, [r0]
    bx lr