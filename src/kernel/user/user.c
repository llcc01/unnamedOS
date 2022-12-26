#include "sched/sched.h"
#include "drivers/uart/16550.h"
#include "user.h"

#define DELAY 1000

#define TASK_FUN(n)                            \
    void user_task##n(void)                      \
    {                                          \
        uart_puts("Task "#n": Created!\n");       \
        while (1)                              \
        {                                      \
            uart_puts("Task "#n": Running...\n"); \
            task_delay(DELAY);                 \
            task_yield();                      \
        }                                      \
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

TASK_FUN(0)
TASK_FUN(1)
TASK_FUN(2)
TASK_FUN(3)
TASK_FUN(4)
TASK_FUN(5)

/* NOTICE: DON'T LOOP INFINITELY IN main() */
void os_main(void)
{
    uart_puts("entry os_main\n");
    task_create(user_task0);
    task_create(user_task1);
    task_create(user_task2);
    task_create(user_task3);
    task_create(user_task4);
    task_create(user_task5);


    uart_puts("os_main: schedule\n");
    schedule();
}