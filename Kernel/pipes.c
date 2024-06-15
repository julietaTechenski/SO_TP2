// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/pipes.h"


static int pipepos_w = 0;
static int pipepos_r = 0;

static char* pipe_mutex = "pipe_mutex";

static int first_write = 1;


int pipe(void* pipesfd[2]){
    char* aux = pipesfd[0] = pipesfd[1] = (char*)mm_alloc(sizeof(char)*128);

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
    char *rReady = "rReady";
    char *wReady = "wReady";

    my_sem_open(rReady, 0);
    my_sem_open(wReady, 0);

    int c = count;
    while (c > 0 && *string != 0 && *string != EOFILE) {
        while ( c > 0 && *string != 0 && pipepos_w < 128 && *string != EOFILE) {
            pipe[pipepos_w] = *string;
            string++;
            pipepos_w++;
            c--;
        }
        if(*string == EOFILE){
            return -1;
        }
        if (pipepos_w == 128) {
            pipepos_w = 0; // restart buffer pos counters
            my_sem_post(wReady);
            my_sem_wait(rReady);  // waiting buffer to be read
        }
    }
    my_sem_post(wReady);
}

int readPipe(char* pipe, char * buffer, int count){
    int  i = 0;
    char *rReady = "rReady";

    char *wReady = "wReady";

    my_sem_open(wReady, 0);
    my_sem_open(rReady, 0); // read => rReady = 1

    while(i < count){
        my_sem_wait(wReady);
        buffer[i] = pipe[pipepos_r++];

        if(buffer[i] == EOFILE){
            return -1;
        }
        if(pipepos_r == 128){
            pipepos_r = 0;
            my_sem_post(rReady);
            my_sem_wait(wReady);
        }
        i++;
    }




//
//    while (c > 0){
//        my_sem_wait(wReady); // waits for input
//        while(pipepos_r < pipepos_w && pipe[pipepos_r] != 0){
//            *buffer = pipe[pipepos_r];
//            pipepos_r++;
//            buffer++;
//            c--;
//        }
//
////        while(c > 0 && pipe[pipepos_r] != 0 && pipe[pipepos_r] != EOFILE && pipepos_r < 128) {
////            buffer[pipepos_r] = pipe[pipepos_r];
////            pipepos_r++;
////            c--;
////        }
////        if(pipe[pipepos_r] == EOFILE){
////            my_sem_post(pipe_mutex);
////            return -1;
////        }
////        if (pipepos_r == 128) {
////            pipepos_r = 0;
////            my_sem_post(pipe_mutex);
////            my_sem_post(rReady);
////            my_sem_wait(wReady);
////        }
//
//    }
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