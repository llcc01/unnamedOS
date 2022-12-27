#ifndef __DRIVERS_PLIC_H__
#define __DRIVERS_PLIC_H__

void plic_init(void);
int plic_claim(void);
void plic_complete(int irq);

#endif /* __DRIVERS_PLIC_H__ */