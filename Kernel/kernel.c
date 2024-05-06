#include "include/kernel.h"
#define MM_SIZE 1024

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

uint8_t * endOfModules;
MemoryManagerCDT * memoryManager;


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

	endOfModules = loadModules(&endOfKernelBinary, moduleAddresses);
    endOfModules = (endOfModules + 0xFFF) & ~0xFFF;     //alignment
    mm_init();

	clearBSS(&bss, &endOfKernel - &bss);

	return getStackBase();
}

void mm_init(){
    memoryManager->nextAddress = endOfModules;
}

void * mm_malloc(uint64_t size){
    uint8_t *answer = memoryManager->nextAddress;   //give requested address
    memoryManager->nextAddress += size;             //increment current mem address
    return (void *) answer;
}

int main() {
    load_idt();
    ((EntryPoint)sampleCodeModuleAddress)();   //call to shell
    return 0;
}
