include ../common.mk

SRCS_ASM = \
	start.S \
	mem/mem_const.S \
	sched/switch.S \
	trap/vector.S \
	lock/spinlock.S \

SRCS_C = \
	kernel.c \
	drivers/uart/16550.c \
	drivers/plic/plic.c \
	drivers/clint/timer.c \
	mem/page.c \
	ds/binary_tree.c \
	ds/heap.c \
	ds/linked_list.c \
	ds/queue.c \
	utils/printf.c \
	utils/string.c \
	utils/malloc.c \
	sched/sched.c \
	user/user.c \
	trap/trap.c \
	trap/isr.c \
	lock/lock.c \
	soft_timer/soft_timer.c \

OBJS = $(SRCS_ASM:.S=.o)
OBJS += $(SRCS_C:.c=.o)

.DEFAULT_GOAL := all
all: os.elf

# start.o must be the first in dependency!
os.elf: ${OBJS}
	${CC} ${CFLAGS} -T kernel.ld -o os.elf $^
	${OBJCOPY} -O binary os.elf os.bin

%.o : %.c
	${CC} ${CFLAGS} -c -o $@ $<

%.o : %.S
	${CC} ${CFLAGS} -c -o $@ $<

run: all
	@${QEMU} -M ? | grep virt >/dev/null || exit
	@echo "Press Ctrl-A and then X to exit QEMU"
	@echo "------------------------------------"
	@${QEMU} ${QFLAGS} -kernel os.elf

.PHONY : debug
debug: all
	@echo "Press Ctrl-C and then input 'quit' to exit GDB and QEMU"
	@echo "-------------------------------------------------------"
	@${QEMU} ${QFLAGS} -kernel os.elf -s -S &
	@${GDB} os.elf -q -x ./gdbinit

.PHONY : debug2
debug2: all
	@echo "Press Ctrl-C and then input 'quit' to exit GDB and QEMU"
	@echo "-------------------------------------------------------"
	@${QEMU} ${QFLAGS} -kernel os.elf -s -S 
# @${GDB} os.elf -q -x ./gdbinit

.PHONY : code
code: all
	@${OBJDUMP} -S os.elf | less

.PHONY : clean
clean:
	rm -f *.bin *.elf
	find -name "*.o" | xargs rm -f
