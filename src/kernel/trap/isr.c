#include "inc/types.h"
#include "inc/platform.h"
#include "inc/riscv.h"
#include "drivers/uart/16550.h"
#include "drivers/plic/plic.h"
#include "drivers/clint/timer.h"
#include "utils/printf.h"
#include "trap/isr.h"
#include "sched/sched.h"

void external_interrupt_handler()
{
    int irq = plic_claim();
    switch (irq)
    {
    case UART0_IRQ:
        uart_isr();
        break;
    default:
        printf("unexpected interrupt irq = %d\n", irq);
        break;
    }

    if (irq)
    {
        plic_complete(irq);
    }
}

/*
 * handle a uart interrupt, raised because input has arrived, called from trap.c.
 */
void uart_isr(void)
{
    while (1)
    {
        int c = uart_getc();
        if (c == -1)
        {
            break;
        }
        else
        {
            uart_putc((char)c);
            uart_putc('\n');
        }
    }
}

void timer_handler()
{
    _tick++;
    // if (_tick % 10 == 0)
    // {
    //     printf("tick: %d\n", _tick);
    // }

    timer_load(TIMER_INTERVAL);

    schedule();
}