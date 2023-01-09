#ifndef __SOFT_TIMER_H__
#define __SOFT_TIMER_H__

#include "inc/types.h"

struct soft_timer
{
    void (*callback)(void *arg);
    void *arg;
    uint64_t interval;
    uint64_t next_tick;
    uint16_t id;
};

#define SOFT_TIMER_ID_INVALID 0xFFFF

void soft_timer_init();
int soft_timer_cmp(void *a, void *b);
uint16_t soft_timer_create(void (*callback)(void *arg), void *arg, uint32_t interval);
void soft_timer_delete(uint16_t id);
void soft_timer_calc_next();
void soft_timer_heap_update();
void soft_timer_handler();

#endif /* __SOFT_TIMER_H__ */