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
    for(;;) {
        timer_list_t *head = list_get_head_type(&timer_list_rollover, timer_list_t, node);
        if (!head || (head->scheduled_expiry < now))
            break;

        list_delete(&head->node);
        timer_enqueue(head, now);
    }

    for(;;) {
        timer_list_t *head = list_get_head_type(&timer_list, timer_list_t, node);
        if (!head || (head->scheduled_expiry > now))
            break;

        list_delete(&head->node);
        head->periods_expired++;

        // If null, no callback.
        if(head->callback)
            head->callback(head, now, head->argument);

        // Is periodic?
        if (head->period) {
            head->scheduled_expiry = now + head->period;
            timer_enqueue(head, now);
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
