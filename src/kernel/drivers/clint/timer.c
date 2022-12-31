#include "inc/types.h"
#include "inc/riscv.h"
#include "inc/platform.h"
#include "drivers/clint/timer.h"

uint32_t _tick = 0;

/* load timer interval(in ticks) for next timer interrupt.*/
void timer_load(int interval)
{
    /* each CPU has a separate source of timer interrupts. */
    int id = r_mhartid();

    *(uint64_t *)CLINT_MTIMECMP(id) = *(uint64_t *)CLINT_MTIME + interval;
}

void timer_init()
{
    /*
     * On reset, mtime is cleared to zero, but the mtimecmp registers
     * are not reset. So we have to init the mtimecmp manually.
     */
    timer_load(TIMER_INTERVAL);

    /* enable machine-mode timer interrupts. */
    w_mie(r_mie() | MIE_MTIE);

}