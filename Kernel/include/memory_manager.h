//
// Created by Usuario on 15/5/2024.
//

#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stddef.h>
#include <stdint.h>

void memory_manager_init(void * ptr, size_t max_size);
void * memory_manager_alloc(size_t size);
void memory_manager_free(void* ptr);


typedef struct MemoryManagerCDT {
    uint8_t *nextAddress;
} MemoryManagerCDT;

#endif // MEMORY_MANAGER_H