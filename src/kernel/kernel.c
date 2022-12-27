#include "drivers/uart/16550.h"
#include "drivers/plic/plic.h"
#include "utils/printf.h"
#include "mem/page.h"
#include "sched/sched.h"
#include "trap/trap.h"

#include "user/user.h"

void start_kernel(void)
{
    uart_init();
    printf("Hello, world!\n");

    page_init();
    // page_test();

    trap_init();
    plic_init();
    sched_init();
    
    os_main();

    uart_puts("Would not go here!\n");
    while (1) ; // stop here!
}

