/*
 * UART initialization & communication definitions.
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