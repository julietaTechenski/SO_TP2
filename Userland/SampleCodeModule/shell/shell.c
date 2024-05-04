#include "include/shell.h"

//============================== Function Declarations ==============================

void getCommand(char buffer[]);
void playNotes();
void intro();

//================================= Variables  =================================

char buffer[MAX_SIZE];

static char username[USERNAME_MAX_SIZE];

tcommand commands[] = {
        {"size", size},
        {"help", help},
        {"time", printTime},
        {"snake", play},
        {"clear", clearScreen},
        {"exit", exitProgram},
        {"zeroexception", zeroexception},
        {"ioexception", ioexception},
        {"regs", getRegs}
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
void getCommand(char buffer[]) {
    //turns possible command to string
    char command[20];
    char * args[5] = {0};
    int i;
    for(i=0; buffer[i] != ' ' && buffer[i] != '\0'; i++){
        command[i] = buffer[i];
    }
    command[i] = '\0';

    if(buffer[i++]==' ') {
        //turns possible arg to string
        int j = 0;
        while (buffer[i] != '\0' && buffer[i] != ' ') {
            int k = 0;
            char aux[20] = {0};
            while (buffer[i] != ' ' && buffer[i] != '\0') {
                aux[k] = buffer[i];
                i++;
                k++;
            }
            aux[k] = 0;
            args[j] = aux;
            j++;
        }
    }

    //If there was no command inserted
    if(command[0] == '\0')
        return;

    int cfound = 0;

    //Finds command
    for (int n = 0; n < AMOUNT_COMMANDS && !cfound; n++) {
        if (strcmp(commands[n].name, command)) {
            cfound = 1;
            commands[n].fn(args);
        }
    }
    //If not found, prints error message
    if(!cfound){
        setColor(255, 51, 51);
        printf("%s: command not found\n", command);
    }
    return;
}
