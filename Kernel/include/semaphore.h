#ifndef SO1_SEMAPHORE_H
#define SO1_SEMAPHORE_H

#include <stdint.h>
#include <string.h>
#include "memory_manager.h"

#define MAX_LENGTH_NAME 255

extern void releaseLock(uint32_t* lock);
extern void acquireLock(uint32_t* lock);

int64_t my_sem_open(char *sem_id, uint64_t initialValue);
int64_t my_sem_wait(char *sem_id);
int64_t my_sem_post(char *sem_id);
int64_t my_sem_close(char *sem_id);

#endif //SO1_SEMAPHORE_H
