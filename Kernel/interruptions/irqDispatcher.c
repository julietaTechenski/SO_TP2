#include "process.h"
#include "keyboardDriver.h"

static void int_20();
static void int_21(uint64_t infoRegs);

void irqDispatcher(uint64_t irq, uint64_t infoRegs) {
	switch (irq) {
		case 0:
			int_20();
			break;
        case 1:
            int_21(infoRegs);
            break;
	}
	return;
}

void int_20() {
    scheduler();
}

void int_21(uint64_t infoRegs){
    keyboard_handler(infoRegs);
}
