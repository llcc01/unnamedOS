#ifndef __TIMER_H__
#define __TIMER_H__

#include "inc/types.h"

/* interval ~= 1s */
#define TIMER_INTERVAL CLINT_TIMEBASE_FREQ

extern uint32_t _tick;

void timer_load(int interval);
void timer_init();


#endif /* __TIMER_H__ */