#ifndef _PROCESSADT_H
#define _PROCESSADT_H

#include <stdint.h>
#include "lib.h"
#include "interrupts.h"
#include "memory_manager.h"
#include "videoDriver.h"
#include "time.h"

#define MAX_NAME_LENGTH 20
#define PRIORITY_AMOUNT 10

typedef enum {
    READY,
    RUNNING,
    BLOCKED
} State;

typedef enum {
    TRUE,
    FALSE
} IsForeground;

typedef struct PCB {
    char name[MAX_NAME_LENGTH];
    uint64_t pid;
    int64_t priority;
    uint64_t* rsp;   //stack
    uint64_t* rbp;   //base pointer
    char isForeground;
    State state;
    int64_t timesRunning;
    struct PCB *prev;
    struct PCB *next;
} PCB;

//INTERNAL FUNCTIONS -------------------------------------------------------------------------------

void setFirstProcess(uint64_t firstProcess);
uint64_t* getCurrentRSP();
PCB * findProcess(int64_t pid, int * priority);
void addProcessToList(PCB *newProcess, int priority);
void removeProcessFromList(PCB *process, int priority);
void killProcess(PCB *process, int priority);
int64_t changeStatePID(uint64_t pid, State newState);
int64_t findNextProcess(uint64_t currentPID);
void scheduler();

//USERLAND COMMAND FUNCTIONS --------------------------------------------------------------------------

int64_t createProcess(uint64_t process, char *name, uint64_t argc, char *argv[]);
void finishProcess();
int64_t getPID();
void printProcesses();
int64_t kill(uint64_t pid);
int64_t changePriority(uint64_t pid, uint64_t newPrio);
int64_t block(uint64_t pid);
int64_t unblock(uint64_t pid);
int64_t yield();

#endif
