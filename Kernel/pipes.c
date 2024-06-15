// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/pipes.h"

int pipe(void* pipesfd[2]){
    char* aux = pipesfd[0] = pipesfd[1] = (char*)mm_alloc(sizeof(char)*128);

    if(pipesfd[0] == NULL){
        return -1;
    }

    for(int i=0; i < 128; i++)  // set memory space to '\0' to avoid garbage readings
        aux[i] = '\0';

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
    char *rReady = "rReady";
    my_sem_open(rReady, 0);

    char *wReady = "wReady";
    my_sem_open(wReady, 0);

    int i = 0;
    int c = count;
    while (c > 0 && string[i] != 0 && string[i] == EOFILE) {
        while ( c > 0 && string[i] != 0 && i < 128 && string[i] != EOFILE) {
            pipe[i] = string[i];
            i++;
            c--;
        }
        if (i == 128) {
            i = 0; // restart buffer pos counters
            my_sem_post(wReady);
            my_sem_wait(rReady);  // waiting buffer to be read
        }
    }
    my_sem_post(wReady);
}

int readPipe(char* pipe, char * buffer, int count){
    int c = count;
    char *rReady = "rReady";
    my_sem_open(rReady, 0); // read => rReady = 1

    char *wReady = "wReady";
    my_sem_open(wReady, 0);

    my_sem_wait(wReady); // waits for input

    int i = 0;
    while (c > 0 && pipe[i] != 0) {
        while(c > 0 && pipe[i] != 0 && pipe[i] != EOFILE && i < 128) {
            buffer[i] = pipe[i];
            c--;
            i++;
        }
        if(pipe[i] == EOFILE)
            return -1;
        if (i == 128) {
            i = 0;
            my_sem_post(rReady);
            my_sem_wait(wReady);
        }

    }
    my_sem_post(rReady);
    return c;
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