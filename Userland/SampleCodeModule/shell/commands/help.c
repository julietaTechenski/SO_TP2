// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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
#define UNBLOCK         "unblock"
#define WC              "wc"
#define EXCP_6          "ioexception"
#define EXCP_0          "zeroexception"


// test commands
#define TMM             "test_mm"
#define TPRIO           "test_prio"
#define TSYNC           "test_sync"
#define TPROC           "test_proc"


// Command functionalities messages
#define BLOCK_MSG		"Modifies process's state from READY to BLOCKED.\n"
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
#define UNBLOCK_MSG		"Modifies process's state from BLOCKED to READY.\n"
#define WC_MSG		    "Prints newline count from input.\n"
#define EXCP_6_MSG		"Command to check the undefined instruction exception routine.\n"
#define EXCP_0_MSG		"Command to check the division by zero exception routine.\n"

// test messages
#define TMM_MSG         "Use 'test_mm <size in mb>' to test the MMU\n"
#define TPRIO_MSG       "Command to display priority scheduling funtionality\n"
#define TSYNC_MSG       "Use 'test_sync <inc> <use_sem>', with use_sem a boolean to indicate whether to use semaphores and inc to indicate the desired increments\n"
#define TPROC_MSG       "Use 'test_proc <process amount>' to display the create, block, unblock, kill, nice and yield functionalities\n "

#define COM_SIZE 20

char * comm[] = {BLOCK, CAT, CLEAR, EXIT, FILTER, HELP, KILL, LOOP,
                 MEM,NICE, PHYLO, PS, REGS, SIZE, SNAKE, TIME,
                 UNBLOCK, WC, EXCP_6, EXCP_0};
char * msg[] = {BLOCK_MSG, CAT_MSG, CLEAR_MSG, EXIT_MSG, FILTER_MSG, HELP_MSG, KILL_MSG, LOOP_MSG,
                MEM_MSG,NICE_MSG, PHYLO_MSG, PS_MSG, REGS_MSG, SIZE_MSG, SNAKE_MSG, TIME_MSG,
                UNBLOCK_MSG, WC_MSG, EXCP_6_MSG, EXCP_0_MSG};

#define tcm_size 4


char* tcm[] = {TMM, TPRIO, TPROC, TSYNC};
char* tmsg[] = {TMM_MSG, TPRIO_MSG, TPROC_MSG, TSYNC_MSG};

//================================ Function declarations ================================

void helpWithParams(char * command);
void helpWithoutParams();

//================================ Command HELP function ================================

void help(uint64_t argc,char * args[]){
    decSize();
    setColor(192, 192, 192);

    if(argc != 0){
        // Checks the user inserted a valid argument
        for(int i=0; i < COM_SIZE;i++){
            if(strcmp(args[0], comm[i]) > 0){
                incSize();
                helpWithParams(args[0]);
                return;
            }
        }
        setColor(255, 51, 51);
        printf("\nNo command found. Please try command 'help' for more info.\n\n");

    } else {    // If the user just inserted 'help'
        helpWithoutParams();
    }

    setColor(255, 255, 255);
    incSize();
}

//==================== Functions that print the command functionalities ====================

void printCOMMAND(char * com, char * msg) {
    setColor(0, 128, 128);
    printf("\n%s: ", com);
    setColor(192, 192, 192);
    printf("%s", msg);
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
    printf("\n_______________________________________________________________\n\n");

    printf("Commands for testing.\n");
    printf("\n_______________________________________________________________\n\n");

    for(int i=0; i < tcm_size;i++) {
        printCOMMAND(tcm[i], tmsg[i]);
    }

    printf("_______________________________________________________________\n\n");
}

void helpWithParams(char * command){
    if(strcmp(command, BLOCK) > 0){
        printCOMMAND(BLOCK, BLOCK_MSG);
    } else if(strcmp(command, CAT) > 0){
        printCOMMAND(CAT, CAT_MSG);
    } else if(strcmp(command, CLEAR) > 0){
        printCOMMAND(CLEAR, CLEAR_MSG);
    } else if(strcmp(command, EXIT) > 0){
        printCOMMAND(EXIT, EXIT_MSG);
    } else if(strcmp(command, FILTER) > 0){
        printCOMMAND(FILTER, FILTER_MSG);
    } else if(strcmp(command, HELP) > 0){
        printCOMMAND(HELP, HELP_MSG);
    } else if(strcmp(command, KILL) > 0){
        printCOMMAND(KILL, KILL_MSG);
    } else if(strcmp(command, LOOP) > 0){
        printCOMMAND(LOOP, LOOP_MSG);
    } else if(strcmp(command, MEM) > 0){
        printCOMMAND(MEM, MEM_MSG);
    } else if(strcmp(command, NICE) > 0){
        printCOMMAND(NICE, NICE_MSG);
    } else if(strcmp(command, PHYLO) > 0){
        printCOMMAND(PHYLO, PHYLO_MSG);
    } else if(strcmp(command, PS) > 0){
        printCOMMAND(PS, PS_MSG);
    } else if(strcmp(command, REGS) > 0){
        printCOMMAND(REGS, REGS_MSG);
    } else if(strcmp(command, SIZE) > 0){
        printCOMMAND(SIZE, SIZE_MSG);
    } else if(strcmp(command, SNAKE) > 0){
        printCOMMAND(SNAKE, SNAKE_MSG);
    } else if(strcmp(command, TIME) > 0){
        printCOMMAND(TIME, TIME_MSG);
    } else if(strcmp(command, UNBLOCK) > 0){
        printCOMMAND(UNBLOCK, UNBLOCK_MSG);
    } else if(strcmp(command, WC) > 0){
        printCOMMAND(WC, WC_MSG);
    } else if(strcmp(command, EXCP_6) > 0){
        printCOMMAND(EXCP_6, EXCP_6_MSG);
    } else if(strcmp(command, EXCP_0) > 0) {
        printCOMMAND(EXCP_0, EXCP_0_MSG);
    } else if(strcmp(command, TMM) > 0) {
        printCOMMAND(TMM, TMM_MSG);
    } else if(strcmp(command, TPRIO) > 0) {
        printCOMMAND(TPRIO, TPRIO_MSG);
    } else if(strcmp(command, TPROC) > 0) {
        printCOMMAND(TPROC, TPROC_MSG);
    } else if(strcmp(command, TSYNC) > 0) {
        printCOMMAND(TSYNC, TSYNC_MSG);
    } else {
        printf("%s: command not found\n", command);
    }
}
