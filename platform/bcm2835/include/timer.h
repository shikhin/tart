#ifndef _BCM2835_TIMER_H
#define _BCM2835_TIMER_H

#include <exceptions.h>

#define TIMER_REG_BASE      0x2000B000

#define TIMER_LOAD          0x400
#define TIMER_VALUE         0x404
#define TIMER_CONTROL       0x408
#define TIMER_IRQ_ACK       0x40C
#define TIMER_RAW_IRQ       0x410
#define TIMER_MASKED_IRQ    0x414
#define TIMER_RELOAD        0x418
#define TIMER_PRE_DIVIDER   0x41C
#define TIMER_FREE_COUNTER  0x420

#define TIMER_CLOCK         1000000

#define TIMER_FREQ          1

/*
 * The timer handler.
 */
void timer_handler(exception_frame_t *exception_frame);

/*
 * Init the timer.
 */
void platform_timer_init(void);

#endif /* _BCM2835_TIMER_H */
