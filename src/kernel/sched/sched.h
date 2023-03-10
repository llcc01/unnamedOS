#ifndef __SCHED_H__
#define __SCHED_H__

#include "inc/types.h"

#define STACK_SIZE 1024

struct context
{
    /* ignore x0 */
    reg_t ra;
    reg_t sp;
    reg_t gp;
    reg_t tp;
    reg_t t0;
    reg_t t1;
    reg_t t2;
    reg_t s0;
    reg_t s1;
    reg_t a0;
    reg_t a1;
    reg_t a2;
    reg_t a3;
    reg_t a4;
    reg_t a5;
    reg_t a6;
    reg_t a7;
    reg_t s2;
    reg_t s3;
    reg_t s4;
    reg_t s5;
    reg_t s6;
    reg_t s7;
    reg_t s8;
    reg_t s9;
    reg_t s10;
    reg_t s11;
    reg_t t3;
    reg_t t4;
    reg_t t5;
    reg_t t6;

    // save the pc to run in next schedule cycle
    reg_t pc; // offset: 31 *4 = 124 or 31 *8 = 248
};

struct task_stack
{
    uint8_t data[STACK_SIZE];
};

struct task_meta
{
    uint16_t tick_max;
};

void sched_init(void);

int task_create(void (*start_routin)(void),uint32_t tick_max);
void task_delay(volatile int count);
void task_yield();

void schedule();

void task_print_reg();

void sched_start();

#endif