#ifndef _BCM2835_TIMER_H
#define _BCM2835_TIMER_H

#include <exceptions.h>
#include <kernel/timer.h>
#include <platform/timer.h>

#define ARM_TIMER_REG_BASE     0x2000B000
#define SYS_TIMER_REG_BASE     0x20003000

#define ARM_TIMER_LOAD         0x400
#define ARM_TIMER_VALUE        0x404
#define ARM_TIMER_CONTROL      0x408
#define ARM_TIMER_IRQ_ACK      0x40C
#define ARM_TIMER_RAW_IRQ      0x410
#define ARM_TIMER_MASKED_IRQ   0x414
#define ARM_TIMER_RELOAD       0x418
#define ARM_TIMER_PRE_DIVIDER  0x41C
#define ARM_TIMER_FREE_COUNTER 0x420

#define SYS_TIMER_CS           0x00
#define SYS_TIMER_CLO          0x04
#define SYS_TIMER_CHI          0x08
#define SYS_TIMER_C0           0x0C
#define SYS_TIMER_C1           0x10
#define SYS_TIMER_C2           0x14
#define SYS_TIMER_C3           0x18

#define TIMER_CLOCK            1000000

/*
 * Init the timer.
 */
void platform_timer_init(void);

#endif /* _BCM2835_TIMER_H */
