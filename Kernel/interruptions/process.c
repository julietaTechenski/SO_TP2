#include "../include/process.h"

static PCB * current = NULL;
static PCB * priorityArray[PRIORITY_AMOUNT] = {NULL};
static PCB * first = NULL;
static PCB * halt = NULL;
static uint32_t currentPID = 0;

//stateArray = ["Ready", "Running", "Blocked"]     //meaning
char * stateArray[] = {"r", "R", "B"};

void setFirstProcess(uint64_t firstProcess){
    uint64_t *rbp = mm_alloc(1024 * sizeof(uint64_t));
    PCB *first = mm_alloc(sizeof(PCB));
    first->rbp = rbp;
    first->pid = currentPID++;
    first->rsp = createStackContext((uint64_t) & rbp[1023], firstProcess, 0, NULL);   //aca debería estar el first process que es mi shell

    first->priority = PRIORITY_AMOUNT;
    first->next = NULL;
    first->prev = NULL;
    my_strcpy(first->name, "shell");

    uint64_t *rbpHalt = mm_alloc(1024 * sizeof(uint64_t));
    halt = mm_alloc(sizeof(PCB));
    halt->rbp = rbp;
    halt->rsp = createStackContext((uint64_t) & rbpHalt[1023], _hlt, 0, NULL);
    halt->priority = 0;
    halt->next = NULL;
    halt->prev = NULL;
    my_strcpy(first->name, "halt");

    addProcessToList(first, PRIORITY_AMOUNT - first->priority);
}

uint64_t* getCurrentRSP(){
    return current->rsp;
}

PCB * findProcess(int64_t pid, int * priority){
    if(current->pid == pid){
        return current;
    }
    PCB * iter;
    for(int i = 0 ; i < PRIORITY_AMOUNT ; i++){
        iter = priorityArray[i];
        while(iter != NULL){
            if(iter->pid == pid){
                *priority = i;
                return iter;
            }
            iter = iter->next;
        }
    }
    return NULL;
}

int64_t createProcess(uint64_t process, char *name, uint64_t argc, char *argv[]){
    uint64_t *rbp = mm_alloc(1024 * sizeof(uint64_t));
    PCB *newProcess = mm_alloc(sizeof(PCB));

    my_strcpy(newProcess->name, name);
    newProcess->pid = currentPID++;
    newProcess->rsp = createStackContext((uint64_t) & rbp[1023], process, argc, argv);
    newProcess->rbp = rbp;
    newProcess->isForeground = TRUE;
    newProcess->state = READY;
    newProcess->timesRunning = 0;
    newProcess->prev = 0;
    newProcess->next = NULL;

    addProcessToList(newProcess, PRIORITY_AMOUNT);

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

void removeProcessFromList(PCB *process, int priority){
    if(process->prev != NULL){  //arrange prev process
        process->prev->next = process->next;
    } else {    //first process
        priorityArray[priority] = process->next;
    }

    if(process->next != NULL){  //arrange next process
        process->next->prev = process->prev;
    }
}

void finishProcess(){
    removeProcessFromList(current, current->priority);
    if(current->next != NULL){
        PCB * aux = current->next;
        mm_free(current->rbp);
        mm_free(current);
        current = aux;  //update current
    } else {    //last process
        int64_t priority = current->priority;
        mm_free(current->rbp);
        mm_free(current);
        current = priorityArray[priority];    //start from the beginning in the same priority
    }
    contextSwitch();
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
                case READY:
                    writeString(0, stateArray[0], 1);
                    break;
                case RUNNING:
                    writeString(0, stateArray[1], 1);
                    break;
                case BLOCKED:
                    writeString(0, stateArray[2], 1);
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
    int priority;
    PCB * process = findProcess(pid, &priority);
    if(process == NULL){
        return -1;
    }
    killProcess(process, priority);
    return 0;
}

void killProcess(PCB *process, int priority){
    removeProcessFromList(process, priority);

    mm_free(process->rbp);
    mm_free(process);
}

int64_t changePriority(uint64_t pid, uint64_t newPrio) {
    if(newPrio > PRIORITY_AMOUNT){
        return -1;
    }

    int oldPrio;
    PCB * process = findProcess(pid, &oldPrio);
    if(process == NULL){
        return -1;
    }
    if(newPrio == oldPrio){
        return newPrio;
    }
    removeProcessFromList(process, oldPrio);
    addProcessToList(process, newPrio);
    return newPrio;
}

int64_t changeStatePID(uint64_t pid, State newState){
    PCB * process = NULL;
    int priority;
    process = findProcess(pid, &priority);
    if(process == NULL){
        return -1;
    }
    // in READY
    if(process->state == READY && newState == RUNNING){
        process->state = newState;
        return 0;
    }
    //in BLOCKED
    if(process->state == BLOCKED && newState == READY){
        process->state = newState;
        return 0;
    }
    // in RUNNING, can go to READY and BLOCKED
    if(process->state == RUNNING){
        process->state = newState;
        return 0;
    }
    return -1;  //made an invalid change of state
}


int64_t block(uint64_t pid){
    int ans = changeStatePID(pid, BLOCKED);
    int20();
    return ans;
}

int64_t unblock(uint64_t pid){
    return changeStatePID(pid, READY);
}

int64_t yield(){
    int20();
    return current->pid;
}

int64_t findNextProcess(uint64_t currentPID){
    int64_t flag = 0;   //flag to check if found
    PCB * ans;
    for(int i = 0 ; i < PRIORITY_AMOUNT && !flag; i++){
        ans = priorityArray[i];
        while(ans != NULL){
            if (ans->state == READY && ans->pid != currentPID) {
                current = ans;
                flag = 1;
                return flag;
            }
            ans = ans->next;
        }

    }
    return flag;
}

void scheduler(){
    timer_handler();
    if(current != halt){
        uint64_t currentPID = current->pid;
        if(current->timesRunning == (PRIORITY_AMOUNT - current->priority) || current->state == BLOCKED){

            current->timesRunning = 0;  //resets timer
            if(current->priority > 0){  //changes priority
                changePriority(current->pid, current->priority++);
            }

            if(!findNextProcess(currentPID)){
                current = halt;
            }
        } else {
            current->timesRunning++;
        }
    }
}