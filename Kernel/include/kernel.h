#ifndef KERNEL_H
#define KERNEL_H
#include "lib.h"
#include "moduleLoader.h"
#include "videoDriver.h"
#include "idtLoader.h"

typedef struct MemoryManagerCDT {
    uint8_t *nextAddress;
} MemoryManagerCDT;

void clearBSS(void * bssAddress, uint64_t bssSize);
void * getStackBase();
void * initializeKernelBinary();
void mm_init();
void * mm_malloc(uint64_t size);

#endif //KERNEL_H
