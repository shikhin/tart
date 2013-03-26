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

#include <Serial.h>
#include <MMIO.h>

// HACK: TILL INTERRUPTS WORK.

/*
 * Initializes the UART, making it available for communication.
 */
/*
void SerialInit()
{
    // Disable UART0.
    MMIORegWrite(UART0_CR, 0x00000000);

    // Setup the GPIO pin 14 && 15.
    
    // Disable pull up/down for all GPIO pins & delay for 150 cycles.
    MMIORegWrite(GPPUD, 0x00000000);
    Delay(150);

    // Disable pull up/down for pin 14,15 & delay for 150 cycles.
    MMIORegWrite(GPPUDCLK0, (1 << 14) | (1 << 15));
    Delay(150);

    // Write 0 to GPPUDCLK0 to make it take effect.
    MMIORegWrite(GPPUDCLK0, 0x00000000);
    
    // Clear pending interrupts.
    MMIORegWrite(UART0_ICR, 0x7FF);

    // Set integer & fractional part of baud rate.
    // Divider = UART_CLOCK/(16 * Baud)
    // Fraction part register = (Fractional part * 64) + 0.5
    // UART_CLOCK = 3000000; Baud = 115200.

    // Divider = 3000000/(16 * 115200) = 1.627 = ~1.
    // Fractional part register = (.627 * 64) + 0.5 = 40.6 = ~40.
    MMIORegWrite(UART0_IBRD, 1);
    MMIORegWrite(UART0_FBRD, 40);

    // Enable FIFO & 8 bit data transmissio (1 stop bit, no parity).
    MMIORegWrite(UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));

    // Mask all interrupts.
    MMIORegWrite(UART0_IMSC, (1 << 1) | (1 << 4) | (1 << 5) |
                             (1 << 6) | (1 << 7) | (1 << 8) |
                             (1 << 9) | (1 << 10));

    // Enable UART0, receive & transfer part of UART.
    MMIORegWrite(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));
}
*/

/*
 * Transmit a byte via UART0.
 *    uint8_t Byte -> Byte to send.
 */
void SerialTransmit(uint8_t Byte)
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
uint8_t SerialReceive()
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