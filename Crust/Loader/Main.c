/*
 * The main function of the loader.
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

#include <UART.h>

// The version.
#define VERSION_MAJOR 0
#define VERSION_MINOR 1

#define LOADER_ADDR   0x400000

/*
 * The Main function for the laoder.
 */
void Main()
{
    // Initialize UART.
    UARTInit();

    // Tart loader protocol.
    uint32_t Size;

    // Check major version, followed by minor.
    if(UARTReceive() != VERSION_MAJOR ||
       UARTReceive() != VERSION_MINOR)
    {
        // Version error.
        UARTTransmit((uint8_t)'V'); UARTTransmit((uint8_t)'E');
        for(;;);
    }

    // Receive size of kernel.
    Size = UARTReceive(); 
    Size |= (UARTReceive() << 8);
    Size |= (UARTReceive() << 16);
    Size |= (UARTReceive() << 24);

    if(Size > (LOADER_ADDR - 0x8000))
    {
        // Size error.
        UARTTransmit((uint8_t)'S'); UARTTransmit((uint8_t)'E');
        for(;;);
    } 

    // Send 'OK' back.
    UARTTransmit((uint8_t)'O'); UARTTransmit((uint8_t)'K');

    // Receive the kernel.
    uint32_t *Kernel = (uint32_t*)0x8000;

    while(Size)
    {
        uint32_t Value = UARTReceive();
        Value |= (UARTReceive() << 8);
        Value |= (UARTReceive() << 16);
        Value |= (UARTReceive() << 24);

        *Kernel = Value;

        Kernel++; Size -= 4;
    }
}
