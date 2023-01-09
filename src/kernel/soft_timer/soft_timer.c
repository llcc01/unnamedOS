#include "inc/types.h"
#include <stddef.h>

#include "drivers/clint/timer.h"

#include "lock/lock.h"
#include "ds/queue.h"
#include "ds/heap.h"

#include "utils/malloc.h"
#include "utils/printf.h"

#include "soft_timer/soft_timer.h"

enum soft_timer_op_type
{
    SOFT_TIMER_OP_INSERT = 0,
    SOFT_TIMER_OP_DELETE = 1,
};

struct soft_timer_op
{
    struct soft_timer *timer;
    enum soft_timer_op_type op;
};

struct spinlock soft_timer_lock;
struct queue _free_queue; // 数据为id uint16_t
struct queue _op_queue;

struct soft_timer **_soft_timers;
min_heap_t _timer_min_heap; // 保存timer的最小堆

uint16_t _soft_timer_id_end = 0;

uint64_t _next_tick = 0;

void soft_timer_init()
{
    spinlock_init(&soft_timer_lock);
    queue_init(&_free_queue);
    queue_init(&_op_queue);
    binary_tree_init(&_timer_min_heap.tree);
    _timer_min_heap.tree.cmp = soft_timer_cmp;
}

int soft_timer_cmp(void *a, void *b)
{
    struct soft_timer *timer_a = (struct soft_timer *)a;
    struct soft_timer *timer_b = (struct soft_timer *)b;

    return timer_a->next_tick - timer_b->next_tick;
}

uint16_t soft_timer_create(void (*callback)(void *arg), void *arg, uint32_t interval)
{
    spin_lock(&soft_timer_lock);
    struct soft_timer **new_timers;
    uint16_t id_new = 0;
    if (_free_queue.size == 0)
    {
        new_timers = (struct soft_timer **)realloc(_soft_timers, sizeof(struct soft_timer *) * (_soft_timer_id_end + 1));
        if (new_timers == NULL)
        {
            spin_unlock(&soft_timer_lock);
            printf("soft_timer_create: new_timers realloc failed\n");
            return SOFT_TIMER_ID_INVALID;
        }
        _soft_timers = new_timers;
        id_new = _soft_timer_id_end;
        _soft_timer_id_end++;
    }
    else
    {
        reg_t id_temp = (reg_t)queue_pop(&_free_queue);
        id_new = (uint16_t)id_temp;
    }

    struct soft_timer *timer = (struct soft_timer *)malloc(sizeof(struct soft_timer));
    if (timer == NULL)
    {
        spin_unlock(&soft_timer_lock);
        printf("soft_timer_create: timer malloc failed\n");
        return SOFT_TIMER_ID_INVALID;
    }
    timer->callback = callback;
    timer->arg = arg;
    timer->interval = interval;

    _soft_timers[id_new] = timer;

    struct soft_timer_op *op = (struct soft_timer_op *)malloc(sizeof(struct soft_timer_op));
    op->timer = timer;
    op->op = SOFT_TIMER_OP_INSERT;

    queue_push(&_op_queue, op);

    spin_unlock(&soft_timer_lock);

    return id_new;
}

/**
 * @brief 释放定时器
 * 此处只是标记为无效，实际释放在soft_timer_calc_next中
 */
void soft_timer_delete(struct soft_timer *timer)
{
    spin_lock(&soft_timer_lock);

    struct soft_timer_op *op = (struct soft_timer_op *)malloc(sizeof(struct soft_timer_op));
    op->timer = timer;
    op->op = SOFT_TIMER_OP_DELETE;
    queue_push(&_op_queue, op);

    spin_unlock(&soft_timer_lock);
}

inline void soft_timer_calc_next()
{
    if (_timer_min_heap.tree.root == NULL)
    {
        _next_tick = 0;
        return;
    }

    // printf("soft_timer_calc_next heap top: %p\n", _timer_min_heap.tree.root->data);
    struct soft_timer *timer = min_heap_get_top_value(&_timer_min_heap);
    // printf("soft_timer_calc_next: %p\n", timer);

    // 如果堆顶的timer无效，则回收，继续取下一个
    while (timer->interval == 0)
    {
        reg_t id_temp = (reg_t)timer->id;
        queue_push(&_free_queue, (void *)id_temp);
        free(timer);
        timer = min_heap_get(&_timer_min_heap);
        // printf("soft_timer_calc_next loop: %p\n", timer);
    }

    _next_tick = timer->next_tick;
}

/**
 * @brief 更新堆
 * 根据队列更新堆，忽略堆中无效的timer
 */
inline void soft_timer_heap_update()
{
    struct soft_timer_op *op;
    while (_op_queue.size > 0)
    {
        // printf("soft_timer_heap_update: %p\n", &_op_queue);
        op = (struct soft_timer_op *)queue_pop(&_op_queue);
        // printf("soft_timer_heap_update op: %p\n", op);
        switch (op->op)
        {
        case SOFT_TIMER_OP_INSERT:
            op->timer->next_tick = _tick + op->timer->interval;
            min_heap_insert(&_timer_min_heap, op->timer);
            // printf("soft_timer_heap_update heap top: %p %p\n", _timer_min_heap.tree.root, _timer_min_heap.tree.root->data);
            break;
        case SOFT_TIMER_OP_DELETE:
            op->timer->interval = 0;
            op->timer->next_tick = 0; // 在一定条件下，堆排序后会置顶，最终在soft_timer_calc_next被回收
        default:
            break;
        }
        // printf("soft_timer_heap_update2 heap top: %p %p\n", _timer_min_heap.tree.root, _timer_min_heap.tree.root->data);

        free(op);
        // printf("soft_timer_heap_update3 heap top: %p %p\n", _timer_min_heap.tree.root, _timer_min_heap.tree.root->data);
    }
}

inline void soft_timer_handler()
{
    if (_next_tick == _tick)
    {
        struct soft_timer *timer = NULL;
        while (1)
        {
            timer = (struct soft_timer *)min_heap_get_top_value(&_timer_min_heap);
            if (timer->next_tick > _tick)
                break;
            timer->callback(timer->arg);
            timer->next_tick += timer->interval;
            min_heap_sort_down(&_timer_min_heap, _timer_min_heap.tree.root);
        }
    }

    soft_timer_heap_update();
    // if (_timer_min_heap.tree.root != NULL)
    //     printf("soft_timer_handler heap top: %p %p\n", _timer_min_heap.tree.root, _timer_min_heap.tree.root->data);
    soft_timer_calc_next();
}