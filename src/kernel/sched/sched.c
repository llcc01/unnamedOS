#include "inc/types.h"
#include "utils/printf.h"
#include "sched/switch.h"
#include "sched/sched.h"

#define TASK_MAX_NUM 64
struct task_stack task_stack[TASK_MAX_NUM];
struct context task_ctx[TASK_MAX_NUM];

static int _top = 0;
static int _current = -1;

static void w_mscratch(reg_t x)
{
    asm volatile("csrw mscratch, %0"
                 :
                 : "r"(x));
}

void sched_init()
{
    w_mscratch(0);
}

/*
 * implment a simple cycle FIFO schedular
 */
void schedule()
{
    if (_top <= 0)
    {
        panic("Num of task should be greater than zero!");
        return;
    }

    _current = (_current + 1) % _top;
    struct context *next = &task_ctx[_current];
    // printf("turn to %d, ctx:%p\n", _current, next);
    switch_to(next);
}

/*
 * DESCRIPTION
 *     Create a task.
 *     - start_routin: task routine entry
 * RETURN VALUE
 *     0: success
 *     -1: if error occured
 */
int task_create(void (*start_routin)(void))
{
    if (_top < TASK_MAX_NUM)
    {
        task_ctx[_top].sp = (reg_t)&task_stack[_top].data[STACK_SIZE - 1];
        task_ctx[_top].ra = (reg_t)start_routin;
        _top++;
        return 0;
    }
    else
    {
        return -1;
    }
}

/*
 * DESCRIPTION
 *     task_yield()  causes the calling task to relinquish the CPU and a new
 *     task gets to run.
 */
void task_yield()
{
    schedule();
}

/*
 * a very rough implementaion, just to consume the cpu
 */
void task_delay(volatile int count)
{
    count *= 50000;
    while (count--)
        ;
}
