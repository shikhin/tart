#include <timer.h>
#include <mailbox.h>
#include <compiler.h>
#include <barrier.h>
#include <mmio.h>
#include <uart.h>
#include <interrupts.h>

void timer_handler(exception_frame_t *exception_frame __UNUSED)
{
    data_memory_barrier();

    // Ack the IRQ.
    mmio_reg_write(TIMER_REG_BASE + TIMER_IRQ_ACK, 0x00000000);
    data_memory_barrier();
}

void platform_timer_init()
{
    volatile uint32_t mailbox_buffer[] __ALIGNED(16) = {
        0x00000000 /* fill in with size */,
        0x00000000 /* process request */,
            0x00030002 /* get clock rate tag */, 8 /* buffer size */,
            4 /* buffer length, MSB = 0 = request */, CORE_CLOCK_ID /* clock id */,
            0x00000000 /* return space */,

            0x00000000 /* empty tag */
    };

    mailbox_buffer[0] = sizeof (mailbox_buffer);
    mailbox_write((uintptr_t)mailbox_buffer, MAILBOX_ARM_TO_VC);
    mailbox_read(MAILBOX_ARM_TO_VC);

    data_memory_barrier();
    /* TODO: assert(mailbox_buffer[1] == MAILBOX_SUCCESS); */
    /* TODO: assert(mailbox_buffer[4] & (1 << 31)); // response. */
    /* TODO: assert((mailbox_buffer[4] & ~(1 << 31)) == 8); */
    uint32_t core_clock = mailbox_buffer[6];

    // timer_clock = apb_clock/(pre_divider + 1)
    // pre_divider = apb_clock/timer_clock - 1
    uint32_t pre_divider = (core_clock/TIMER_CLOCK) - 1;
    /* TODO: assert(pre_divider <= 0x3FF); // 10 bits. */

    mmio_reg_write(TIMER_REG_BASE + TIMER_PRE_DIVIDER, pre_divider);
    // 23-bit counter, timer interrupt enabled, timer enabled.
    mmio_reg_write(TIMER_REG_BASE + TIMER_CONTROL, (1 << 1) |
                   (1 << 5) | (1 << 7));
    mmio_reg_write(TIMER_REG_BASE + TIMER_LOAD, TIMER_CLOCK/TIMER_FREQ);

    platform_enable_fiq(FIQ_ARM_TIMER, timer_handler);
}
