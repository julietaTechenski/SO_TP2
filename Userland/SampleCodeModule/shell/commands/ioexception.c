#include "../include/ioexception.h"

//================================ Command IOEXCEPTION function ================================

void ioexception(uint64_t argc,char *args[]){
    __asm__("ud2");  // Execute an undefined instruction (UD2)
}