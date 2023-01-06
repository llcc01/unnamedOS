
#include "lock/lock.h"
#include "soft_timer/soft_timer.h"

struct spinlock soft_timer_lock;

void soft_timer_init()
{
    spinlock_init(&soft_timer_lock);
}

void soft_timer_creat()
{
    spin_lock(&soft_timer_lock);



    spin_unlock(&soft_timer_lock);
}