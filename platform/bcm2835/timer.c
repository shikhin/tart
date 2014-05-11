#include <timer.h>
#include <barrier.h>
#include <mmio.h>
#include <interrupts.h>
#include <compiler.h>

void timer_handler(exception_frame_t *exception_frame __UNUSED)
{
    data_memory_barrier();

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

    platform_enable_fiq(FIQ_TIMER3, timer_handler);
}
