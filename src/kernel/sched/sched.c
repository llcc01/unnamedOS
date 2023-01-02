#include "inc/types.h"
#include "inc/riscv.h"
#include "inc/platform.h"
#include "drivers/clint/timer.h"
#include "utils/printf.h"
#include "sched/switch.h"
#include "sched/sched.h"

#define TASK_MAX_NUM 64
struct task_stack task_stack[TASK_MAX_NUM];
struct context task_ctx[TASK_MAX_NUM];
struct task_meta task_meta[TASK_MAX_NUM];

struct context task_kernel_ctx;

static int _top = 0;
static int _current = -1;

void sched_init()
{
    w_mscratch((reg_t)&task_kernel_ctx);

    // /* enable machine-mode software interrupts. */
    // w_mie(r_mie() | MIE_MSIE);
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

    static uint32_t tick_count = 0;
    static uint32_t tick_max = 0;
    tick_count++;
    if (tick_count < tick_max)
    {
        return;
    }
    tick_count = 0;

    _current = (_current + 1) % _top;
    struct context *next = &task_ctx[_current];
    tick_max = task_meta[_current].tick_max;
    // printf("turn to %d, ctx: %p, tick_max: %d\n", _current, next, tick_max);
    switch_to(next);
}

void sched_start()
{
    if (_top <= 0)
    {
        panic("Num of task should be greater than zero!");
        return;
    }

    /* enable machine-mode global interrupts. */
    w_mstatus(r_mstatus() | MSTATUS_MIE);
}

/*
 * DESCRIPTION
 *     Create a task.
 *     - start_routin: task routine entry
 * RETURN VALUE
 *     0: success
 *     -1: if error occured
 */
int task_create(void (*start_routin)(void), uint32_t tick_max)
{
    if (_top < TASK_MAX_NUM)
    {
        task_ctx[_top].sp = (reg_t)&task_stack[_top].data[STACK_SIZE - 1];
        task_ctx[_top].pc = (reg_t)start_routin;
        // task_ctx[_top].ra = (reg_t)start_routin;
        task_meta[_top].tick_max = tick_max;

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
    // /* trigger a machine-level software interrupt */
    // int id = r_mhartid();
    // *(uint32_t *)CLINT_MSIP(id) = 1;

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

void task_print_reg()
{
    printf("task_current: %d\n", _current);
    printf("regs:\n");
    struct context *ctx = &task_ctx[_current];
    printf("\
ra=%p sp =%p  gp =%p tp=%p\n\
t0=%p t1 =%p  t2 =%p s0=%p\n\
s1=%p a0 =%p  a1 =%p a2=%p\n\
a3=%p a4 =%p  a5 =%p a6=%p\n\
a7=%p s2 =%p  s3 =%p s4=%p\n\
s5=%p s6 =%p  s7 =%p s8=%p\n\
s9=%p s10=%p  s11=%p t3=%p\n\
t4=%p t5 =%p  t6 =%p pc=%p\n",
           ctx->ra, ctx->sp, ctx->gp, ctx->tp,
           ctx->t0, ctx->t1, ctx->t2, ctx->s0,
           ctx->s1, ctx->a0, ctx->a1, ctx->a2,
           ctx->a3, ctx->a4, ctx->a5, ctx->a6,
           ctx->a7, ctx->s2, ctx->s3, ctx->s4,
           ctx->s5, ctx->s6, ctx->s7, ctx->s8,
           ctx->s9, ctx->s10, ctx->s11, ctx->t3,
           ctx->t4, ctx->t5, ctx->t6, ctx->pc);
}