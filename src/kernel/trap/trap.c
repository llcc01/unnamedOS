#include "inc/types.h"
#include "trap/vector.h"
#include "inc/riscv.h"
#include "utils/printf.h"
#include "drivers/uart/16550.h"
#include "trap/isr.h"
#include "trap/trap.h"
#include "sched/sched.h"

const char *cause_msg[] = {
    "Instruction address misaligned",
    "Instruction access fault",
    "Illegal instruction",
    "Breakpoint",
    "Load address misaligned",
    "Load access fault",
    "Store/AMO address misaligned",
    "Store/AMO access fault",
    "Environment call from U-mode",
    "Environment call from S-mode",
    "Reserved",
    "Environment call from M-mode",
    "Instruction page fault",
    "Load page fault",
    "Reserved",
    "Store/AMO page fault"};

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
    if (cause >> (sizeof(reg_t) * 8 - 1))
    {
        /* Asynchronous trap - interrupt */
        switch (cause_code)
        {
        case 3:
            uart_puts("software interruption!\n");
            break;
        case 7:
            // uart_puts("timer interruption!\n");
            timer_handler();
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
        if (cause_code < (sizeof(cause_msg) / sizeof(char *)))
        {
            printf("Exception: %s\n", cause_msg[cause_code]);
        }
        printf("Instruction Addr: %p\n", epc);
        task_print_reg();
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
