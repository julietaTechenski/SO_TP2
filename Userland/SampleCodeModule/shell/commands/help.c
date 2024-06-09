#include "../include/help.h"

// Command names
#define BLOCK           "block"
#define CAT             "cat"
#define CLEAR           "clear"
#define EXIT            "exit"
#define FILTER           "filter"
#define HELP            "help"
#define KILL            "kill"
#define LOOP            "loop"
#define MEM             "mem"
#define NICE            "nice"
#define PHYLO            "phylo"
#define PS              "ps"
#define REGS            "regs"
#define SIZE            "size"
#define SNAKE           "snake"
#define TIME            "time"
#define WC              "wc"
#define EXCP_6          "ioexception"
#define EXCP_0          "zeroexception"


// Command functionalities messages
#define BLOCK_MSG		"Modifies process's state between BLOCKED and READY.\n"
#define CAT_MSG		    "Prints on the standard output what it receives from standard input.\n"
#define CLEAR_MSG		"Clears the shell screen.\n"
#define EXIT_MSG        "Causes normal process termination.\n"
#define FILTER_MSG		"Prints from input without vowels.\n"
#define HELP_MSG        "Explanation of command functionalities. Try 'help [command]' for specific information.\n"
#define KILL_MSG		"Kills a process given its ID.\n"
#define LOOP_MSG		"Prints its ID and a greeting every n amount of seconds.\n"
#define MEM_MSG		    "Prints memory state.\n"
#define NICE_MSG		"Changes process's priority given its ID and new priority.\nHighest priority: 0\nLowest priority: 10\n"
#define PHYLO_MSG		"Implementation of the Dining Philosophers Problem.\n\"A\" key: adds philosopher\n\"R\" key: removes philosopher\n"
#define PS_MSG		    "Prints all process's properties.\n"
#define REGS_MSG 	    "Shows the registers at the time the ALT key was pressed during the program.\n"
#define SIZE_MSG        "Use 'size +' to increase the font size and 'size -' to decrease it.\n"
#define SNAKE_MSG		"Game 'Snake'.\n"
#define TIME_MSG 	    "Prints the current time.\n"
#define WC_MSG		    "Prints newline count from input.\n"
#define EXCP_6_MSG		"Command to check the undefined instruction exception routine.\n"
#define EXCP_0_MSG		"Command to check the division by zero exception routine.\n"


#define COM_SIZE 19

char * comm[] = {BLOCK, CAT, CLEAR, EXIT, FILTER, HELP, KILL, LOOP,
                 MEM,NICE, PHYLO, PS, REGS, SIZE, SNAKE, TIME,
                 WC, EXCP_6, EXCP_0};
char * msg[] = {BLOCK_MSG, CAT_MSG, CLEAR_MSG, EXIT_MSG, FILTER_MSG, HELP_MSG, KILL_MSG, LOOP_MSG,
                MEM_MSG,NICE_MSG, PHYLO_MSG, PS_MSG, REGS_MSG, SIZE_MSG, SNAKE_MSG, TIME_MSG,
                WC_MSG, EXCP_6_MSG, EXCP_0_MSG};

//================================ Function declarations ================================

void helpWithParams(char * command);
void helpWithoutParams();

//================================ Command HELP function ================================

void help(uint64_t argc,char * args[]){
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

void printCOMMAND(char * com, char * msg) {
    setColor(0, 128, 128);
    printf("\n%s: ", com);
    setColor(192, 192, 192);
    printf(msg);
    printf("\n");
}

//================================== Help functions ==================================

void helpWithoutParams(){
    printf("\n_______________________________________________________________\n\n");
    printf("These shell commands are defined internally. Type 'help' to see this list.\nType 'help <name>' to find out more about the specific function <name>.\n");
    printf("_______________________________________________________________\n");

    for(int i=0; i < COM_SIZE;i++) {
        printCOMMAND(comm[i], msg[i]);
    }

    printf("_______________________________________________________________\n\n");
}

void helpWithParams(char * command){
    if(strcmp(command, BLOCK)){
        printCOMMAND(BLOCK, BLOCK_MSG);
    } else if(strcmp(command, CAT)){
        printCOMMAND(CAT, CAT_MSG);
    } else if(strcmp(command, CLEAR)){
        printCOMMAND(CLEAR, CLEAR_MSG);
    } else if(strcmp(command, EXIT)){
        printCOMMAND(EXIT, EXIT_MSG);
    } else if(strcmp(command, FILTER)){
        printCOMMAND(FILTER, FILTER_MSG);
    } else if(strcmp(command, HELP)){
        printCOMMAND(HELP, HELP_MSG);
    } else if(strcmp(command, KILL)){
        printCOMMAND(KILL, KILL_MSG);
    } else if(strcmp(command, LOOP)){
        printCOMMAND(LOOP, LOOP_MSG);
    } else if(strcmp(command, MEM)){
        printCOMMAND(MEM, MEM_MSG);
    } else if(strcmp(command, NICE)){
        printCOMMAND(NICE, NICE_MSG);
    } else if(strcmp(command, PHYLO)){
        printCOMMAND(PHYLO, PHYLO_MSG);
    } else if(strcmp(command, PS)){
        printCOMMAND(PS, PS_MSG);
    } else if(strcmp(command, REGS)){
        printCOMMAND(REGS, REGS_MSG);
    } else if(strcmp(command, SIZE)){
        printCOMMAND(SIZE, SIZE_MSG);
    } else if(strcmp(command, SNAKE)){
        printCOMMAND(SNAKE, SNAKE_MSG);
    } else if(strcmp(command, TIME)){
        printCOMMAND(TIME, TIME_MSG);
    } else if(strcmp(command, WC)){
        printCOMMAND(WC, WC_MSG);
    } else if(strcmp(command, EXCP_6)){
        printCOMMAND(EXCP_6, EXCP_6_MSG);
    } else if(strcmp(command, EXCP_0)){
        printCOMMAND(EXCP_0, EXCP_0_MSG);
    }
}
