#ifndef _PROCESSADT_H
#define _PROCESSADT_H

#include <stdint.h>
#include "lib.h"
#include "interrupts.h"
#include "memory_manager.h"
#include "videoDriver.h"
#include "time.h"

/*
 * Priority Guide:
 * There are 10 priorities in this project. 0 being the highest and 10 the lowest.
 */

#define MAX_NAME_LENGTH 20
#define PRIORITY_AMOUNT 10
#define MAX_STACK 1024*8
#define MAX_PARENT_AMOUNT 5
#define MAX_ARGC 20
#define MAX_ARG_LENGTH 16

typedef enum State{
    READY,
    RUNNING,
    BLOCKED,
    EXITED
} State;


typedef struct PCB {
    char name[MAX_NAME_LENGTH];
    uint64_t pid;
    void * rsp;   //stack pointer
    void * rsb;   //stack base
    char * argv[MAX_ARGC];
    int priority;
    char isForeground;  //0 no, 1 yes
    State state;
    uint64_t waitingPID[MAX_PARENT_AMOUNT];
    uint64_t waitingAmount;
    struct PCB *prev;
    struct PCB *next;
    char * fd[2];
} PCB;
//INTERNAL FUNCTIONS -------------------------------------------------------------------------------


/**
 * @def creates halt process and makes preparations for running
 */
void initScheduler();


/**
 * @def Priority Based Round Robin algorithm for scheduling
 * @param prevRsp is the previous process RSP
 * @return current process rsp
 */
void * scheduler(void * prevRsp);

//USERLAND COMMAND FUNCTIONS --------------------------------------------------------------------------

/**
 * @def handles process creation
 * @param process
 * @param name process name
 * @param argc amount of parameters for function
 * @param argv function parameters
 * @param isForeground flag: 0 if background, 1 if foreground
 * @return process pid
 */
int64_t createProcess(void * process, char *name, uint64_t argc, char *argv[], uint64_t isForeground);

/**
 * @def finishes process execution
 */
void exit();

/**
 * @return current process pid
 */
int64_t getPID();

/**
 * @def prints process's name, ID, priority, rsp, rbp, state and if it is foreground of all processes in list
 */
void printProcesses();

/**
 * @def kills process with pid
 * @param pid process to kill pid
 * @return 0 if successful, -1 if not
 */
int64_t kill(int64_t pid);

/**
 * @def modifies process priority
 * @param pid process to change priority's pid
 * @param newPrio
 * @return new process priority
 */
int64_t nice(uint64_t pid, uint64_t newPrio);

/**
 * @def blocks process with matching pid if possible
 * @param pid process to block pid
 * @return 0 if change was successful, -1 if state not changed, -2 if process non existent or EXITED
 */
int64_t block(uint64_t pid);

/**
 * @def unblocks process with matching pid if possible
 * @param pid process to unblock pid
 * @return 0 if change was successful, -1 if not
 */
int64_t unblock(uint64_t pid);

/**
 * @def current process renounces their time RUNNING with the CPU
 * @return current process pid
 */
int64_t yield();

/**
 * @def wait for process with pid to finish
 * @param pid child process
 */
void waitPID(uint64_t pid);

/**
 * @def kills foreground process if not shell
 */
void killForeground();

PCB * findProcess(int64_t pid);


#endif
