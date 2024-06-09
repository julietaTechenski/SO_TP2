#include "include/shell.h"

#define NULL ((void*)0)
#define MAX_ARGS 4
#define MAX_ARG_LENGTH 16

//============================== Function Declarations ==============================

void getCommand(char buffer[]);
void playNotes();
void intro();

//================================= Variables  =================================

char buffer[MAX_SIZE];

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
        {"filter", &filter} //wrapper?

};



void shell() {
    //Into
    intro();
    //Username
    int c;
    setColor(141, 132, 255);
    printf("\nInsert username: ");
    c = read(0, username, USERNAME_MAX_SIZE);
    username[c] = '\0';
    clearScreen();

    //Start of terminal
    setColor(255, 153, 102);
    printf("\n\nWelcome %s!\n\nThe 'help' command to display a menu with all the functionalities.\n\n", username);

    //terminal
    while(1) {
        setColor(255,255,255);
        printf("%s:$>", username);
        c = read(0, buffer, MAX_SIZE);
        buffer[c] = 0;
        getCommand(buffer);
    }

}

//====================================== Functions ======================================

//Obtains the command inserted by the user
void getCommand(char* buffer) {
    //turns possible command to string
    char command[20];
    char args[MAX_ARGS][MAX_ARG_LENGTH];
    int argsAmount = 0;
    int isForeground = 1;
    int index = 0;

    while(buffer[index] != ' ' && buffer[index] != '\0'){
        command[index] = buffer[index];
        index++;
    }
    command[index] = '\0';

    while (buffer[index] == ' ') index++;

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

    //Finds command
    for (int n = 0; n < AMOUNT_COMMANDS && !cfound; n++) {
        if (strcmp(commands[n].name, command)) {
            cfound = 1;
            int64_t pid = my_createProcess(commands[n].fn, commands[n].name, argsAmount,  args, isForeground);
            if(isForeground){
                wait(pid);
            }
        }
    }
    //If not found, prints error message
    if(!cfound){
        setColor(255, 51, 51);
        printf("%s: command not found\n", command);
    }
    return;
}
