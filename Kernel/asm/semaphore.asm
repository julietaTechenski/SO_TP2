global acquireLock
global releaseLock

acquireLock:
    sti
    lock bts dword [rdi], 0
    jc .spin_wait
    cli
    ret

.spin_wait:
    test dword [rdi], 1
    jnz .spin_wait
    jmp acquireLock

releaseLock:
    mov dword [rdi],0
    ret