/*
 * UART initialization & communication functions.
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

#include <stdint.h>
#include <UART.h>

// HACK TILL INTERRUPTS WORK.

/*
 * Transmit a byte via UART0.
 *    uint8_t Byte -> Byte to send.
 */
void UARTTransmit(uint8_t Byte)
{
    // Keep trying.
    while(1)
    {
        // If transmit FIFO isn't full.
        if(!(MMIORegRead(UART0_FR) & (1 << 5)))
        {
            // Break.
            break;
        }
    }

    // So transmit FIFO isn't full, transmit the byte.
    MMIORegWrite(UART0_DR, (uint32_t)Byte);
}

/*
 * Receive a byte via UART0.
 *
 * Returns:
 *    uint8_t -> Byte received.
 */
uint8_t UARTReceive()
{
    // Keep trying.
    while(1)
    {
        // If receive FIFO isn't empty, break.
        if(!(MMIORegRead(UART0_FR) & (1 << 4)))
        {
            // Break.
            break;
        }
    }

    // So receive FIFO isn't empty, receive a byte.
    return (uint8_t)(MMIORegRead(UART0_DR));
}