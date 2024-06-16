#ifndef SO_TP2_PIPES_H
#define SO_TP2_PIPES_H

#include "memory_manager.h"
#include "semaphore.h"
#include "process.h"

#define EOFILE '\377'


typedef struct fd {
    int fd;
    void * dir;
} fd;

int pipe(void * pipesfd[2]);
int dup(int pid,int oldfd, void* pipedir);
int writePipe(char * pipe,char * string, int count);
int readPipe(char* pipe, char * buffer, int count);
void closePipe();

#endif //SO_TP2_PIPES_H
