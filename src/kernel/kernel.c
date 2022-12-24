#include "drivers/uart/16550.h"

void start_kernel(void)
{
	uart_init();
	uart_puts("Hello, world!\n");

	while (1) ; // stop here!
}

