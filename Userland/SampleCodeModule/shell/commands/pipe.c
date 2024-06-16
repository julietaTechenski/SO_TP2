// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../include/pipe.h"

#define STDOUT 1
#define STDIN 0

#define NULL  ((void*)0)

#define AMOUNT_COMMANDS 26



typedef struct tcommand{
    char * name;                //command
    void * fn;  //function
}tcommand;

tcommand pipe_commands[] = {
        {"size", &size},
        {"help", &help},
        {"time", &printTime},
        {"snake", &play},
        {"clear", &clearScreen},
        {"mem", &mem_state},
        {"exit", &exitProgram},
        {"cat", &cat},
        {"zeroexception", &zeroexception},
        {"ioexception", &ioexception},
        {"regs", &getRegs},
        {"test_mm", &test_mm},
        {"test_processes", &test_processes},
        {"test_prio", &test_prio},
        {"test_sync", &test_sync},
        {"ps", &print_processes},
        {"loop", &loop},
        {"nice", &niceProcess},
        {"kill",&killProcess},
        {"block", &blockProcess},
        {"cat", &cat}, //wrapper?
        {"wc", &wc},  //wrapper?
        {"filter", &filter}, //wrapper?
        {"phylo", &phylosProcess}, //wrapper?
        {"unblock", &unblockProcess},
        {"hola", &hola}
};




void pipe_command(uint64_t argc, char* argv[]){
    if (argv == NULL || argc == 0 || argv[0] == NULL) {
        return;
    }

    char * aux_args[2];
    aux_args[0] = (char*) malloc(sizeof(char)*20);
    aux_args[1] = (char*) malloc(sizeof(char)*20);

    strcpy_n(aux_args[0], argv[0], 20);
    strcpy_n(aux_args[1], argv[1], 20);

    void * pipefd[2];

    int p1_found = 0;
    int p2_found = 0;

    int pid1 = 0;
    int pid2 = 0;

    void * fd1[2];
    fd1[0] = NULL;

    void * fd2[2];
    fd2[1] = NULL;


    if(pipe(pipefd) == -1){
        printf("Error creating pipe\n");
    } else {
        fd1[1] = pipefd[1];
        fd2[0] = pipefd[0];

        for(int i =0; i < AMOUNT_COMMANDS; i++){

            if(aux_args[0] != NULL && strcmp(pipe_commands[i].name, aux_args[0]) > 0 && !p1_found){
                pid1 = my_createProcess(pipe_commands[i].fn, "p1", 0, NULL, 1, fd1);
                p1_found = 1;
            }
            if(aux_args[1] != NULL && strcmp(pipe_commands[i].name, aux_args[1])>0 && !p2_found){
                pid2 = my_createProcess(pipe_commands[i].fn,"p2", 0, NULL,1, fd2);
                p2_found = 1;
            }
        }
        if(pid1 != 0 && pid2 != 0 ){
            wait(pid1);
            wait(pid2);
        }else {
            printf("Error in pipe process creation\n");
        }

    }
}