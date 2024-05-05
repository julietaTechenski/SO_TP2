
#include "include/lib.h"
#include "include/moduleLoader.h"


static void loadModule(uint8_t ** module, void * targetModuleAddress);
static uint32_t readUint32(uint8_t ** address);

void* loadModules(void * payloadStart, void ** targetModuleAddress)
{
	int i;

	uint8_t * currentModule = (uint8_t*)payloadStart;
	uint32_t moduleCount = readUint32(&currentModule);

    void * endOfModules = currentModule;
	for (i = 0; i < moduleCount; i++)
		endOfModules+= loadModule(&currentModule, targetModuleAddress[i]);

    return endOfModules;
}

static uint32_t loadModule(uint8_t ** module, void * targetModuleAddress)
{
	uint32_t moduleSize = readUint32(module);

	memcpy(targetModuleAddress, *module, moduleSize);
	*module += moduleSize;
    return moduleSize+sizeof(uint32_t);

}

static uint32_t readUint32(uint8_t ** address)
{
	uint32_t result = *(uint32_t*)(*address);
	*address += sizeof(uint32_t);
	return result;
}
