// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/pipes.h"

#define SEM "counter_pipe"
#define AVAILABLE "available"
#define MAX_SIZE 1024

static int pipepos_w;
static int pipepos_r;
static char closed;


int pipe(void* pipesfd[2]) {
    closed = 0;
    pipepos_w = 0;
    pipepos_r= 0;

    my_sem_close(SEM);
    my_sem_close(AVAILABLE);
    my_sem_open(SEM, 0);
    my_sem_open(AVAILABLE, MAX_SIZE-1);

    char* aux = pipesfd[0] = pipesfd[1] = (char*) mm_alloc(sizeof(char)*MAX_SIZE);

    if(pipesfd[0] == NULL){
        return -1;
    }

    memset(aux, 0, MAX_SIZE);

    return 1;
}

int dup(int pid,int oldfd, void* pipedir){
    if(oldfd != 0 && oldfd !=1)
        return -1;
    PCB *p = findProcess(pid);
    p->fd[oldfd] = pipedir;
    return 1;
}

void closePipe(){
    closed = 1;
    my_sem_post(SEM); // in case someone is blocked in sem, trying to read from pipe
}

int writePipe(char * pipe,char * string, int count){
    if(closed)
        return 0;
    int i = 0;
    while(i < count){
        my_sem_wait(AVAILABLE);
        pipe[pipepos_w] = string[i];
        pipepos_w = (pipepos_w +1) % MAX_SIZE;
        i++;
        my_sem_post(SEM);
    }
}

int readPipe(char* pipe, char * buffer, int count){
    if(closed && pipepos_r==pipepos_w)
        return 0;

    int  i = 0;
    while(i < count && (pipepos_r != pipepos_w || i==0)){
        my_sem_wait(SEM);
        if(pipepos_w == pipepos_r)
            return i;
        buffer[i] = pipe[pipepos_r];
        pipepos_r = (pipepos_r+1)%MAX_SIZE;
        i++;
        my_sem_post(AVAILABLE);
    }
    return i;
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