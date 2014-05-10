#ifndef _BCM2835_GPIO_H
#define _BCM2835_GPIO_H

// The GPIO registers base address.
#define GPIO_BASE    0x20200000

// The offsets for reach register.

// Controls actuation of pull up/down to ALL GPIO pins.
#define GPPUD        (GPIO_BASE + 0x94)

// Controls actuation of pull up/down for specific GPIO pin.
#define GPPUDCLK0    (GPIO_BASE + 0x98)

#endif /* _BCM2835_GPIO_H */
