#ifndef _PROCESSADT_H
#define _PROCESSADT_H

#include <stdint.h>
#include "lib.h"
#include "interrupts.h"
#include "memory_manager.h"
#include "videoDriver.h"

#define MAX_NAME_LENGTH 20

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

typedef struct PCB {
    char name[MAX_NAME_LENGTH];
    int64_t pid;
    Priority priority;
    uint64_t* rsp;   //stack
    uint64_t* rbp;   //base pointer
    char isForeground;
    State state;
    struct PCB *prev;
    struct PCB *next;
} PCB;

int64_t createProcess(char isForeground, char *name);
void addProcessToList(PCB *newProcess);
void finishProcess();
int64_t getPID();
void printProcesses();
#endif