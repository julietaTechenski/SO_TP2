#include "../include/help.h"

// Command names
#define CLEAR           "clear"
#define EXIT            "exit"
#define HELP            "help"
#define REGS            "regs"
#define SIZE            "size"
#define SNAKE           "snake"
#define TIME            "time"
#define EXCP_6          "ioexception"
#define EXCP_0          "zeroexception"

// Command functionalities messages
#define CLEAR_MSG		"Clears the shell screen.\n"
#define EXIT_MSG        "Causes normal process termination.\n"
#define HELP_MSG        "Explanation of command functionalities. Try 'help [command]' for specific information.\n"
#define REGS_MSG 	    "Shows the registers at the time the ALT key was pressed during the program.\n"
#define SIZE_MSG        "Use 'size +' to increase the font size and 'size -' to decrease it.\n"
#define SNAKE_MSG		"Game 'Snake'.\n"
#define TIME_MSG 	    "Prints the current time.\n"
#define EXCP_6_MSG		"Command to check the undefined instruction exception routine.\n"
#define EXCP_0_MSG		"Command to check the division by zero exception routine.\n"

#define COM_SIZE 9

char * comm[] = {CLEAR, EXIT, HELP, REGS, SIZE, SNAKE, TIME, EXCP_6,EXCP_0};

//================================ Function declarations ================================

void helpWithParams(char * command);
void helpWithoutParams();

//================================ Command HELP function ================================

void help(char * args[]){
    int flag = 0;   // If its 1 then the argument was a command
    setColor(192, 192, 192);

    if(args[0] != 0){
        // Checks the user inserted a valid argument
        for(int i=0; i < COM_SIZE;i++){
            if(strcmp(args[0], comm[i])){
                flag = 1;
                helpWithParams(args[0]);
            }
        }
        if(!flag){
            setColor(255, 51, 51);
            printf("\nNo command found. Please try command 'help' for more info.\n\n");
        }
    } else {    // If the user just inserted 'help'
        helpWithoutParams();
    }

    setColor(255, 255, 255);
}

//==================== Functions that print the command functionalities ====================

void printCLEAR(){
    setColor(0, 128, 128);
    printf("\n%s: ", CLEAR);
    setColor(192, 192, 192);
    printf(CLEAR_MSG);
    printf("\n");
}

void printEXIT(){
    setColor(0, 128, 128);
    printf("\n%s: ", EXIT);
    setColor(192, 192, 192);
    printf(EXIT_MSG);
    printf("\n");
}

void printHELP(){
    setColor(0, 128, 128);
    printf("\n%s: ", HELP);
    setColor(192, 192, 192);
    printf(HELP_MSG);
    printf("\n");
}

void printREGS(){
    setColor(0, 128, 128);
    printf("\n%s: ", REGS);
    setColor(192, 192, 192);
    printf(REGS_MSG);
    printf("\n");
}

void printSIZE(){
    setColor(0, 128, 128);
    printf("\n%s: ", SIZE);
    setColor(192, 192, 192);
    printf(SIZE_MSG);
    printf("\n");
}

void printSNAKE(){
    setColor(0, 128, 128);
    printf("\n%s: ", SNAKE);
    setColor(192, 192, 192);
    printf(SNAKE_MSG);
    printf("\n");
}

void printTIME(){
    setColor(0, 128, 128);
    printf("\n%s: ", TIME);
    setColor(192, 192, 192);
    printf(TIME_MSG);
    printf("\n");
}

void printEXCP_6(){
    setColor(0, 128, 128);
    printf("\n%s: ", EXCP_6);
    setColor(192, 192, 192);
    printf(EXCP_6_MSG);
    printf("\n");
}

void printEXCP_0(){
    setColor(0, 128, 128);
    printf("\n%s: ", EXCP_0);
    setColor(192, 192, 192);
    printf(EXCP_0_MSG);
    printf("\n");
}

//================================== Help functions ==================================

void helpWithoutParams(){
    printf("\n_______________________________________________________________\n\n");
    printf("These shell commands are defined internally. Type 'help' to see this list.\nType 'help <name>' to find out more about the specific function <name>.\n");
    printf("_______________________________________________________________\n");

    printCLEAR();
    printEXIT();
    printHELP();
    printREGS();
    printSIZE();
    printSNAKE();
    printTIME();
    printEXCP_6();
    printEXCP_0();

    printf("_______________________________________________________________\n\n");
}

void helpWithParams(char * command){
    if(strcmp(command, CLEAR)){
        printCLEAR();
    } else if(strcmp(command, EXIT)){
        printEXIT();
    } else if(strcmp(command, HELP)){
        printHELP();
    } else if(strcmp(command, REGS)){
        printREGS();
    } else if(strcmp(command, SIZE)){
        printSIZE();
    } else if(strcmp(command, SNAKE)){
        printSNAKE();
    } else if(strcmp(command, TIME)){
        printTIME();
    } else if(strcmp(command, EXCP_6)){
        printEXCP_6();
    } else if(strcmp(command, EXCP_0)){
        printEXCP_0();
    }
}
