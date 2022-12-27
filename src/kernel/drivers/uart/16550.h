#ifndef __16550_H__
#define __16550_H__

void uart_init();
void uart_puts(char *);
int uart_putc(char ch);
int uart_getc(void);

#endif