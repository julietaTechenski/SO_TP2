global acquireLock
global releaseLock

acquireLock:
    lock bts dword [rdi], 0
    jc .spin_wait
    ret

.spin_wait:
    test dword [rdi],1
    jnz .spin_wait
    jmp acquireLock

releaseLock:
    mov dword [rdi],0
    ret