/*
 * Exceptions support.
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
#include <arm.h>

// Text section.
.text

#if ARMV >= 6

/*
 * Macro for exception prologue.
 *     Offset -> offset to PC, to the particular instruction to return to.
 */
.macro EXCEPTION_PROLOGUE Offset, SwitchMode
    # Adjust LR to offset.
    sub lr, \Offset
    
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

#endif

/*
 * Stub (for all exceptions I haven't bothered with yet).
 */
GLOBAL(Stub)
FUNCTION(Stub)
    EXCEPTION_PROLOGUE #0, #1
    EXCEPTION_EPILOGUE