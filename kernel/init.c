#include <serial.h>

/*
 * Init kernel.
 */
void Init()
{
    while(1)
    {
        SerialTransmit(SerialReceive());
    }
}
