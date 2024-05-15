
GLOBAL _hlt
GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler

GLOBAL _exception0Handler
GLOBAL _exception6Handler

GLOBAL _sysCallHandler
GLOBAL haltcpu
GLOBAL getKeyPressed

GLOBAL sound
GLOBAL noSnd


EXTERN irqDispatcher
EXTERN exceptionDispatcher
EXTERN sysCallHandler
EXTERN getStackBase

SECTION .text

%macro pushStateNoA 0
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popStateNoA 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
%endmacro

%macro popState 0
	popStateNoA
	pop rax
	mov [reg_rip], rax
	pop rax ;RIP
	mov rax, [reg_rip]
%endmacro

%macro pushState 0
    mov [reg_rip], rax
    mov rax, [rsp]
    push rax    ;RIP
    mov rax, [reg_rip]
	push rax
    pushStateNoA
%endmacro

%macro irqHandlerMaster 1
	pushState

	mov rdi, %1     ; argument passage
    mov rsi, rsp    ; pointer to direction in stack where registers can be found
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro



%macro exceptionHandler 1
    mov rsi, rsp     ; get registers
	mov rdi, %1      ; argument passage
	call exceptionDispatcher

    ; recover and go back to the shell
    call getStackBase
    mov [rsp+24], rax
    mov rax, userland
    mov [rsp], rax
	iretq
%endmacro



_hlt:
	sti
	hlt
	ret

_cli:
	cli ; In the x86 instruction set CLI is commonly used as a synchronization mechanism in uniprocessor systems. For example, a CLI is used in operating systems to disable interrupts so kernel code (typically a driver) can avoid race conditions within an interrupt handler.
	ret


; sti activates interruptions and cli deactivates them

_sti:
	sti ;STI sets the interrupt flag (IF) in the EFLAGS register. This allows the processor to respond to maskable hardware interrupts.
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara of 16 bits
    out	0A1h,al
    pop     rbp
    retn


;8254 Timer (Timer Tick)
_irq00Handler:
	irqHandlerMaster 0

;Keyboard
_irq01Handler:
	irqHandlerMaster 1

;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3

;Serial Port 1 and 3
_irq04Handler:
	irqHandlerMaster 4

;USB
_irq05Handler:
	irqHandlerMaster 5


;Zero Division Exception
_exception0Handler:
	exceptionHandler 0

; Invalid Operation Code
_exception6Handler:
    exceptionHandler 6


; Entonces lo unico que se realiza desde c es un llamado a las system Call que cafrga los parametros y hace int algo para acceder a este handelr de abajo

; A call is made to the system call from Userland, passing the parameters in order,
; as done in a 64 architecture, and with the number with which the call to this function
; is carried in the idt (int (call number)). Then in sysCallDistpacher.c (in kernel) it
; is done.
; They put as parameters the id of the syscalls and all the parameters that are needed for
; the system calls (then those are passed accordingly from the Userland when int xx is done).
; Dispatcher is responsible for making a switch with the parameter that would be the syscall id
; of the system call that was passed from Userland (with 64 architecture) and in each case a call
; is made to the functions that we believe (suitable for us in c) that are responsible for
; interacting with the kernel and performing the requested operations.

; So the only thing that is done from c is a call to the system call that changes the parameters
; and does int xx to access this handler below:

_sysCallHandler:
    pushStateNoA

	sub rsp, 56

	mov [rsp], r15
	mov [rsp+8], r14
	mov [rsp+16], r13
	mov [rsp+24], r12
	mov [rsp+32], r11
	mov [rsp+40], r10
	mov [rsp+48], r9
	mov [rsp+56], r8
	mov [rsp+64], rsi
	mov [rsp+72], rdi
	mov [rsp+80], rbp
	mov [rsp+88], rdx
	mov [rsp+96], rcx
	mov [rsp+104], rbx
	mov [rsp+112], rax

	mov rdi, rsp

	call sysCallHandler

	add rsp, 56
	popStateNoA
	iretq


haltcpu:
	cli
	hlt
	ret

; Returns key from current keyboard
getKeyPressed:
    push rbp
    mov rbp, rsp

    xor rax, rax
    in al, 60h  ; check if key number is returned

    mov rsp, rbp
    pop rbp
    ret

sound:
	push rbp
    mov rbp, rsp

    mov rax, 1193180
    xor rdx, rdx
    div rdi

    mov rcx, rax

    mov dx, 0x43
    mov al, 0xB6
    out dx, al

    mov dx, 0x42
    mov al, cl
    out dx, al

    sar rcx, 8
    mov al, cl
    out dx, al

    in al, 0x61
    mov bl, al
    or bl, 3
    cmp al, bl
    je .end
    mov al, bl
    out 0x61, al
.end:
    mov rsp, rbp
	pop rbp
    ret


noSnd:
    push rbp
    mov rbp, rsp

    in al, 0x61
    and al, 0xFC
    out 0x61, al

    mov rsp, rbp
    pop rbp
    ret

section .data
    userland equ 0x400000
    reg_rip dq 0

SECTION .bss
	aux resq 1
