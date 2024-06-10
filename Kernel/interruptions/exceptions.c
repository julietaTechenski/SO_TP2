// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../include/exceptions.h"

#define ZERO_EXCEPTION_ID 0
#define INVALID_OPERATION_CODE 6

void exceptionDispatcher(int exception, uint64_t infoRegs) {
    RegAux * aux = (RegAux *) infoRegs;     //modifies the registers format to print

    //prints corresponding exceptions message
    if (exception == ZERO_EXCEPTION_ID){
		zero_division();
    } else if (exception == INVALID_OPERATION_CODE) {
        invalid_opcode();
    }else{
        writeString(2, "EXCEPTION\n",10);
    }

    printRegisters(*aux);   //prints the registers

    writeString(2,"\nThe system will be restarting soon.\n",36);
    sleep(5000);    //time registers are printed before restarting
    clearScreen();
}

static void zero_division() {
	writeString(2, "\nZero Exception: division by zero.\nCurrent Registers:\n\n",55);
}

static void invalid_opcode(){
    writeString(2, "\nAn Invalid Upcode Exception was triggered.\nCurrent Registers:\n\n",64);
}