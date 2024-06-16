// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/pipes.h"


static int pipepos_w = 0;
static int pipepos_r = 0;

static char* pipe_mutex = "pipe_mutex";

static int first_write = 1;


int pipe(void* pipesfd[2]){
    char* aux = pipesfd[0] = pipesfd[1] = (char*) mm_alloc(sizeof(char)*128);

    if(pipesfd[0] == NULL){
        return -1;
    }

    for(int i=0; i < 128; i++)  // set memory space to '\0' to avoid garbage readings
        aux[pipepos_w] = '\0';

    return 1;
}

int dup(int pid,int oldfd, void* pipedir){
    if(oldfd != 0 && oldfd !=1)
        return -1;
    PCB *p = findProcess(pid);
    p->fd[oldfd] = pipedir;
    return 1;
}



int writePipe(char * pipe,char * string, int count){
    char * countSem = "counter_pipe";

    my_sem_open(countSem, 0);

    int i = 0;
    while(i < count){
        pipe[pipepos_w] = string[i];
        pipepos_w = (pipepos_w +1) % 128;
        i++;
        my_sem_post(countSem);
    }
}

int readPipe(char* pipe, char * buffer, int count){
    char * countSem = "counter_pipe";

    my_sem_open(countSem, 0);

    int  i = 0;
    while(i < count){
        my_sem_wait(countSem);
        buffer[i] = pipe[pipepos_r];
        pipepos_r = (pipepos_r+1)%128;
        i++;
    }
    return count;
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