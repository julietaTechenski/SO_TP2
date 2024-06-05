#include "include/pipes.h"

int pipe(void * pipesfd[2]){
    pipesfd[0] = pipesfd[1] = mm_alloc(sizeof(char)*128);
    if(pipesfd[0] == NULL){
        return -1;
    }
    return 1;
}

int dup(int oldfd, void* pipedir){
    uint64_t ppid = getpid();
    PCB *p = getprocess(ppid);
    p->fds[oldfd] = pipedir;
    return 1;
}