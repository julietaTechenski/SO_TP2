

#ifndef SYSCALLDISPATCHER_H
#define SYSCALLDISPATCHER_H

#include <stdint.h>
#include "videoDriver.h"
#include "keyboardDriver.h"
#include "time.h"
#include "soundDriver.h"
#include "kernel.h"

typedef struct{
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rbp;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;
    uint64_t rip;
}* Registers;

uint64_t sysCallHandler(Registers registers);

#endif //SYSCALLDISPATCHER_H

