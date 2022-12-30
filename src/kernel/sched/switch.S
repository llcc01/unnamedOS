#include "reg.s"

.text

# void switch_to(struct context *next);
# a0: pointer to the context of the next task
.globl switch_to
.align 4
switch_to:
	# switch mscratch to point to the context of the next task
	csrw	mscratch, a0
	# set mepc to the pc of the next task
	lw	a1, _mepc_offset(a0)
	csrw	mepc, a1

	# Restore all GP registers
	# Use t6 to point to the context of the new task
	mv	t6, a0
	reg_restore t6

	# Do actual context switching.
	# Notice this will enable global interrupt
	mret

.end

