#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include "../include/videoDriver.h"
#include "../include/time.h"


void exceptionDispatcher(int exception, uint64_t infoRegs);
static void zero_division();
static void invalid_opcode();


#endif //EXCEPTIONS_H
