#include "../include/pipe.h"



void pipe_command(void * p1, void * p2){
    void * pipefd[2];

    if(pipe(pipefd) == -1){
        //hubo error -> despues vemos el manejo
    }
//    createProcess(p1, 0, NULL)
//    createProcess(p2, 0, NULL)
    dup(p1, STDOUT, pipefd[1]);
    dup(p2, STDIN, pipefd[0]);
}