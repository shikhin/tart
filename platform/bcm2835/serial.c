#include <serial.h>
#include <mmio.h>

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