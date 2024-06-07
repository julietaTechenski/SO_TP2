#include "include/sampleCodeModule.h"

void test(){
    printf("test process creation\n");
}

int main() {
    char *argvAux[] = {0};
    my_createProcess(&shell, "shell", 0, argvAux, 1);
    my_createProcess(&test, "test", 0, argvAux, 1);
    return 0;
}