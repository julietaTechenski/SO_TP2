// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../include/idtLoader.h"


#pragma pack(push)		//Current lineup push
#pragma pack (1) 		//Align the following structures to 1 byte

//Interruption descriptor
typedef struct {
  uint16_t offset_l, selector;
  uint8_t cero, access;
  uint16_t offset_m;
  uint32_t offset_h, other_cero;
} DESCR_INT;

#pragma pack(pop)		//Resets current alignment

DESCR_INT * idt = (DESCR_INT *) 0;	// IDT of 255 entries

void load_idt() {

    setup_IDT_entry(0x00, (uint64_t)&_exception0Handler);  // divide by zero exception

    setup_IDT_entry(0x06, (uint64_t)&_exception6Handler);   // invalid opcode exception

    setup_IDT_entry (0x20, (uint64_t)&_irq00Handler);  // PIT interruption

    setup_IDT_entry(0x21, (uint64_t)&_irq01Handler);  // keyboard interruption

    setup_IDT_entry(0x80,(uint64_t)&_sysCallHandler);  // system calls
    picMasterMask(0xFC);
    picSlaveMask(0xFF);

}

//Builds every entry used
void setup_IDT_entry (int index, uint64_t offset) {
  idt[index].selector = 0x08;
  idt[index].offset_l = offset & 0xFFFF;
  idt[index].offset_m = (offset >> 16) & 0xFFFF;
  idt[index].offset_h = (offset >> 32) & 0xFFFFFFFF;
  idt[index].access = ACS_INT;
  idt[index].cero = 0;
  idt[index].other_cero = (uint64_t) 0;
}
