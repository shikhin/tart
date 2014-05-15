#include <platform.h>

volatile int critical_section_count = 0;

void init()
{
    platform_init();
}
