 @
 @ Start for loader.
 @
 @ Copyright (c) 2013, Shikhin Sethi
 @ 
 @ This software is provided 'as-is', without any express or implied
 @ warranty. In no event will the authors be held liable for any damages
 @ arising from the use of this software.
 @
 @ Permission is granted to anyone to use this software for any purpose,
 @ including commercial applications, and to alter it and redistribute it
 @ freely, subject to the following restrictions:
 @
 @     1. The origin of this software must not be misrepresented; you must not
 @        claim that you wrote the original software. If you use this software
 @        in a product, an acknowledgment in the product documentation would be
 @        appreciated but is not required.
 @
 @     2. Altered source versions must be plainly marked as such, and must not be
 @        misrepresented as being the original software.
 @
 @     3. This notice may not be removed or altered from any source
 @        distribution.
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
    