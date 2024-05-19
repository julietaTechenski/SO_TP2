#include "../include/contextHandler.h"

static PCB * current = NULL;
static PCB * first = NULL;
static PCB * halt = NULL;
static uint32_t currentPID = 0;

char createProcess(char isForeground, char *name){
    PCB *newProcess = memory_manager_alloc(sizeof(PCB));

    my_strcpy(newProcess->name, name);
    newProcess->pid = currentPID++;
    newProcess->priority = 0;
    newProcess->isForeground = isForeground;
    newProcess->state = READY;
    newProcess->prev = 0;
    newProcess->next = NULL;

    return (newProcess->pid);
}

