// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../include/zeroexception.h"

//================================ Command ZEROEXCEPTION function ================================

void zeroexception(uint64_t argc,char *args[]){
    int i = 5;
    i /= 0;
    return;
}