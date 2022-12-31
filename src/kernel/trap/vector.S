#include "sched/reg.s"

.text

# interrupts and exceptions while in machine mode come here.
.globl trap_vector
# the trap vector base address must always be aligned on a 4-byte boundary
.align 4
trap_vector:
	# save context(registers).
	csrrw	t6, mscratch, t6	# swap t6 and mscratch
	reg_save t6

	# Save the actual t6 register, which we swapped into
	# mscratch
	mv	t5, t6		# t5 points to the context of current task
	csrr	t6, mscratch	# read t6 back from mscratch
	sd	t6, _t6_offset(t5)	# save t6 with t5 as base

	# save mepc to context of current task
	csrr	a0, mepc
	sd	a0, _mepc_offset(t5)

	# Restore the context pointer into mscratch
	csrw	mscratch, t5

	# call the C trap handler in trap.c
	csrr	a0, mepc
	csrr	a1, mcause
	call	trap_handler

	# trap_handler will return the return address via a0.
	csrw	mepc, a0

	# restore context(registers).
	csrr	t6, mscratch
	reg_restore t6

	# return to whatever we were doing before trap.
	mret