// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "process.h"
#include "keyboardDriver.h"

static void int_21(uint64_t infoRegs);

void irqDispatcher(uint64_t irq, uint64_t infoRegs) {
	switch (irq) {
        case 1:
            int_21(infoRegs);
            break;
	}
	return;
}


void int_21(uint64_t infoRegs){
    keyboard_handler(infoRegs);
}
