#include <kernel/timer.h>
#include <platform/timer.h>
#include <kernel/critical.h>
#include <list.h>

static list_node_t timer_list = EMPTY_LIST(timer_list);
static list_node_t timer_list_rollover = EMPTY_LIST(timer_list_rollover);

static void timer_add_to_list(timer_list_t *timer, list_node_t *list)
{
    enter_critical_section();

    timer_list_t *iterator;

    list_foreach(list, timer_list_t, node, iterator) {
        if (iterator->scheduled_expiry > timer->scheduled_expiry) {
            list_add_before(&iterator->node, &timer->node);
            return;
        }
    }

    // Last entry.
    list_add_tail(list, &timer->node);
    exit_critical_section();
}

void timer_setup(timer_list_t *timer, timer_ticks_t expires, timer_ticks_t period, 
                 timer_callback_t callback, void *argument)
{
    timer_ticks_t now = timer_get_ticks();

    timer->scheduled_expiry = now + expires;
    timer->period = period;
    timer->periods_expired = 0;
    timer->callback = callback;
    timer->argument = argument;

    if (timer->scheduled_expiry >= now) {
        timer_add_to_list(timer, &timer_list);
    } else {
        timer_add_to_list(timer, &timer_list_rollover);
    }
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
