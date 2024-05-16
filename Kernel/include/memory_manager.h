//
// Created by Usuario on 15/5/2024.
//

#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stddef.h>
#include <stdint.h>

void mm_init(void * ptr, size_t max_size);
void * mm_alloc(size_t size);
void mm_free(void* ptr);

#endif // MEMORY_MANAGER_H