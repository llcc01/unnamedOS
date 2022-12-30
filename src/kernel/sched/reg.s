# save all General-Purpose(GP) registers to context
# struct context *base = &task_ctx;
# base->ra = ra;
# ......
# !!! in risc-v32, offset should be 4; in risc-v64, offset should be 8
.macro reg_save32 base
    sw ra, 0(\base)
    sw sp, 4(\base)
    sw gp, 8(\base)
    sw tp, 12(\base)
    sw t0, 16(\base)
    sw t1, 20(\base)
    sw t2, 24(\base)
    sw s0, 28(\base)
    sw s1, 32(\base)
    sw a0, 36(\base)
    sw a1, 40(\base)
    sw a2, 44(\base)
    sw a3, 48(\base)
    sw a4, 52(\base)
    sw a5, 56(\base)
    sw a6, 60(\base)
    sw a7, 64(\base)
    sw s2, 68(\base)
    sw s3, 72(\base)
    sw s4, 76(\base)
    sw s5, 80(\base)
    sw s6, 84(\base)
    sw s7, 88(\base)
    sw s8, 92(\base)
    sw s9, 96(\base)
    sw s10, 100(\base)
    sw s11, 104(\base)
    sw t3, 108(\base)
    sw t4, 112(\base)
    sw t5, 116(\base)
    # we don't save t6 here, due to we have used
    # it as base, we have to save t6 in an extra step
    # outside of reg_save
.endm

.macro reg_save64 base
    sd ra, 0(\base)
    sd sp, 8(\base)
    sd gp, 16(\base)
    sd tp, 24(\base)
    sd t0, 32(\base)
    sd t1, 40(\base)
    sd t2, 48(\base)
    sd s0, 56(\base)
    sd s1, 64(\base)
    sd a0, 72(\base)
    sd a1, 80(\base)
    sd a2, 88(\base)
    sd a3, 96(\base)
    sd a4, 104(\base)
    sd a5, 112(\base)
    sd a6, 120(\base)
    sd a7, 128(\base)
    sd s2, 136(\base)
    sd s3, 144(\base)
    sd s4, 152(\base)
    sd s5, 160(\base)
    sd s6, 168(\base)
    sd s7, 176(\base)
    sd s8, 184(\base)
    sd s9, 192(\base)
    sd s10, 200(\base)
    sd s11, 208(\base)
    sd t3, 216(\base)
    sd t4, 224(\base)
    sd t5, 232(\base)
    # we don't save t6 here, due to we have used
    # it as base, we have to save t6 in an extra step
    # outside of reg_save
.endm

# restore all General-Purpose(GP) registers from the context
# struct context *base = &task_ctx;
# ra = base->ra;
# ......
# !!! in risc-v32, offset should be 4; in risc-v64, offset should be 8
.macro reg_restore32 base
    lw ra, 0(\base)
    lw sp, 4(\base)
    lw gp, 8(\base)
    lw tp, 12(\base)
    lw t0, 16(\base)
    lw t1, 20(\base)
    lw t2, 24(\base)
    lw s0, 28(\base)
    lw s1, 32(\base)
    lw a0, 36(\base)
    lw a1, 40(\base)
    lw a2, 44(\base)
    lw a3, 48(\base)
    lw a4, 52(\base)
    lw a5, 56(\base)
    lw a6, 60(\base)
    lw a7, 64(\base)
    lw s2, 68(\base)
    lw s3, 72(\base)
    lw s4, 76(\base)
    lw s5, 80(\base)
    lw s6, 84(\base)
    lw s7, 88(\base)
    lw s8, 92(\base)
    lw s9, 96(\base)
    lw s10, 100(\base)
    lw s11, 104(\base)
    lw t3, 108(\base)
    lw t4, 112(\base)
    lw t5, 116(\base)
    lw t6, 120(\base)
.endm

.macro reg_restore64 base
    ld ra, 0(\base)
    ld sp, 8(\base)
    ld gp, 16(\base)
    ld tp, 24(\base)
    ld t0, 32(\base)
    ld t1, 40(\base)
    ld t2, 48(\base)
    ld s0, 56(\base)
    ld s1, 64(\base)
    ld a0, 72(\base)
    ld a1, 80(\base)
    ld a2, 88(\base)
    ld a3, 96(\base)
    ld a4, 104(\base)
    ld a5, 112(\base)
    ld a6, 120(\base)
    ld a7, 128(\base)
    ld s2, 136(\base)
    ld s3, 144(\base)
    ld s4, 152(\base)
    ld s5, 160(\base)
    ld s6, 168(\base)
    ld s7, 176(\base)
    ld s8, 184(\base)
    ld s9, 192(\base)
    ld s10, 200(\base)
    ld s11, 208(\base)
    ld t3, 216(\base)
    ld t4, 224(\base)
    ld t5, 232(\base)
    ld t6, 240(\base)
.endm

# Something to note about save/restore:
# - We use mscratch to hold a pointer to context of previous task
# - We use t6 as the 'base' for reg_save/reg_restore, because it is the
#   very bottom register (x31) and would not be overwritten during loading.

# !!! in risc-v32, offset should be 120=4*30; in risc-v64, offset should be 240=8*30
#define _t6_offset 240
#define _mepc_offset 248
#define reg_restore reg_restore64
#define reg_save reg_save64