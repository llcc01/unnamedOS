#include "sched/sched.h"
#include "drivers/uart/16550.h"
#include "utils/printf.h"
#include "trap/trap.h"
#include "user/user.h"
#include "lock/lock.h"

#define DELAY 1000

#define TASK_FUN(n)                                 \
    void user_task##n(void)                         \
    {                                               \
        uart_puts("Task " #n ": Created!\n");       \
        while (1)                                   \
        {                                           \
            uart_puts("Task " #n ": Running...\n"); \
            task_delay(DELAY);                      \
        }                                           \
    }

#define TASK_FUN_LOCK(n)                                       \
    void user_task_lock##n(void)                               \
    {                                                          \
        uart_puts("Task lock " #n ": Created!\n");             \
        while (1)                                              \
        {                                                      \
            spin_lock(&lock);                                  \
            for (int i = 0; i < 4; i++)                        \
            {                                                  \
                printf("Task lock " #n ": Running%d...\n", i); \
                task_delay(DELAY);                             \
            }                                                  \
            spin_unlock(&lock);                                \
            task_delay(DELAY);                                 \
        }                                                      \
    }
// void user_task0(void)
// {
//     uart_puts("Task 0: Created!\n");
//     while (1)
//     {
//         uart_puts("Task 0: Running...\n");
//         task_delay(DELAY);
//         task_yield();
//     }
// }

// void user_task1(void)
// {
//     uart_puts("Task 1: Created!\n");
//     while (1)
//     {
//         uart_puts("Task 1: Running...\n");
//         task_delay(DELAY);
//         task_yield();
//     }
// }

struct spinlock lock;

TASK_FUN(0)
TASK_FUN(1)
// TASK_FUN(2)
// TASK_FUN(3)
// TASK_FUN(4)
// TASK_FUN(5)
TASK_FUN_LOCK(0)
TASK_FUN_LOCK(1)

void user_task_trap(void)
{
    uart_puts("Task trap: Created!\n");
    while (1)
    {
        uart_puts("Task trap: Running...\n");
        // trap_test();
        *((int *)0) = 0;
        printf("%d\n", *((int *)0));

        task_delay(DELAY);
        task_yield();
    }
}

/* NOTICE: DON'T LOOP INFINITELY IN main() */
void os_main(void)
{
    uart_puts("entry os_main\n");
    spinlock_init(&lock);
    task_create(user_task0, 10);
    task_create(user_task1, 12);
    task_create(user_task_lock0, 10);
    task_create(user_task_lock1, 10);
    // task_create(user_task2, 15);
    // task_create(user_task3, 20);
    // task_create(user_task4, 30);
    // task_create(user_task5, 20);
    // task_create(user_task_trap);

    uart_puts("os_main: schedule\n");

    sched_start();

    while (1)
        ;
}