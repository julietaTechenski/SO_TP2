#include "../include/process.h"

static PCB * current = NULL;
static PCB * priorityArray[PRIORITY_AMOUNT] = {NULL};
static PCB * first = NULL;
static PCB * halt = NULL;
static uint32_t currentPID = 0;

//stateArray = ["Finished", "Ready", "Running", "Blocked"]     //meaning
char * stateArray[] = {"F", "r", "R", "B"};

PCB * findProcess(int64_t pid){
    if(current->pid == pid){
        return current;
    }
    PCB * iter;
    for(int i = 0 ; i < PRIORITY_AMOUNT ; i++){
        iter = priorityArray[i];
        while(iter != NULL){
            if(iter->pid == pid){
                return iter;
            }
            iter = iter->next;
        }
    }
    return NULL;
}

int64_t createProcess(char isForeground, char *name){
    uint64_t *rbp = mm_alloc(1024 * sizeof(uint64_t));
    PCB *newProcess = mm_alloc(sizeof(PCB));

    my_strcpy(newProcess->name, name);
    newProcess->pid = currentPID++;
    newProcess->rsp = createStackContext((uint64_t) & rbp[1023]);
    newProcess->rbp = rbp;
    newProcess->isForeground = isForeground;
    newProcess->state = READY;
    newProcess->prev = 0;
    newProcess->next = NULL;

    addProcessToList(newProcess, HIGH);

    return (newProcess->pid);
}

void addProcessToList(PCB *newProcess, int priority){
    first = priorityArray[priority];
    if(first == NULL){
        first = newProcess;
        current = newProcess;
    } else {
        newProcess->next = first;
        first->prev = newProcess;
        first = newProcess;
    }
}

void finishProcess(){
    current->state = EXIT;
    mm_free(current->rbp);
    mm_free(current);
}

int64_t getPID(){
    return current->pid;
}

void printProcesses() {
    char buffer[MAX_NAME_LENGTH];
    writeString(0, "NAME  PID  PRIORITY    RSP    RBP    STATE", 42);
    PCB *iter;
    for(int i = 0 ; i < PRIORITY_AMOUNT ; i++){
        iter = priorityArray[i];
        while (iter != NULL) {
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
            if (current->isForeground) {
                writeString(0, "+", 1);
            }
            writeString(0, "\n", 1);
            iter = iter->next;
        }
    }
}

int64_t kill(uint64_t pid) {
    PCB * process = findProcess(pid);
    if(process == NULL){
        return -1;
    }
    killProcess(process);
    return 0;
}

void killProcess(PCB *process){
    if(process->prev != NULL){  //arrange prev process
        process->prev->next = process->next;
    } else {    //first process
        first = process->next;
    }

    if(process->next != NULL){  //arrange next process
        process->next->prev = process->prev;
    }

    mm_free(process->rbp);
    mm_free(process);
}
