#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stdint.h>
#define NULL ((void*)0)
typedef uintptr_t size_t;

void mm_init(void * ptr, size_t max_size);
void * mm_alloc(size_t size);
void mm_free(void* ptr);

#endif // MEMORY_MANAGER_H