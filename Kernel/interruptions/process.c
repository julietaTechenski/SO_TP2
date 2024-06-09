#include "../include/process.h"

static PCB * current = NULL;
static PCB * foreground = NULL;
static PCB * priorityArray[PRIORITY_AMOUNT] = {NULL};
static PCB * first = NULL;
static PCB * halt = NULL;
static uint32_t currentPID = 0;

//stateArray = ["Ready", "Running", "Blocked"]     //meaning
char * stateArray[] = {"r", "R", "B"};

typedef void (*FunctionType)(uint64_t, char *[]);
char buffer[20];

//============================================= FUNCTIONS ==============================================================

void schedulingWrapper(FunctionType function, uint64_t argc, char *argv[]){
    (*function)(argc, argv);
    exit();
}

void addProcessToList(PCB *newProcess, int priority){
    newProcess->next = newProcess->prev = NULL;
    first = priorityArray[priority];
    if(first != NULL){
        newProcess->next = first;
        first->prev = newProcess;
    }
    priorityArray[priority] = newProcess;

}

int64_t changeStatePID(PCB * process, State newState){
    if(process == NULL){
        return -1;
    }
    if(process->state == newState){
        return 0;
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

PCB * findNextProcess(){
    PCB * ans;
    for(int i = 0 ; i < PRIORITY_AMOUNT ; i++){
        ans = priorityArray[i];
        while(ans != NULL){
            if (ans->state != BLOCKED) {
                return ans;
            }
            ans = ans->next;
        }
    }
    return NULL;
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

void haltWrapper(){
    while(1) {
        _hlt();
    }
}

void initScheduler(){
    halt = newPcbProcess(&haltWrapper, "halt", 0, NULL, 0);
    current = halt;
    initHalt(halt->rsp);
}

void killForeground(){
    if(my_strcmp(foreground->name, "shell") != 0){
        kill(foreground->pid);
    }
}

void killProcess(PCB *process){
    removeProcessFromList(process, process->priority);
    mm_free((void*) process->rsb);
    mm_free((void*) process);
}

PCB * newPcbProcess(void * process, char *name, uint64_t argc, char *argv[], uint64_t isForeground){
    PCB * result = (PCB *) mm_alloc(sizeof(PCB));

    my_strcpy(result->name, name);
    result->pid = currentPID++;
    result->rsb = mm_alloc(MAX_STACK);
    result->rsp = createStackContext(result->rsb + MAX_STACK, &schedulingWrapper, process, argc, argv);
    result->priority = 0;
    result->isForeground = isForeground;
    result->state = READY;
    result->waitingAmount = 0;
    result->prev = NULL;
    result->next = NULL;

    return result;
}

void removeProcessFromList(PCB *process, int priority){
    if(process->prev != NULL){  //arrange prev process
        (process->prev)->next = process->next;
    } else {    //first process
        priorityArray[priority] = process->next;
    }

    if(process->next != NULL){  //arrange next process
        (process->next)->prev = process->prev;
    }
}

void * scheduler(void * prevRsp){
    timer_handler();

    current->rsp = prevRsp; // update rsp from previous process

    if(current != halt) {
        if (current->state == EXITED) {
            killProcess(current);
        } else if (current->state == RUNNING) {
            if (current->priority < PRIORITY_AMOUNT-1)
                changePriority(current, current->priority + 1);
            changeStatePID(current, READY);
        } else if (current->state == BLOCKED) {
            if (current->priority > 0)
                changePriority(current, current->priority - 1);
        }
    }

    PCB * next = findNextProcess();
    if(next != NULL) {
        current = next;
        if(current->isForeground){
            foreground = current;
        }
    }
    else
        current = halt;

    current->state = RUNNING;
    return current->rsp;
}

//======================================================================================================================

int64_t createProcess(void * process, char *name, uint64_t argc, char *argv[], uint64_t isForeground){
    PCB *newProcess = newPcbProcess(process, name, argc, argv, isForeground);
    addProcessToList(newProcess, newProcess->priority);
    return (newProcess->pid);
}

void exit(){
    current->state = EXITED;
    for(int i = 0 ; i < current->waitingAmount ; i++){
        unblock(current->waitingPID[i]);
    }
    int20();
}

int64_t getPID(){
    return current->pid;
}

void printProcesses() {
    char buffer[MAX_NAME_LENGTH];
    writeString(1, "NAME  PID  PRIORITY    RSP        RBP    STATE\n", 47);
    PCB *iter;

    for(int i = 0 ; i < PRIORITY_AMOUNT ; i++){
        iter = priorityArray[i];
        while (iter != NULL) {
            writeString(1, iter->name, my_strlen(iter->name));
            writeString(1, "  ", 2);

            uint32_t pidStringLen = intToString(iter->pid, buffer);
            writeString(1, buffer, pidStringLen);
            writeString(1, "       ", 7);

            uint32_t priorityStringLen = intToString(i, buffer);
            writeString(1, buffer, priorityStringLen);
            writeString(1, "       ", 7);

            uint32_t rspStringLen = intToString(iter->rsp, buffer);
            writeString(1, buffer, rspStringLen);
            writeString(1, "  ", 2);

            uint32_t s = intToString(iter, buffer);
            writeString(1, buffer, s);
            writeString(1, "     ", 5);

            switch (iter->state) {
                case READY:
                    writeString(1, stateArray[0], 1);
                    break;
                case RUNNING:
                    writeString(1, stateArray[1], 1);
                    break;
                case BLOCKED:
                    writeString(1, stateArray[2], 1);
                    break;
                case EXITED:
                    writeString(1,"E",1);
                    break;
            }
            if (current->isForeground) {
                writeString(1, "+", 1);
            }

            writeString(1, "\n", 1);
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
    killProcess(process);
    return 0;
}

int64_t changePriority(PCB * process, uint64_t newPrio) {
    removeProcessFromList(process, process->priority);
    process->priority = newPrio;
    addProcessToList(process, newPrio);
    return newPrio;
}

int64_t block(uint64_t pid){
    int priority;
    int ans = changeStatePID(findProcess(pid, &priority), BLOCKED);
    int20();
    return ans;
}

int64_t unblock(uint64_t pid){
    int priority;
    int ans = changeStatePID(findProcess(pid, &priority), READY);
    int20();
    return ans;
}

int64_t yield(){
    int20();
    return current->pid;
}

void waitPID(uint64_t pid){
    int priority;
    PCB * aux = findProcess(pid, &priority);
    uint64_t myPID = getPID();
    aux->waitingPID[aux->waitingAmount++] = myPID;
    block(myPID);
}