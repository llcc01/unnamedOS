#include "inc/types.h"
#include "vector.h"
#include "inc/riscv.h"
#include "utils/printf.h"
#include "drivers/uart/16550.h"
#include "trap/isr.h"
#include "trap/trap.h"

void trap_init()
{
    /*
     * set the trap-vector base-address for machine-mode
     */
    w_mtvec((reg_t)trap_vector);
}

reg_t trap_handler(reg_t epc, reg_t cause)
{
    reg_t return_pc = epc;
    reg_t cause_code = cause & 0xfff;

    /* if the highest bit is 1 */
    if (cause >> (sizeof(ssize_t) * 8 - 1))
    {
        /* Asynchronous trap - interrupt */
        switch (cause_code)
        {
        case 3:
            uart_puts("software interruption!\n");
            break;
        case 7:
            uart_puts("timer interruption!\n");
            break;
        case 11:
            uart_puts("external interruption!\n");
            external_interrupt_handler();
            break;
        default:
            uart_puts("unknown async exception!\n");
            break;
        }
    }
    else
    {
        /* Synchronous trap - exception */
        printf("Sync exceptions!, code = %d\n", cause_code);

        panic("OOPS! What can I do!");
        // return_pc += 4;
    }

    return return_pc;
}

void trap_test()
{
    /*
     * Synchronous exception code = 7
     * Store/AMO access fault
     */
    *(int *)0x00000000 = 100;

    /*
     * Synchronous exception code = 5
     * Load access fault
     */
    // int a = *(int *)0x00000000;

    uart_puts("Yeah! I'm return back from trap!\n");
}
