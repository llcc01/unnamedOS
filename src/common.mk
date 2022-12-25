CROSS_COMPILE = riscv64-unknown-elf-
CFLAGS = -I. -nostdlib -fno-builtin -march=rv64ima -mabi=lp64 -mcmodel=medany -g -Wall
# CFLAGS = -I. -nostdlib -fno-builtin -march=rv32ima -mabi=ilp32 -mcmodel=medany -g -Wall

QEMU = qemu-system-riscv64
# QEMU = qemu-system-riscv32
QFLAGS = -nographic -smp 1 -machine virt -bios none

GDB = gdb-multiarch
CC = ${CROSS_COMPILE}gcc
OBJCOPY = ${CROSS_COMPILE}objcopy
OBJDUMP = ${CROSS_COMPILE}objdump