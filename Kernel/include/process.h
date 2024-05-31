#ifndef _PROCESSADT_H
#define _PROCESSADT_H

#include <stdint.h>
#include "lib.h"
#include "interrupts.h"
#include "memory_manager.h"
#include "videoDriver.h"

#define MAX_NAME_LENGTH 20
#define PRIORITY_AMOUNT 3

typedef enum {
    HIGH,   //0
    MEDIUM, //1
    LOW     //2
} Priority;

typedef enum {
    EXIT,
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
    Priority priority;
    uint64_t* rsp;   //stack
    uint64_t* rbp;   //base pointer
    char isForeground;
    State state;
    struct PCB *prev;
    struct PCB *next;
} PCB;

PCB * findProcess(int64_t pid, int * priority);
int64_t createProcess(char *name, uint64_t argc, char *argv[]);
void addProcessToList(PCB *newProcess, int priority);
void removeProcessToList(PCB *process, int priority);
void finishProcess();
int64_t getPID();
void printProcesses();
int64_t kill(uint64_t pid);
void killProcess(PCB *process, int priority);
int64_t changePriority(uint64_t pid, uint64_t newPrio);
int64_t changeStatePID(uint64_t pid, State newState);
int64_t block(uint64_t pid);
int64_t unblock(uint64_t pid);
#endif
