#ifndef SO_TP2_PIPES_H
#define SO_TP2_PIPES_H

#include "memory_manager.h"

typedef struct PCB {
    char name[MAX_NAME_LENGTH];
    uint64_t pid;
    int64_t priority;
    uint64_t* rsp;   //stack
    uint64_t* rbp;   //base pointer
    char isForeground;
    State state;
    int64_t timesRunning;
    struct PCB *prev;
    struct PCB *next;

} PCB;

int pipe(void * pipesfd[2]);
int dup(void* p, int oldfd, void* pipedir);

#endif //SO_TP2_PIPES_H
