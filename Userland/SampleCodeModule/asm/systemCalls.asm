GLOBAL _getRegs        ; syscall 1
GLOBAL put_pixel       ; syscall 2
GLOBAL system_read     ; syscall 3
GLOBAL system_write    ; syscall 4
GLOBAL clear_screen    ; syscall 5
GLOBAL change_color    ; syscall 6
GLOBAL inc_size        ; syscall 7
GLOBAL dec_size        ; syscall 8
GLOBAL get_seconds_elapsed ; syscall 9
GLOBAL _sleep          ; syscall 10
GLOBAL _playSound      ; syscall 11
GLOBAL sys_writeXY     ; syscall 12
GLOBAL system_malloc   ; syscall 13
GLOBAL system_free     ; syscall 14
GLOBAL system_create_process ; syscall 15
GLOBAL system_getpid   ; syscall 16
GLOBAL system_kill     ; syscall 17
GLOBAL system_block    ; syscall 18
GLOBAL system_unblock  ; syscall 19
GLOBAL system_wait     ; syscall 20
GLOBAL system_yield    ; syscall 21
GLOBAL system_nice     ; syscall 22
GLOBAL system_sem_init ; syscall 23
GLOBAL system_sem_wait ; syscall 24
GLOBAL system_sem_post ; syscall 25
GLOBAL system_sem_close ; syscall 26
GLOBAL sys_mem_state   ; syscall 27
GLOBAL system_dup      ; syscall 28
GLOBAL system_print_processes ; syscall 29
GLOBAL system_pipe     ; syscall 30


GLOBAL getTimeRTC

section .text

%macro sysCall 1
    push rbp
    mov rbp, rsp

    push rbx
    push r12
    push r13
    push r15

    mov rax, %1
    int 80h

    pop r15
    pop r13
    pop r12
    pop rbx

    mov rsp, rbp
    pop rbp
    ret
%endmacro

_getRegs:
    sysCall 1

put_pixel:
    sysCall 2

system_read:
   sysCall 3

system_write:
    sysCall 4

clear_screen:
    sysCall 5

change_color:
    sysCall 6

inc_size:
    sysCall 7

dec_size:
    sysCall 8

get_seconds_elapsed:
    sysCall 9

_sleep:
    sysCall 10

_playSound:
    sysCall 11

sys_writeXY:
    sysCall 12

system_malloc:
    sysCall 13

system_free:
    sysCall 14

system_create_process:
    sysCall 15

system_getpid:
    sysCall 16

system_kill:
    sysCall 17

system_block:
    sysCall 18

system_unblock:
    sysCall 19

system_wait:
    sysCall 20

system_yield:
    sysCall 21

system_nice:
    sysCall 22

system_sem_init:
    sysCall 23

system_sem_wait:
    sysCall 24

system_sem_post:
    sysCall 25

system_sem_close:
    sysCall 26

sys_mem_state:
    sysCall 27

system_dup:
    sysCall 28

system_print_processes:
    sysCall 29

system_pipe:
    sysCall 30

;-------------------------------------------------------
;   Recibe un  puntero a un struct typedef struct{
     ;                                  unsigned int segs;
     ;                                  unsigned int mins;
     ;                                  unsigned int hours;
     ;                             } TimeFormat;
;  Regresa el tiempo actual en un puntero a estructura
;-------------------------------------------------------
getTimeRTC:
	push rbp
	mov rbp, rsp

	push rax
	push rcx
	push rdx
;
	;	Antes de poder leer la hora, debo saber si hay una actualizacion en progreso o no
.updateCheck:
	mov rax, 0Ah
	out 70h, al
	in al, 71h
	shr al, 7				; El bit de upgrade in progress es el 7mo
	cmp al, 0
	jne .updateCheck		; Si el bit era 1, debo controlar de nuevo

	;	En al se guarda el dato en formato DDDUUU con D = decena y U = unidad
.seconds:
	mov rax, 0
	out 70h, al
	in al, 71h

    mov rcx, 0
	mov cl, al              ; me guardoo una copia

	and al, 240				; En "al" queda la decena
	shr al, 4

    mov rdx, 0
    mov dl, cl
	and cl, 15
	mov dl, 10
	mul dl
					; eax*=edx
	add al, cl				; En "cl" queda la unidad					; Los sumo
	mov [rdi], eax	;	quedan los segundos en eax
.miuntes:
	mov rax, 2
    out 70h, al
    in al, 71h

    mov rcx, 0
    mov cl, al              ; me guardoo una copia

    and al, 240				; En "al" queda la decena
    shr al, 4

    mov rdx, 0
    and cl, 15
    mov dl, 10
    mul dl
    					; eax*=edx
    add al, cl				; En "cl" queda la unidad					; Los sumo
    mov [rdi+4], eax

.hours:
	mov rax, 4
    out 70h, al
    in al, 71h

   mov rcx, 0
   mov cl, al              ; me guardoo una copia

   and al, 240				; En "al" queda la decena
   shr al, 4

   mov rdx, 0
   mov dl, cl
   and cl, 15
   mov dl, 10
   mul dl
       					; eax*=edx
   add al, cl
        					; eax*=edx
   mov  [rdi+8], eax

	pop rdx
	pop rcx
	pop rax

	mov rsp, rbp
	pop rbp
	ret
