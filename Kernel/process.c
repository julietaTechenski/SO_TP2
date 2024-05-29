#include "../include/process.h"

static PCB * current = NULL;
static PCB * first = NULL;
static PCB * halt = NULL;
static uint32_t currentPID = 0;

//stateArray = ["Finished", "Ready", "Running", "Blocked"]     //meaning
char * stateArray[] = {"F", "r", "R", "B"};

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

void printProcesses(){
    char buffer[MAX_NAME_LENGTH];
    writeString(0, "NAME  PID  PRIORITY    RSP    RBP    STATE", 42);
    PCB * iter = first;
    while(iter != NULL){
        writeString(0, iter->name, my_strlen(iter->name));

        uint32_t pidStringLen = intToString(iter->pid, buffer);
        writeString(0, buffer, pidStringLen);
        writeString(0, "  ", 2);

        uint32_t priorityStringLen = intToString(iter->priority, buffer);
        writeString(0, buffer, priorityStringLen);
        writeString(0, "  ", 2);

        uint32_t rspStringLen = intToString(*iter->rsp, buffer);
        writeString(0, buffer, rspStringLen);
        writeString(0, "  ", 2);

        uint32_t rbpStringLen = intToString(*iter->rbp, buffer);
        writeString(0, buffer, rbpStringLen);
        writeString(0, "  ", 2);

        switch (current->state) {
            case EXIT:
                writeString(0, stateArray[0], 1);
                break;
            case READY:
                writeString(0, stateArray[1], 1);
                break;
            case RUNNING:
                writeString(0, stateArray[2], 1);
                break;
            case BLOCKED:
                writeString(0, stateArray[3], 1);
                break;
        }
        if(current->isForeground){
            writeString(0, "+", 1);
        }
        writeString(0, "\n", 1);
        iter = iter->next;
    }
}

uint32_t getPID(){
    return current->pid;
}
