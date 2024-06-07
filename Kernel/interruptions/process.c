#include "../include/process.h"

static PCB * current = NULL;
static PCB * priorityArray[PRIORITY_AMOUNT] = {NULL};
static PCB * first = NULL;
static PCB * halt = NULL;
static uint32_t currentPID = 0;
static int32_t amountProcessesReady = 0;

//stateArray = ["Ready", "Running", "Blocked"]     //meaning
char * stateArray[] = {"r", "R", "B"};

PCB * newPcbProcess(void * process, char *name, uint64_t argc, char *argv[]){
    PCB * result = (PCB *) mm_alloc(sizeof(PCB));
    uint64_t * rbp = (uint64_t *) mm_alloc(MAX_STACK * sizeof(uint64_t));

    my_strcpy(result->name, name);
    result->pid = currentPID++;
    result->rsp = createStackContext( &rbp[MAX_STACK-1], process, argc, argv);
    result->rbp = &rbp[MAX_STACK-1];
    result->priority = PRIORITY_AMOUNT;
    result->isForeground = TRUE;
    result->state = READY;
    result->timesRunning = 0;
    result->prev = NULL;
    result->next = NULL;

    return result;
}

int64_t createProcess(void * process, char *name, uint64_t argc, char *argv[]){
    PCB *newProcess = newPcbProcess(process,name, argc, argv);
    addProcessToList(newProcess, GET_PRIORITY_VALUE(newProcess->priority));
    amountProcessesReady++;
    return (newProcess->pid);
}

void initHaltProcess(){
    halt = newPcbProcess(&_hlt, "halt", 0, NULL);
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



void addProcessToList(PCB *newProcess, int priority){
    first = priorityArray[priority];
    if(first == NULL){
        current = newProcess;
        priorityArray[priority] = newProcess;
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
    mm_free(current->rbp);
    mm_free(current);
    scheduler();
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
    if(process->state == newState){
        return 0;
    }
    if(newState == READY){
        amountProcessesReady++;
    }
    if(newState == RUNNING || newState == BLOCKED){
        amountProcessesReady--;
    }
    // in READY
    if(process->state == READY){
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
    int ans = changeStatePID(pid, READY);
    int20();
    return ans;
}

int64_t yield(){
    int20();
    return current->pid;
}

int64_t findNextProcess(uint64_t currentPID){
    int64_t flag = 0;   //flag to check if found
    PCB * ans;
    if(amountProcessesReady == 0){
        return flag;
    }
    for(int i = 0 ; i < PRIORITY_AMOUNT ; i++){
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
    if(current != halt) {
        uint64_t currentPID = current->pid;
        if (current->timesRunning == GET_PRIORITY_VALUE(current->priority) || current->state == BLOCKED) {

            current->timesRunning = 0;  //resets timer
            if (current->priority < PRIORITY_AMOUNT) {  //changes priority
                changePriority(current->pid, current->priority++);  //less priority
            }
            changeStatePID(current->pid, READY);
            if (!findNextProcess(currentPID)) {
                current = halt;
            }
        } else {
            current->timesRunning++;
        }
        changeStatePID(current->pid, RUNNING);
        contextSwitch();
    } else {
        if (!findNextProcess(currentPID)) {
            current = halt;
        }
    }


}