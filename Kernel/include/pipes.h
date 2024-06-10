#ifndef SO_TP2_PIPES_H
#define SO_TP2_PIPES_H

#include "memory_manager.h"
#include "semaphore.h"
#include "process.h"

typedef struct fd {
    int fd;
    void * dir;
} fd;

int pipe(void * pipesfd[2]);
int dup(int oldfd, void* pipedir);

#endif //SO_TP2_PIPES_H