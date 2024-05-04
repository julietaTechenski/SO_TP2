#include "../include/ioexception.h"

//================================ Command IOEXCEPTION function ================================

void ioexception(char *args[]){
    __asm__("ud2");  // Execute an undefined instruction (UD2)
}