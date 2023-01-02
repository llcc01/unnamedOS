#ifndef __LOCK_H__
#define __LOCK_H__

#include "inc/types.h"

struct spinlock {
    uint32_t lock;
};

/* defined in lock.S */
void spin_lock(struct spinlock *lock);
void spin_unlock(struct spinlock *lock);

void spinlock_init(struct spinlock *lock);

#endif