#include "drivers/uart/16550.h"
#include "drivers/plic/plic.h"
#include "drivers/clint/timer.h"
#include "utils/printf.h"
#include "utils/malloc.h"
#include "mem/page.h"
#include "sched/sched.h"
#include "trap/trap.h"

#include "soft_timer/soft_timer.h"

#include "user/user.h"

void start_kernel(void)
{
    uart_init();
    printf("Hello, world!\n");

    page_init();
    // page_test();
    malloc_init();

    trap_init();
    plic_init();

    sched_init();
    timer_init();
    soft_timer_init();
    
    os_main();

    uart_puts("Would not go here!\n");
    while (1)  // stop here!
    {
        uart_puts("Would not go here!\n");
        task_delay(100);
    }
}

