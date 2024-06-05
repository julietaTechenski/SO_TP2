#include "../include/pipe.h"



void pipe_command(void * p1, void * p2){
    void * pipefd[2];

    if(pipe(pipefd) == -1){
        //hubo error -> despues vemos el manejo
    }
    dup(void * p1, STDOUT, pipefd[1]);
    dup(void * p2, STDIN, pipefd[0]);
}