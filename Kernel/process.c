#include "../include/process.h"

static PCB * current = NULL;
static PCB * first = NULL;
static PCB * halt = NULL;
static uint32_t currentPID = 0;

char createProcess(char isForeground, char *name){
    uint64_t *rbp = mm_alloc(1024 * sizeof(uint64_t));
    PCB *newProcess = mm_alloc(sizeof(PCB));

    my_strcpy(newProcess->name, name);
    newProcess->pid = currentPID++;
    newProcess->priority = 0;
    newProcess->rsp = createStackContext((uint64_t) & rbp[1023]);
    newProcess->rbp = rbp;
    newProcess->isForeground = isForeground;
    newProcess->state = READY;
    newProcess->prev = 0;
    newProcess->next = NULL;

    addProcessToList(newProcess);

    return (newProcess->pid);
}

void addProcessToList(PCB *newProcess){
    if(first == NULL){
        first = newProcess;
        current = newProcess;
        return;
    } else {
        newProcess->next = first;
        first->prev = newProcess;
        first = newProcess;
    }
}

void finishProcess(){
    current->state = EXIT;
    mm_free(current);
}


