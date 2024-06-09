#include "../include/pipe.h"

#define STDOUT 1
#define STDIN 0

void pipe_command(uint64_t argc, char* argv[]){
    void * pipefd[2];

    if(pipe(pipefd) == -1){
        //hubo error -> despues vemos el manejo
    }
//    createProcess(p1, 0, NULL)
//    createProcess(p2, 0, NULL)
    dup(argv[0], STDOUT, pipefd[1]);
    dup(argv[1], STDIN, pipefd[0]);
}