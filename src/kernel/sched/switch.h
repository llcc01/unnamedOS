#ifndef __SWITCH_H__
#define __SWITCH_H__

#include "sched.h"

/* defined in entry.S */
void switch_to(struct context *next);

#endif