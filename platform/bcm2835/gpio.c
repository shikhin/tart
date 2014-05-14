#include <gpio.h>
#include <mmio.h>
#include <utility.h>

void gpio_pull_up_down(uint8_t pud, uint32_t lines)
{
    /* TODO: assert(!(pud & ~3)); */

    // Write pud for all pins.
    mmio_reg_write(GPIO_REG_BASE + GPPUD, pud);
    delay(150);

    // Enforce for pins.
    mmio_reg_write(GPIO_REG_BASE + GPPUDCLK0, lines);
    delay(150);

    // Remove the control signal.
    mmio_reg_write(GPIO_REG_BASE + GPPUD, 0);
    // Remove the clock.
    mmio_reg_write(GPIO_REG_BASE + GPPUDCLK0, 0);
}
