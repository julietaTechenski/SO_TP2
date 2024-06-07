#include "sampleCodeModule.h"

int main() {
    char *argvAux[] = {0};
    printf("in sample code\n");
    createProcess(&shell, "shell", 0, argvAux);
    printf("created\n");
	return 0;
}