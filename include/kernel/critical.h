#ifndef _CRITICAL_H
#define _CRITICAL_H

#include <stdint.h>
#include <exceptions.h>

extern volatile int critical_section_count;

static inline void enter_critical_section()
{
    if (!critical_section_count) {
        arch_disable_interrupts();
    }

    critical_section_count++;
}

static inline void exit_critical_section()
{
    if (!(--critical_section_count)) {
        arch_enable_interrupts();
    }
}

#endif /* _CRITICAL_H */
