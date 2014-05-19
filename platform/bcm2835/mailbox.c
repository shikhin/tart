#include <mailbox.h>
#include <mmio.h>
#include <barrier.h>
#include <compiler.h>
#include <kernel/timer.h>

static void mailbox_write_timeout(timer_list_t *timer __UNUSED, timer_ticks_t now __UNUSED,
                                 void *arg __UNUSED)
{
    // arg is data | channel.
    /* todo: panic(); */
    for(;;);
}

static void mailbox_read_timeout(timer_list_t *timer __UNUSED, timer_ticks_t now __UNUSED,
                                 void *arg __UNUSED)
{
    // arg is channel.
    /* todo: panic(); */
    for(;;);
}

void mailbox_write(uint32_t data, uint8_t channel)
{
    // Only accesses of different peripherals can get out of order,
    // so we'll do a DMB at the start of each peripheral access.

    // Interrupts that access peripherals would do a DMB at, both,
    // the beginning and the end.
    data_memory_barrier();
    
    timer_list_t timer;
    timer_setup(&timer, MAILBOX_TIMEOUT, 0, mailbox_write_timeout, 0);

    while (mmio_reg_read(MAILBOX_BASE + MAILBOX_STATUS) & (1 << 31));

    timer_delete(&timer);
    mmio_reg_write(MAILBOX_BASE + MAILBOX_WRITE, data | channel);
}

uint32_t mailbox_read(uint8_t channel)
{
    data_memory_barrier();
    uint32_t data;
    do {
        timer_list_t timer;
        timer_setup(&timer, MAILBOX_TIMEOUT, 0, mailbox_read_timeout, 0);

        while (mmio_reg_read(MAILBOX_BASE + MAILBOX_STATUS) & (1 << 30));

        timer_delete(&timer);
    } while (((data = mmio_reg_read(MAILBOX_BASE)) & 0xF) != channel);

    return data;
}
