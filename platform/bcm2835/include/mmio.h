#ifndef _BCM2835_MMIO_H
#define _BCM2835_MMIO_H

#include <stdint.h>

/*
 * Writes to a MMIO register.
 *     uintptr_t ptr.
 *     uint32_t value.
 */
static inline void mmio_reg_write(uintptr_t ptr, uint32_t val)
{
    volatile uint32_t* reg = (volatile uint32_t*)ptr;
    *reg = val;
}

/*
 * Reads a MMIO register.
 *     uintptr_t ptr.
 *
 * Returns:
 *     uint32_t.
 */
static inline uint32_t mmio_reg_read(uintptr_t ptr)
{
    return *((volatile uint32_t*)ptr);
}

#endif /* _BCM2835_MMIO_H */
