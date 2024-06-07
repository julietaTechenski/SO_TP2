#include "../include/process.h"

static PCB * current = NULL;
static PCB * priorityArray[PRIORITY_AMOUNT] = {NULL};
static PCB * first = NULL;
static PCB * halt = NULL;
static uint32_t currentPID = 0;
static int32_t amountProcessesReady = 0;

//stateArray = ["Ready", "Running", "Blocked"]     //meaning
char * stateArray[] = {"r", "R", "B"};

typedef void (*FunctionType)(uint64_t, char *[]);

void schedulingWrapper(FunctionType function, uint64_t argc, char *argv[]){
    function(argc, argv);
    finishProcess();
}

PCB * newPcbProcess(void * process, char *name, uint64_t argc, char *argv[]){
    PCB * result = (PCB *) mm_alloc(sizeof(PCB));
    uint64_t * rbp = (uint64_t *) mm_alloc(MAX_STACK * sizeof(uint64_t));

    my_strcpy(result->name, name);
    result->pid = currentPID++;
    result->rsp = createStackContext( &rbp[MAX_STACK-1], &schedulingWrapper, process, argc, argv);
    result->rsb = rbp;
    result->priority = PRIORITY_AMOUNT;
    result->isForeground = TRUE;
    result->state = READY;
    result->timesRunning = 0;
    result->prev = NULL;
    result->next = NULL;

    return result;
}

int64_t createProcess(void * process, char *name, uint64_t argc, char *argv[]){
    PCB *newProcess = newPcbProcess(process, name, argc, argv);
    addProcessToList(newProcess, GET_PRIORITY_VALUE(newProcess->priority));
    amountProcessesReady++;
    return (newProcess->pid);
}

void initHaltProcess(){
    halt = newPcbProcess(&_hlt, "halt", 0, NULL);
    current = halt;
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

void killProcess(PCB *process){
    removeProcessFromList(process, process->priority);
    mm_free(process->rsb);
    mm_free(process);
}

void finishProcess(){
    killProcess(current);
    int20();
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
    killProcess(process);
    return 0;
}


int64_t changePriority(PCB * process, uint64_t newPrio) {
    if(process == NULL)
        return -1;

    if(newPrio == process->priority)
        return newPrio;

    removeProcessFromList(process, process->priority);
    process->priority = newPrio;
    addProcessToList(process, newPrio);
    return newPrio;
}

int64_t changeStatePID(PCB * process, State newState){
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

PCB * findNextProcess(uint64_t currentPID){
    PCB * ans;
    for(int i = 0 ; i < PRIORITY_AMOUNT ; i++){
        ans = priorityArray[i];
        while(ans != NULL){
            if (ans->state == READY && ans->pid != currentPID)
                return ans;
            ans = ans->next;
        }
    }
    return NULL;
}



void * scheduler(void * prevRsp){
    timer_handler();

    if(current != halt) {
        current->rsp = prevRsp; // update rsp from previous process

        if (current->state == BLOCKED) { //didn't use all quantum
            if (current->priority > 0)
                changePriority(current, current->priority - 1);
        } else { //used all quantum
            if (current->priority < PRIORITY_AMOUNT)
                changePriority(current, current->priority + 1);
            changeStatePID(current, READY);
        }

    }

    PCB * next = findNextProcess(current->pid);
    if(next != NULL)
        current = next;
    else
        current = halt;

    return current->rsp;
}