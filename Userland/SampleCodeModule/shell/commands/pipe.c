// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../include/pipe.h"

#define STDOUT 1
#define STDIN 0

#define NULL  ((void*)0)

void pipe_command(uint64_t argc, char* argv[]){
    void * pipefd[2];

    if(pipe(pipefd) == -1){
        printf("Error creating pipe\n");
    } else{
        int64_t pid1 = my_createProcess(argv[0], "p1", 0, NULL, 1);
        int64_t pid2 = my_createProcess(argv[1],"p2", 0, NULL,1);
        dup(pid1, STDOUT, pipefd[1]);
        dup(pid2, STDIN, pipefd[0]);
        wait(pid1);
        wait(pid2);
    }
}