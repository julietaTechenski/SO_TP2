
#ifndef IDTLOADER_H
#define IDTLOADER_H

#include <stdint.h>
#include "defs.h"
#include "interrupts.h"


void load_idt();
void setup_IDT_entry (int index, uint64_t offset);


#endif //IDTLOADER_H
