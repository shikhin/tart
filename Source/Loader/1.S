 @
 @ Start for loader.
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

@ To keep this in the first portion of the binary.
.text

@ Make Start global, and use Main from the C sources.
.globl Start

 @ Entry point for the loader.
 @     r15 -> should begin execution at 0x8000.
Start:
    @ Setup the stack.
    mov sp, #0x8000
    bl Main

@ Keep on looping.
.Hang:
    b .Hang

@ Make MMIORegWrite & MMIORegRead & Delay global.
.globl MMIORegWrite
.globl MMIORegRead
.globl Delay

 @ Used to write to a MMIO register.
 @     r0 -> address where to write to.
 @     r1 -> the data to write to the MMIO register.
MMIORegWrite:
    @ Just do a plain write. This function ensures the compiler doesn't optimize this out,
    @ and that they are perfect 32-bit writes.
    str r1, [r0]
    bx lr

 @ Used to read from a MMIO register.
 @     r0 -> address where to read from.
 @
 @ Returns:
 @     r0 -> the data read from the MMIO register.
MMIORegRead:
    @ Just do a plain read. This function ensures the compiler doesn't optimize this out,
    @ and that they are perfect 32-bit read.
    str r0, [r0]
    bx lr

 @ Create a delay for 'n' cycles.
 @     r0 -> the number of cycles to delay for.
Delay:
    @ Subtract 1 and keep looping till not 0.
    subs r0, r0, #1
    bne Delay

    bx lr
    