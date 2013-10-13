#ifndef _MMIO_H
#define _MMIO_H

/*
 * Used to write to a MMIO register.
 *     uint32_t Addr -> address where to write to.
 *     uint32_t Data -> the data to write to the MMIO register.
 */
extern void MMIORegWrite(uint32_t Addr, uint32_t Data);

/*
 * Used to read from a MMIO register.
 *     uint32_t Addr -> address where to read from.
 *
 * Returns:
 *     uint32_t      -> the data read from the MMIO register.
 */
extern uint32_t MMIORegRead(uint32_t Addr);

#endif /* _MMIO_H */