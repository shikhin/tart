/*
 * UART initialization & communication functions.
 *
 * Copyright (c) 2013, Shikhin Sethi
 * 
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 *     1. The origin of this software must not be misrepresented; you must not
 *        claim that you wrote the original software. If you use this software
 *        in a product, an acknowledgment in the product documentation would be
 *        appreciated but is not required.
 *
 *     2. Altered source versions must be plainly marked as such, and must not be
 *        misrepresented as being the original software.
 *
 *     3. This notice may not be removed or altered from any source
 *        distribution.
 */

#include <stdint.h>
#include <UART.h>

/*
 * Initializes the UART, making it available for communication.
 */
void UARTInit()
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
    //MMIORegWrite(UART0_IMSC, (1 << 1) | (1 << 4) | (1 << 5) |
    //                         (1 << 6) | (1 << 7) | (1 << 8) |
    //                         (1 << 9) | (1 << 10));

    // Enable UART0, receive & transfer part of UART.
    MMIORegWrite(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));
}

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