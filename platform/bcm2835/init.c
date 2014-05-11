#include <platform.h>
#include <interrupts.h>
#include <timer.h>

void platform_init()
{
    platform_interrupts_init();
    platform_timer_init();
}
