// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../include/ioexception.h"

//================================ Command IOEXCEPTION function ================================

void ioexception(uint64_t argc,char *args[]){
    __asm__("ud2");  // Execute an undefined instruction (UD2)
}