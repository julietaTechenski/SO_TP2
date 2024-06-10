// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../include/pipe.h"

#define STDOUT 1
#define STDIN 0

#define NULL  ((void*)0)

void pipe_command(uint64_t argc, char* argv[]){
    void * pipefd[2];

    if(pipe(pipefd) == -1){
        //hubo error -> despues vemos el manejo
    }
    my_createProcess(argv[0], "p1", 0, NULL, 1);
    my_createProcess(argv[1],"p2", 0, NULL,1);
    dup(argv[0], STDOUT, pipefd[1]);
    dup(argv[1], STDIN, pipefd[0]);
}