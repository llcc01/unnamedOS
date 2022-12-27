#include "inc/types.h"
#include "inc/platform.h"
#include "inc/riscv.h"
#include "drivers/plic/plic.h"
#include "trap/isr/uart.h"
#include "utils/printf.h"
#include "trap/isr.h"

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