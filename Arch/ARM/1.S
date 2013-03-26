 @
 @ Start for ARM.
 @
 @ Copyright (c) 2013, Shikhin Sethi
 @
 @ Permission is hereby granted, free of charge, to any person obtaining a
 @ copy of this software and associated documentation files (the "Software"),
 @ to deal in the Software without restriction, including without limitation 
 @ the rights to use, copy, modify, merge, publish, distribute, sublicense,
 @ and/or sell copies of the Software, and to permit persons to whom the Software
 @ is furnished to do so, subject to the following conditions:
 @
 @     * The above copyright notice and this permission notice shall be included in
 @       all copies or substantial portions of the Software.
 @
 @ THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 @ IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 @ FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 @ AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 @ WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
 @ OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 @

@ Text section.
.text

 @ Entry point for the kernel.
 @     r15 -> should begin execution at 0x8000.
.globl Start
Start:
    @ Setup the stack.
    mov sp, #0x8000

    @ TODO: Clear out bss.
    ldr r0, =Main
    bx r0

@ DON'T RETURN HERE.

@ Utility functions.

 @ Create a delay for 'n' cycles.
 @     r0 -> the number of cycles to delay for.
.globl Delay
Delay:
    @ Subtract 1 and keep looping till not 0.
    subs r0, r0, #1
    bne Delay

    bx lr
