#include "../include/process.h"

typedef void (*FunctionType)(uint64_t argc, char *argv[]);

extern void initHalt(void * rspHalt);

//STATIC FUNC
static void addProcessToList(PCB *newProcess, int priority);
static void removeProcessFromList(PCB *process, int priority);
static PCB * findProcess(int64_t pid);
static PCB * findNextProcess();
static void haltWrapper();
static void schedulingWrapper(FunctionType function, uint64_t argc, char *argv[]);
static PCB * newPcbProcess(void * process, char *name, uint64_t argc, char *argv[], uint64_t isForeground);
static int64_t changePriority(PCB * process, uint64_t newPrio);
static int64_t changeStatePID(PCB * process, State newState);
static void killProcess(PCB *process);

//GLOBAL PROCESS ARRAY
static PCB * priorityArray[PRIORITY_AMOUNT] = {NULL};

//GLOBAL STATE VAR
static PCB * current = NULL;
static PCB * foreground = NULL;

//GLOBAL ITER VAR
static PCB * first = NULL;

// GLOBAL AUX VAR
static PCB * halt = NULL;
static uint32_t currentPID = 0;

//stateArray (PRINTING PURPOSE)
char * stateArray[] = {"r", "R", "B"};



//=====================================================================================================================
//============================================= AUX LIST FUNCTIONS ====================================================
//=====================================================================================================================

static void addProcessToList(PCB *newProcess, int priority){
    newProcess->next = newProcess->prev = NULL;
    first = priorityArray[priority];
    if(first != NULL) {
        newProcess->next = first;
        first->prev = newProcess;
    }
    priorityArray[priority] = newProcess;
}

static void removeProcessFromList(PCB *process, int priority){
    if(process->prev != NULL)  //arrange prev process
        (process->prev)->next = process->next;
    else     //first process
        priorityArray[priority] = process->next;


    if(process->next != NULL) //arrange next process
        (process->next)->prev = process->prev;
}

static PCB * findProcess(int64_t pid){
    PCB * iter;
    for(int i = 0 ; i < PRIORITY_AMOUNT ; i++){
        iter = priorityArray[i];
        while(iter != NULL){
            if(iter->pid == pid)
                return iter;
            iter = iter->next;
        }
    }
    return NULL;
}

static PCB * findNextProcess(){
    PCB * ans;
    for(int i = 0 ; i < PRIORITY_AMOUNT ; i++){
        ans = priorityArray[i];
        while(ans != NULL) {
            if (ans->state != BLOCKED)
                return ans;
            ans = ans->next;
        }
    }
    return NULL;
}

//=====================================================================================================================
//========================================= SCHEDULER =================================================================
//=====================================================================================================================

static void haltWrapper(){
    while(1) {
        _hlt();
    }
}

void initScheduler(){
    halt = newPcbProcess(&haltWrapper, "halt", 0, NULL, 0);
    current = halt;
    initHalt(halt->rsp);
}

void * scheduler(void * prevRsp){
    timer_handler();

    current->rsp = prevRsp; // update rsp from previous process

    if(current != halt) { // change priority or terminate process
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


//=====================================================================================================================
//=========================================CREATE PROCESS==============================================================
//=====================================================================================================================

static void schedulingWrapper(FunctionType function, uint64_t argc, char *argv[]){
    (*function)(argc, argv);
    exit();
}

static PCB * newPcbProcess(void * process, char *name, uint64_t argc, char *argv[], uint64_t isForeground){
    PCB * result = (PCB *) mm_alloc(sizeof(PCB));
    my_strcpy(result->name, name);
    result->pid = currentPID++;
    result->rsb = mm_alloc(MAX_STACK);

    for(int i = 0; i < argc; i++)
        my_strcpy(result->argv[i], argv[i]);

    result->rsp = createStackContext((uint64_t *)((char *)result->rsb + MAX_STACK - 1), &schedulingWrapper, process, argc, result->argv);
    result->priority = 0;
    result->isForeground = isForeground;
    result->state = READY;
    result->waitingAmount = 0;
    result->prev = NULL;
    result->next = NULL;

    return result;
}

int64_t createProcess(void * process, char *name, uint64_t argc, char *argv[], uint64_t isForeground){
    PCB *newProcess = newPcbProcess(process, name, argc, argv, isForeground);
    addProcessToList(newProcess, newProcess->priority);
    return (newProcess->pid);
}

//==============================================================================================================
//========================================= UTILS ==============================================================
//==============================================================================================================

static int64_t changePriority(PCB * process, uint64_t newPrio) {
    removeProcessFromList(process, process->priority);
    process->priority = newPrio;
    addProcessToList(process, newPrio);
    return newPrio;
}

static int64_t changeStatePID(PCB * process, State newState){
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


void killForeground(){
    if(my_strcmp(foreground->name, "shell") != 0){
        kill(foreground->pid);
    }
}

static void killProcess(PCB *process){
    removeProcessFromList(process, process->priority);
    mm_free((void*) process->rsb);
    mm_free((void*) process);
}


void exit(){
    for(int i = 0 ; i < current->waitingAmount ; i++)
        unblock(current->waitingPID[i]);
    current->state = EXITED;
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
    PCB * process = findProcess(pid);
    if(process == NULL){
        return -1;
    }
    killProcess(process);
    return 0;
}

int64_t nice(uint64_t pid, uint64_t newPrio){
    return changePriority(findProcess(pid), newPrio);
}

int64_t block(uint64_t pid){
    int ans = changeStatePID(findProcess(pid), BLOCKED);
    int20();
    return ans;
}

int64_t unblock(uint64_t pid){
    int ans = changeStatePID(findProcess(pid), READY);
    int20();
    return ans;
}

int64_t yield(){
    int20();
    return current->pid;
}

void waitPID(uint64_t pid){
    PCB * aux = findProcess(pid);
    uint64_t myPID = getPID();
    aux->waitingPID[aux->waitingAmount++] = myPID;
    block(myPID);
}