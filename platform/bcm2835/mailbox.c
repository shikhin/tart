#include <mailbox.h>
#include <mmio.h>
#include <barrier.h>

void mailbox_write(uint32_t data, uint8_t channel)
{
    // Only accesses of different peripherals can get out of order,
    // so we'll do a DMB at the start of each peripheral access.

    // Interrupts that access peripherals would do a DMB at, both,
    // the beginning and the end.
    data_memory_barrier();
    do {
        /* TODO: add timeout. */
    } while (mmio_reg_read(MAILBOX_BASE + MAILBOX_STATUS) & (1 << 31));

    mmio_reg_write(MAILBOX_BASE + MAILBOX_WRITE, data | channel);
}

uint32_t mailbox_read(uint8_t channel)
{
    data_memory_barrier();
    uint32_t data;
    do {
        do {
            /* TODO: add timeout. */
        } while (mmio_reg_read(MAILBOX_BASE + MAILBOX_STATUS) & (1 << 30));
    } while (((data = mmio_reg_read(MAILBOX_BASE)) & 0xF) != channel);

    return data;
}
