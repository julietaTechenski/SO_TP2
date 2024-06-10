global acquireLock
global releaseLock


acquireLock:
    mov eax, 1
    lock xchg eax, dword [rdi]
    cmp eax, 0
    jne acquireLock
    ret

releaseLock:
    mov dword [rdi], 0
    ret
