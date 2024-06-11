// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/sampleCodeModule.h"


int main() {
    char *argvAux[] = {0};
    void* fd[2];
    fd[0] = NULL;
    fd[1] = NULL;
    my_createProcess(&shell, "sh", 0, argvAux, 1, (void**)fd);
    return 0;
}