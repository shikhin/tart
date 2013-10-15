/*
 * Kernel init.
 */

#include <uart.h>

/*
 * Kernel init.
 */
void init()
{
    uart_init();

    while (1)
    {
        uart_transmit(uart_receive());
    }
}
