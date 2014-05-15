#include <kernel/timer.h>
#include <platform/timer.h>
#include <kernel/critical.h>
#include <list.h>

static list_node_t timer_list = EMPTY_LIST(timer_list);
static list_node_t timer_list_rollover = EMPTY_LIST(timer_list_rollover);

static void timer_enqueue(timer_list_t *timer, timer_ticks_t now)
{
    timer_list_t *iterator; list_node_t *list;

    if (timer->scheduled_expiry >= now) {
        list = &timer_list;
    } else {
        list = &timer_list_rollover;
    }

    list_foreach(list, timer_list_t, node, iterator) {
        if (iterator->scheduled_expiry > timer->scheduled_expiry) {
            list_add_before(&iterator->node, &timer->node);
            return;
        }
    }

    // Last entry.
    list_add_tail(list, &timer->node);
}

void timer_tick(timer_ticks_t now)
{
    timer_list_t *iterator, *temp_iter;
    list_foreach_safe(&timer_list_rollover, timer_list_t, node, iterator, temp_iter) {
        if (iterator->scheduled_expiry < now)
            break;

        list_delete(&iterator->node);
        timer_enqueue(iterator, now);
    }

    list_foreach_safe(&timer_list, timer_list_t, node, iterator, temp_iter) {
        if (iterator->scheduled_expiry > now)
            break;

        list_delete(&iterator->node);
        iterator->periods_expired++;

        // If null, no callback.
        if(iterator->callback)
            iterator->callback(iterator, now, iterator->argument);

        // Is periodic?
        if (iterator->period) {
            iterator->scheduled_expiry = now + iterator->period;
            timer_enqueue(iterator, now);
        }
    }
}

void timer_setup(timer_list_t *timer, timer_ticks_t expires, timer_ticks_t period, 
                 timer_callback_t callback, void *argument)
{
    enter_critical_section();
    timer_ticks_t now = timer_get_ticks();

    timer->scheduled_expiry = now + expires;
    timer->period = period;
    timer->periods_expired = 0;
    timer->callback = callback;
    timer->argument = argument;

    timer_enqueue(timer, now);
    exit_critical_section();
}

void timer_delete(timer_list_t *timer)
{
    enter_critical_section();
    
    if (list_in_list(&timer->node)) {
        list_delete(&timer->node);
    }

    exit_critical_section();
}

bool timer_pending(const timer_list_t *timer)
{
    return list_in_list(&timer->node);
}
