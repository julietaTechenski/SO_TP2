#include "include/sampleCodeModule.h"

void test(){
    printf("test process creation");
}

int main() {
    char *argvAux[] = {0};
    my_createProcess(&shell, "shell", 0, argvAux);
    my_createProcess(&test, "test", 0, argvAux);
    return 0;
}