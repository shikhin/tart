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
 */
.macro EXCEPTION_PROLOGUE offset, mode, switch_mode, save_all
.if \offset
    // Adjust LR to offset.
    sub lr, #\offset
.endif

    // Save SPSR and LR onto the particular stack.
    srsdb sp!, #\mode

.if \switch_mode
    // Switch to the particular mode, interrupts disabled.
    cpsid i, #\mode
.endif

.if \save_all
    push { r0-r12, lr }
.else
    push { r0-r3, r12, lr }
.endif

    // First argument is pointer to the structure above.
    mov r0, sp
.endm

/*
 * Macro for exception epilogue.
 */
.macro EXCEPTION_EPILOGUE restore_all
.if \restore_all
    pop { r0-r12, lr }
.else
    pop { r0-r3, r12, lr }
.endif

    // Return from exception (via the SPSR and LR we saved before).
    rfeia sp!
.endm

#else
    #error ARM versions below ARMv6 not supported yet.
#endif

GLOBAL(exception_ud)
FUNCTION(exception_ud)
    EXCEPTION_PROLOGUE 4, UND_MODE, 0, 1
    EXCEPTION_EPILOGUE 1

GLOBAL(exception_swi)
FUNCTION(exception_swi)
    EXCEPTION_PROLOGUE 0, SVC_MODE, 0, 1
    EXCEPTION_EPILOGUE 1

GLOBAL(exception_prefetch_abort)
FUNCTION(exception_prefetch_abort)
    EXCEPTION_PROLOGUE 4, ABT_MODE, 0, 1
    EXCEPTION_EPILOGUE 1

GLOBAL(exception_data_abort)
FUNCTION(exception_data_abort)
    EXCEPTION_PROLOGUE 8, ABT_MODE, 0, 1
    EXCEPTION_EPILOGUE 1

GLOBAL(exception_unused)
FUNCTION(exception_unused)
    b .

GLOBAL(exception_irq)
FUNCTION(exception_irq)
    EXCEPTION_PROLOGUE 4, SVC_MODE, 1, 0
    EXCEPTION_EPILOGUE 0

GLOBAL(exception_fiq)
FUNCTION(exception_fiq)
    EXCEPTION_PROLOGUE 4, FIQ_MODE, 0, 1
    EXCEPTION_EPILOGUE 1
