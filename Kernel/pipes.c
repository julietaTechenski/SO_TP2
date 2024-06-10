#include "include/pipes.h"

int pipe(void* pipesfd[2]){
    char* aux = pipesfd[0] = pipesfd[1] = (char*)mm_alloc(sizeof(char)*128);

    if(pipesfd[0] == NULL){
        return -1;
    }

    for(int i=0; i < 128; i++, aux++){  // set memory space to '\0' to avoid garbage readings
        aux[i] = '\0';
    }
    return 1;
}

int dup(int oldfd, void* pipedir){
    if(oldfd != 0 && oldfd !=1)
        return -1;
    uint64_t ppid = getPID();
    PCB *p = findProcess(ppid);
    p->fd[oldfd] = pipedir;
    return 1;
}

/**
* typedef struct PCB {
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
    void * fd[2];   // inicializados en NULL
} PCB;
*/