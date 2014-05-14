#ifndef _BCM2835_GPIO_H
#define _BCM2835_GPIO_H

#include <stdint.h>

// The GPIO registers base address.
#define GPIO_REG_BASE   0x20200000

// Controls actuation of pull up/down to ALL GPIO pins.
#define GPPUD           0x94

// Controls actuation of pull up/down for specific GPIO pin.
#define GPPUDCLK0       0x98

#define DISABLE_PULL_UP_DOWN    (0)
#define ENABLE_PULL_DOWN        (1 << 0)
#define ENABLE_PULL_UP          (1 << 1)

/*
 * Control the actuation of internal pull-up/down on GPIO pins.
 *     uint8_t pud -> the GPIO pull up/down to change to.
 *     uint32_t lines -> each bit represents a line.
 */
void gpio_pull_up_down(uint8_t pud, uint32_t lines);

#endif /* _BCM2835_GPIO_H */
