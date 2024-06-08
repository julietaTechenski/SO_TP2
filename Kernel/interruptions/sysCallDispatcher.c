#include "../include/sysCallDispatcher.h"


static Color aux;

uint64_t sysCallHandler(Registers registers){
        switch (registers->rax) {
            RegAux  r;
            case 0:
                return 0;
            case 1:
                r.rip = registers->rip;
                r.rax = registers->rax;
                r.rsi = registers->rsi;
                r.rdi= registers->rdi;
                r.rdx = registers->rdx;
                r.rcx = registers->rcx;
                r.rbx = registers->rbx;
                r.rbp = registers->rbp;
                r.r8 = registers->r8;
                r.r9 = registers->r9;
                r.r10 = registers->r10;
                r.r11 = registers->r11;
                r.r12 = registers->r12;
                r.r13 = registers->r13;
                r.r14 = registers->r14;
                r.r15 = registers->r15;
                printRegisters(r);
                return 0;
            case 2:
                aux.red = (unsigned char) registers->rdi;
                aux.green = (unsigned char) registers->rsi;
                aux.blue = (unsigned char) registers->rdx;
                aux.red =  registers->rdi;
                aux.green =  registers->rsi;
                aux.blue =  registers->rdx;
                putPixel(aux, registers->rcx, registers->r8);
                return 0;
            case 3:
                return read(registers->rdi,(char *)registers->rsi,registers->rdx);
            case 4:
                return writeString(registers->rdi,(char *) registers->rsi,registers->rdx);
            case 5:
                clearScreen();
                return 0;
            case 6:
                aux.blue = registers->rdi;
                aux.green = registers->rsi;
                aux.red = registers->rdx;
                setForeGroundColor(aux);
                return 0;
            case 7:
                incSize();
                return 0;
            case 8:
                decSize();
                return 0;
            case 9:
                return seconds_elapsed();
            case 10:
                sleep(registers->rdi);
                return 0;
            case 11:
                playSound(registers->rdi, registers->rsi);
                return 0;
            case 12:
                return writeXY(registers->rdi, (char *) registers->rsi, registers->rdx, registers->rcx, registers->r8);
            case 13:
                return mm_alloc(registers->rdi);
            case 14:
                mm_free((void*)registers->rdi);
                break;
            case 15:
                return createProcess((void*)registers->rdi, (char *)registers->rsi, registers->rdx, (char **)registers->rcx, registers->r8);
            case 16:
                return getPID();
            case 17:
                return kill(registers->rdi);
            case 18:
                return block(registers->rdi);
            case 19:
                return unblock(registers->rdi);
            case 20:
                //wait
                return 0;
            case 21:
                yield();
                return 0;
            case 22:
                //nice
                return changePriority(registers->rdi, registers->rsi);
            case 23:
                //sem_init
                return 0;
            case 24:
                //sem_wait
                return 0;
            case 25:
                //sem_post
                return 0;
            case 26:
                //sem_close
                return 0;
            case 27:
                mm_state();
                return 0;
            //case 27: CAMBIAR NUMERO
            //    return 0/*pipe((void**)registers->rdi)*/;
            case 28:
                return 0/*dup((void *)registers->rdi, registers->rsi, (void*)registers->rdx)*/;
            case 29:
                return changeStatePID(registers->rdi, registers->rsi);
            case 30:
                printProcesses();
                return 0;
        }
        return 0;
    }

