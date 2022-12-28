#include "reg.s"

.text

# void switch_to(struct context *next);
# a0: pointer to the context of the next task
.globl switch_to
.align 4
switch_to:
    csrrw    t6, mscratch, t6    # swap t6 and mscratch
    beqz    t6, 1f            # Notice: the first time switch_to() is
                                    # called, mscratch is initialized as zero
                    # (in sched_init()), which makes t6 zero,
                    # and that's the special case we have to
                    # handle with t6
    reg_save t6            # save context of prev task

    # Save the actual t6 register, which we swapped into
    # mscratch
    mv    t5, t6        # t5 points to the context of next task
    csrr    t6, mscratch    # read t6 back from mscratch
    sw    t6, _t6_offset(t5)    # save t6 with t5 as base

1:
    # switch mscratch to point to the context of the next task
    csrw    mscratch, a0

    # Restore all GP registers
    # Use t6 to point to the context of the new task
    mv    t6, a0
    reg_restore t6

    # Do actual context switching.
    ret

.end

