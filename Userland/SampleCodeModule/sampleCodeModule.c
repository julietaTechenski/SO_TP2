#include "sampleCodeModule.h"

int main() {
    char *argvAux[] = {0};
    createProcess(&shell, "shell", 0, argvAux);
	return 0;
}