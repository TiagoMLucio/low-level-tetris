#include <stdint.h>
#include "uptime.h"

#if RASPPI == 1
#define SYSTEM_TIMER_BASE 0x20003000
#else
#define SYSTEM_TIMER_BASE 0x3F003000
#endif
#define TIMER_CLO_OFFSET  0x04
#define TIMER_CHI_OFFSET  0x08

uint64_t get_current_time(void) {
    volatile uint32_t *timer_clo = (uint32_t *)(SYSTEM_TIMER_BASE + TIMER_CLO_OFFSET);
    volatile uint32_t *timer_chi = (uint32_t *)(SYSTEM_TIMER_BASE + TIMER_CHI_OFFSET);

    uint32_t low = *timer_clo;
    uint32_t high = *timer_chi;

    return ((uint64_t)high << 32) | low;
}