#ifndef _PLATFORM_TIMER_H
#define _PLATFORM_TIMER_H

#include <exceptions.h>
#include <kernel/timer.h>

/*
 * Get timer ticks (ms).
 *
 * Returns:
 *     timer_ticks_t.
 */
timer_ticks_t timer_get_ticks();

/*
 * Init the timer.
 */
void platform_timer_init(void);

#endif /* _PLATFORM_TIMER_H */
