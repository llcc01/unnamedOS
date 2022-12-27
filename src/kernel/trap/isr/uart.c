#include "drivers/uart/16550.h"
#include "trap/isr/uart.h"

/*
 * handle a uart interrupt, raised because input has arrived, called from trap.c.
 */
void uart_isr(void)
{
	while (1) {
		int c = uart_getc();
		if (c == -1) {
			break;
		} else {
			uart_putc((char)c);
			uart_putc('\n');
		}
	}
}