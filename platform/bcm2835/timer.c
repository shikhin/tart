#include <bcm2835/timer.h>
#include <bcm2835/interrupts.h>
#include <mmio.h>
#include <barrier.h>
#include <compiler.h>
#include <kernel/critical.h>

static volatile timer_ticks_t timer_ticks;

timer_ticks_t timer_get_ticks()
{
    enter_critical_section();
    timer_ticks_t ticks = timer_ticks;
    exit_critical_section();

    return ticks;
}

void platform_timer_handler(irq_frame_t *irq_frame_t __UNUSED)
{
    data_memory_barrier();

    timer_ticks++;

    // Ack the IRQ.
    mmio_reg_write(SYS_TIMER_REG_BASE + SYS_TIMER_CS, (1 << 3));

    // Feed the new offset.
    uint32_t c3 = mmio_reg_read(SYS_TIMER_REG_BASE + SYS_TIMER_CLO);
    mmio_reg_write(SYS_TIMER_REG_BASE + SYS_TIMER_C3, c3 + TIMER_CLOCK/TIMER_FREQ);
    data_memory_barrier();
}

void platform_timer_init()
{
    data_memory_barrier();

    // Disable all timers.
    mmio_reg_write(ARM_TIMER_REG_BASE + ARM_TIMER_CONTROL, 0);

    data_memory_barrier();
    uint32_t c3 = mmio_reg_read(SYS_TIMER_REG_BASE + SYS_TIMER_CLO);
    mmio_reg_write(SYS_TIMER_REG_BASE + SYS_TIMER_C3, c3 + TIMER_CLOCK/TIMER_FREQ);

    platform_enable_fiq(FIQ_TIMER3, platform_timer_handler);
}
