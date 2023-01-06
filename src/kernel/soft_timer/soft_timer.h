#ifndef __SOFT_TIMER_H__
#define __SOFT_TIMER_H__

#include "inc/types.h"

struct soft_timer {
    void (*func)(void *arg);
    void *arg;
    uint64_t next_tick;
};


#endif /* __SOFT_TIMER_H__ */