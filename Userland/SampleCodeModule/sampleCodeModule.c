// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/sampleCodeModule.h"


int main() {
    char *argvAux[] = {0};
    my_createProcess(&shell, "sh", 0, argvAux, 1);
    return 0;
}