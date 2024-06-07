#include "include/sampleCodeModule.h"

void hola(){
    for (int i = 0; i < 200; ++i) {
        printf("hola");
    }
}

void chau(){
    for (int i = 0; i < 200; ++i) {
        printf("chau");
    }
}

int main() {
    my_createProcess(hola, "hola",0,0);
    my_createProcess(chau, "chau",0,0);
	return 0;
}