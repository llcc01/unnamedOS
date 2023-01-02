
.global spin_lock
spin_lock:
    li a5, 1
    amoswap.w a5,a5,(a0)
    bnez a5, spin_lock
    ret
    
.global spin_unlock
spin_unlock:
    sw x0,(a0)
    ret