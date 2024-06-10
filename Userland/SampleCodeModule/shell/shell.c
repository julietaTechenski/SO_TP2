// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/shell.h"

#define NULL ((void*)0)
#define MAX_ARGS 4
#define MAX_ARG_LENGTH 16
#define MAX_SIZE 100

//============================== Function Declarations ==============================

void getCommand(char buffer[]);
void playNotes();
void intro();

//================================= Variables  =================================

static char * buffer;
static char username[USERNAME_MAX_SIZE];

tcommand commands[] = {
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



void shell() {
    //Into
    intro();
    //Username
    int c;
    setColor(141, 132, 255);
    printf("\nInsert username: ");
    c = read(0, username, USERNAME_MAX_SIZE);
    if(c > 0 && c < USERNAME_MAX_SIZE){
        username[c] = '\0';
    }
    clearScreen();

    //Start of terminal
    setColor(255, 153, 102);
    printf("\n\nWelcome %s!\n\nThe 'help' command to display a menu with all the functionalities.\n\n", username);

    buffer = (char*) malloc(sizeof(char)*128);
    //terminal
    while(1) {
        setColor(255,255,255);
        printf("%s:$>", username);
        c = read(0, buffer, MAX_SIZE);
        if(strcmp(buffer, "this project is wrong") > 0){
            exitProgram(0, NULL);
        }
        buffer[c] = '\0';
        getCommand(buffer);
    }

}

//====================================== Functions ======================================

//Obtains the command inserted by the user
void getCommand(char* buffer) {
    //turns possible command to string
    char command[MAX_ARG_LENGTH];
    char * args[MAX_ARGS];

    for (int i = 0; i < MAX_ARGS; ++i){
        args[i] = (char*) malloc(MAX_ARG_LENGTH);
    }

    int argsAmount = 0;
    int isForeground1 = 1;
    int isForeground2 = 1;
    int index = 0;

    while(buffer[index] != ' ' && buffer[index] != '\0'){
        command[index] = buffer[index];
        index++;
    }
    command[index] = '\0';

    if(buffer[index] == ' ') {
        index++;
        if (buffer[index] == '&') { //is process in background?
            isForeground1 = 0;
            index++;
        }
        index--;
    }

    while (buffer[index] == ' ') index++;

    // arguments
    while (buffer[index] != '\0') {
        int k = 0;
        while (buffer[index] != ' ' && buffer[index] != '\0' && k < MAX_ARG_LENGTH - 1) {
            args[argsAmount][k++] = buffer[index++];
        }
        args[argsAmount][k] = '\0';

        if (k > 0) argsAmount++;

        // Skip any extra spaces
        while (buffer[index] == ' ') index++;
    }

    //If there was no command inserted
    if(command[0] == '\0')
        return;

    int cfound = 0;
    int n = 0;
    //Finds command
    for (; n < AMOUNT_COMMANDS && !cfound; n++) {
        if (strcmp(commands[n].name, command) > 0) {
            cfound = 1;
            if(strcmp(args[0],"|") == 0){
                int64_t pid = my_createProcess(commands[n].fn, commands[n].name, argsAmount, args, isForeground1);
                if(isForeground1)
                    wait(pid);
            }
        }
    }
    if(cfound && strcmp(args[0],"|") > 0){
        char *pArgs[2];
        pArgs[0] = commands[n].fn;
        int i = 0;
        int pfound = 0;
        for(;!pfound && i < AMOUNT_COMMANDS; i++){
            if (strcmp(args[1], commands[i].name) > 0) {
                pArgs[1] = commands[i].fn;
                pfound = 1;
            }
        }
        if(pfound){
            int64_t pid = my_createProcess(&pipe_command, "pipe",2, pArgs, isForeground2);
            wait(pid);
        }else {
            setColor(255, 51, 51);
            printf("%s: not a command. Valid pipe entry: 'p1 | p2' \n",args[1]);
        }
    }
    // reset values


    for(int i = 0; i < MAX_ARGS; i++)
        free((void*)args[i]);

    // if not a valid entry
    if(!cfound){
        setColor(255, 51, 51);
        printf("%s: command not found\n", command);
    }
    return;
}
