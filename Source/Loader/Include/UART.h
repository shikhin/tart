/*
 * UART initialization & communication definitions.
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

#ifndef _UART_H
#define _UART_H

#include <stdint.h>

// The GPIO registers base address.
#define GPIO_BASE    0x20200000

// The offsets for reach register.

// Controls actuation of pull up/down to ALL GPIO pins.
#define GPPUD        (GPIO_BASE + 0x94)

// Controls actuation of pull up/down for specific GPIO pin.
#define GPPUDCLK0    (GPIO_BASE + 0x98)

// The base address for MMIO for UART.
#define UART0_BASE   0x20201000

// The offsets for reach register for the UART.
#define UART0_DR     (UART0_BASE + 0x00)
#define UART0_RSRECR (UART0_BASE + 0x04)
#define UART0_FR     (UART0_BASE + 0x18)
#define UART0_ILPR   (UART0_BASE + 0x20)
#define UART0_IBRD   (UART0_BASE + 0x24)
#define UART0_FBRD   (UART0_BASE + 0x28)
#define UART0_LCRH   (UART0_BASE + 0x2C)
#define UART0_CR     (UART0_BASE + 0x30)
#define UART0_IFLS   (UART0_BASE + 0x34)
#define UART0_IMSC   (UART0_BASE + 0x38)
#define UART0_RIS    (UART0_BASE + 0x3C)
#define UART0_MIS    (UART0_BASE + 0x40)
#define UART0_ICR    (UART0_BASE + 0x44)
#define UART0_DMACR  (UART0_BASE + 0x48)
#define UART0_ITCR   (UART0_BASE + 0x80)
#define UART0_ITIP   (UART0_BASE + 0x84)
#define UART0_ITOP   (UART0_BASE + 0x88)
#define UART0_TDR    (UART0_BASE + 0x8C)

/*
 * Used to write to a MMIO register.
 *     uint32_t Addr -> address where to write to.
 *     uint32_t Data -> the data to write to the MMIO register.
 */
extern void MMIORegWrite(uint32_t Addr, uint32_t Data);

/*
 * Used to read from a MMIO register.
 *     uint32_t Addr -> address where to read from.
 *
 * Returns:
 *     uint32_t      -> the data read from the MMIO register.
 */
extern uint32_t MMIORegRead(uint32_t Addr);

/*
 * Create a delay for 'n' cycles.
 *     uint32_t n -> the number of cycles to delay for.
 */
extern void Delay(uint32_t n);

/*
 * Initializes the UART, making it available for communication.
 */
void UARTInit();

/*
 * Transmit a byte via UART0.
 *    uint8_t Byte -> Byte to send.
 */
void UARTTransmit(uint8_t Byte);

#endif /* _UART_H */