#ifndef _TIMER_H
#define _TIMER_H

#include <stdint.h>
#include <list.h>

#define TIMER_FREQ             1000

struct timer_list;

typedef uint64_t timer_ticks_t;
typedef void (*timer_callback_t)(struct timer_list *, timer_ticks_t ticks, void *argument);

typedef struct timer_list
{
    list_node_t node;

    timer_ticks_t scheduled_expiry;
    timer_ticks_t period;

    uint64_t periods_expired;

    timer_callback_t callback;
    void *argument;
} timer_list_t;

/*
 * The timer tick function, gets called on timer IRQ.
 *     timer_ticks_t now -> current ticks.
 */
void timer_tick(timer_ticks_t now);

/*
 * Setup a timer.
 *     timer_list_t *timer -> empty time struct.
 *     timer_ticks_t expires -> ticks (ms) before it expires.
 *     timer_ticks_t period -> period, if any.
 *     timer_callback_t callback -> the callback.
 *     void *argument -> argument for the callback.
 */
void timer_setup(timer_list_t *timer, timer_ticks_t expires, timer_ticks_t period, 
                 timer_callback_t callback, void *argument);

/*
 * Delete the timer.
 *     timer_list_t *timer.
 */
void timer_delete(timer_list_t *timer);

/*
 * Check if a timer is pending or not.
 *     const timer_list_t *timer.
 */
bool timer_pending(const timer_list_t *timer);

#endif /* _TIMER_H */
