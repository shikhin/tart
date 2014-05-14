#include <platform.h>
#include <bcm2835/interrupts.h>
#include <bcm2835/timer.h>

void platform_init()
{
    platform_interrupts_init();
    platform_timer_init();
}
