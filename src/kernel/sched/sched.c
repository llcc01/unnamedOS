#include "inc/types.h"
#include "inc/riscv.h"
#include "inc/platform.h"
#include "utils/printf.h"
#include "sched/switch.h"
#include "sched/sched.h"

#define TASK_MAX_NUM 64
struct task_stack task_stack[TASK_MAX_NUM];
struct context task_ctx[TASK_MAX_NUM];

static int _top = 0;
static int _current = -1;

void sched_init()
{
    w_mscratch(0);

    /* enable machine-mode software interrupts. */
    w_mie(r_mie() | MIE_MSIE);
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
        task_ctx[_top].pc = (reg_t)start_routin;
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
inline void task_yield()
{
    /* trigger a machine-level software interrupt */
    int id = r_mhartid();
    *(uint32_t *)CLINT_MSIP(id) = 1;
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

void task_print_reg()
{
    printf("task_current: %d\n", _current);
    printf("reg:\n");
    struct context *ctx = &task_ctx[_current];
    printf("\
ra=%x sp =%x  gp =%x tp=%x\n\
t0=%x t1 =%x  t2 =%x s0=%x\n\
s1=%x a0 =%x  a1 =%x a2=%x\n\
a3=%x a4 =%x  a5 =%x a6=%x\n\
a7=%x s2 =%x  s3 =%x s4=%x\n\
s5=%x s6 =%x  s7 =%x s8=%x\n\
s9=%x s10=%x  s11=%x t3=%x\n\
t4=%x t5 =%x  t6 =%x pc=%x\n",
        ctx->ra, ctx->sp, ctx->gp, ctx->tp,
        ctx->t0, ctx->t1, ctx->t2, ctx->s0,
        ctx->s1, ctx->a0, ctx->a1, ctx->a2,
        ctx->a3, ctx->a4, ctx->a5, ctx->a6,
        ctx->a7, ctx->s2, ctx->s3, ctx->s4,
        ctx->s5, ctx->s6, ctx->s7, ctx->s8,
        ctx->s9, ctx->s10, ctx->s11, ctx->t3,
        ctx->t4, ctx->t5, ctx->t6, ctx->pc);
}