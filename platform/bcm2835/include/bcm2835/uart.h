#ifndef _BCM2835_UART_H
#define _BCM2835_UART_H

#include <stdint.h>
#include <gpio.h>

// The base address for MMIO for UART.
#define UART0_REG_BASE  0x20201000

// The offsets for reach register for the UART.
#define UART0_DR        0x00
#define UART0_RSRECR    0x04
#define UART0_FR        0x18
#define UART0_ILPR      0x20
#define UART0_IBRD      0x24
#define UART0_FBRD      0x28
#define UART0_LCRH      0x2C
#define UART0_CR        0x30
#define UART0_IFLS      0x34
#define UART0_IMSC      0x38
#define UART0_RIS       0x3C
#define UART0_MIS       0x40
#define UART0_ICR       0x44
#define UART0_DMACR     0x48
#define UART0_ITCR      0x80
#define UART0_ITIP      0x84
#define UART0_ITOP      0x88
#define UART0_TDR       0x8C

/*
 * Initializes the UART, making it available for communication.
 */
void uart_init(void);

/*
 * Transmit a byte via UART0.
 *    uint8_t byte.
 */
void uart_transmit(uint8_t byte);

/*
 * Receive a byte via UART0.
 *
 * Returns:
 *    uint8_t.
 */
uint8_t uart_receive(void);

#endif /* _BCM2835_UART_H */
