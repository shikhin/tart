#include <platform.h>

volatile int critical_section_count;

void init()
{
    platform_init();
}
