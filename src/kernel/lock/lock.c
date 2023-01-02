#include "lock/lock.h"

void spinlock_init(struct spinlock *lock)
{
    spin_unlock(lock);
}