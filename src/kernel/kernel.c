#include "drivers/uart/16550.h"
#include "utils/printf.h"
#include "mem/page.h"

void start_kernel(void)
{
	uart_init();
	printf("Hello, world!\n");

	page_init();
	page_test();

	while (1) ; // stop here!
}

