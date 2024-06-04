#include "include/kernel.h"

#define PAGE_SIZE 4096
#define MAX_MEM_SIZE 128*1024*1024

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

uint8_t * endOfModules;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;

typedef int (*EntryPoint)();


void clearBSS(void * bssAddress, uint64_t bssSize){
	memset(bssAddress, 0, bssSize);
}

void * getStackBase(){
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary(){
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};



	void * addr = loadModules(&endOfKernelBinary, moduleAddresses);
    clearBSS(&bss, &endOfKernel - &bss);

    addr = (void *)(((uintptr_t)addr + 0xFFF) & ~0xFFF);
    mm_init(addr, MAX_MEM_SIZE);

	return getStackBase();
}


int main() {
    load_idt();
    setFirstProcess(((EntryPoint)sampleCodeModuleAddress)());   //call to shell -> reemplazar por cargar a la shell como porceso y esperar la int de timeer tick
    return 0;
}
