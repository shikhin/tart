#include <uart.h>

/*
 * Init kernel.
 */
void init()
{
    uart_init();

    while(1)
    {
        uart_transmit(uart_receive());
    }
}
