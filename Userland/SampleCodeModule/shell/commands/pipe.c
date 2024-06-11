// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../include/pipe.h"

#define STDOUT 1
#define STDIN 0

#define NULL  ((void*)0)

#define AMOUNT_COMMANDS 24



typedef struct tcommand{
    char * name;                //command
    void * fn;  //function
}tcommand;

tcommand pipe_commands[] = {
        {"size", &size},
        {"help", &help},
        {"time", &printTime},
        {"snake", &play},
        {"clear", &clear_screen}, //wrapper?
        {"mem", &mem_state}, //wrapper?
        {"exit", &exitProgram},
        {"cat", &cat}, //wrapper?
        {"zeroexception", &zeroexception},
        {"ioexception", &ioexception},
        {"regs", &getRegs},  //wrapper?
        {"test_mm", &test_mm},
        {"test_processes", &test_processes},
        {"test_prio", &test_prio},
        {"test_sync", &test_sync},
        {"ps", &print_processes},
        {"loop", &loop},
        {"nice", &nice},
        {"kill",&kill},
        {"block", &block},
        {"cat", &cat}, //wrapper?
        {"wc", &wc},  //wrapper?
        {"filter", &filter}, //wrapper?
        {"phylo", &phylos} //wrapper?

};


void pipe_command(uint64_t argc, char* argv[]){
    char * aux_args[2];
    aux_args[0] = (char*) malloc(sizeof(char)*20);
    aux_args[1] = (char*) malloc(sizeof(char)*20);

    strcpy(aux_args[0], argv[0]);
    strcpy(aux_args[1], argv[1]);

    void * pipefd[2];


    int p1_found = 0;
    int p2_found = 0;

    int64_t pid1 = 0;
    int64_t pid2 = 0;

    void* fd1[2];
    fd1[0] = NULL;

    void* fd2[2];
    fd2[1] = NULL;

    if(pipe(pipefd) == -1){
        printf("Error creating pipe\n");
    } else {
        fd1[1] = pipefd[1];
        fd2[0] = pipefd[0];

        for(int i =0; i < AMOUNT_COMMANDS; i++){
            if(strcmp(pipe_commands[i].name, aux_args[0])>0 && !p1_found){
                pid1 = my_createProcess(pipe_commands[i].fn, "p1", 0, NULL, 1, fd1);
                p1_found = 1;
            }
            if(strcmp(pipe_commands[i].name, aux_args[1])>0 && !p2_found){
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