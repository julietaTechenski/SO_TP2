global acquireLock
global releaseLock

//void acquireLock(uint32_t* lock)
acquireLock:
    lock bts [rdi],0
    jc .spin_wait
    ret

.spin_wait:
    test dword [rdi],1
    jnz .spin_wait
    jmp acquireLock

//void releaseLock(uint32_t* lock)
releaseLock:
    mov dword [rdi],0
    ret