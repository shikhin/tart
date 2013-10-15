/*
 * Exceptions support.
 */

#include <asm.h>
#include <arm.h>

// Text section.
.text

#if ARMV >= 6

/*
 * Macro for exception prologue.
 *     Offset -> offset to PC, to the particular instruction to return to.
 */
.macro EXCEPTION_PROLOGUE offset, switch_mode
    # Adjust LR to offset.
    sub lr, \offset
    
    // Save SPSR and LR onto the SVC stack.
    srsdb #SVC_MODE!

    // Switch to SVC mode, interrupts disabled.
    cpsid i, #SVC_MODE

    // Save registers (r0 - r15) on stack.
    // Go down 60 bytes.
    sub sp, #64
    stmia sp, {r0 - r15}

    // First argument is pointer to the structure above.
    mov r0, sp
.endm

/*
 * Macro for exception epilogue.
 */
.macro EXCEPTION_EPILOGUE
    // Get back registers (r0 - r14), while skipping r15, from stack.
    ldmia sp, {r0 - r14}
    // Get back up 64 bytes.
    add sp, #64

    // Return from exception (via the SPSR and LR we saved before).
    rfeia sp!
.endm

#else
    #error ARM versions below ARMv6 not supported yet.
#endif

/*
 * Stub (for all exceptions I haven't bothered with yet).
 */
GLOBAL(stub)
FUNCTION(stub)
    EXCEPTION_PROLOGUE #0, #1
    EXCEPTION_EPILOGUE