#include "include/sampleCodeModule.h"


int main() {
    char *argvAux[] = {0};
    my_createProcess(&shell, "shell", 0, argvAux, 1);
    return 0;
}