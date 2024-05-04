#ifndef KERNEL_H
#define KERNEL_H
#include "lib.h"
#include "moduleLoader.h"
#include "videoDriver.h"
#include "idtLoader.h"

void clearBSS(void * bssAddress, uint64_t bssSize);
void * getStackBase();
void * initializeKernelBinary();


#endif //KERNEL_H
