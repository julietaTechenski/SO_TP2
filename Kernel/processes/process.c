#include "../include/process.h"

//TYPES
typedef int64_t (*FunctionType)(uint64_t argc, char *argv[]);

//EXTERN FUNC
extern void initHalt(void * rspHalt);

//STATIC FUNC
static void addProcessToList(PCB *newProcess, int priority);
static void removeProcessFromList(PCB *process, int priority);
static PCB * findNextProcess();
static void haltWrapper();
static void schedulingWrapper(FunctionType function, uint64_t argc, char *argv[]);
static PCB * newPcbProcess(void * process, char *name, uint64_t argc, char *argv[], uint64_t isForeground);
static int64_t changePriority(PCB * process, uint64_t newPrio);
static int64_t changeStatePID(PCB * process, State newState);
static void killProcess(PCB *process);
static void printProcess(PCB * process);

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

PCB * findProcess(int64_t pid){
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


static int64_t changePriority(PCB * process, uint64_t newPrio) {
    removeProcessFromList(process, process->priority);
    process->priority = newPrio;
    addProcessToList(process, newPrio);
    return newPrio;
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
    uint64_t res = function(argc, argv);
    if(res != 0) {
        writeString(1, "Error during process execution: ", 32);
        writeString(1, current->name, my_strlen(current->name));
        writeString(1, "\n", 1);
    }
    exit();
}

static PCB * newPcbProcess(void * process, char *name, uint64_t argc, char *argv[], uint64_t isForeground){
    PCB * result = (PCB *) mm_alloc(sizeof(PCB));
    my_strcpy(result->name, name);
    result->pid = currentPID++;
    result->rsb = mm_alloc(MAX_STACK);


    for(int i = 0; i < argc; i++) {
        result->argv[i] = mm_alloc(MAX_ARG_LENGTH);
        my_strcpy((result->argv)[i], argv[i]);
    }

    result->rsp = createStackContext((uint64_t *)((char *)result->rsb + MAX_STACK - 1), &schedulingWrapper, process, argc, result->argv);
    result->priority = 0;
    result->isForeground = isForeground;
    result->state = READY;
    result->fd[0] = NULL;
    result->fd[1] = NULL;
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


static int64_t changeStatePID(PCB * process, State newState){
    //in BLOCKED
    if(process->state == BLOCKED && newState != READY)
        return -1;

    process->state = newState;
    return 0;
}


void killForeground(){
    if(!my_strcmp(foreground->name, "shell")){
        kill(foreground->pid);
        exit();
    }
}

static void killProcess(PCB *process){
    removeProcessFromList(process, process->priority);
    mm_free(process->rsb);
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

static void printProcess(PCB * process){
    uint32_t aux;
    char buffer[MAX_NAME_LENGTH];

    writeString(1, process->name, my_strlen(process->name));
    writeString(1, "  ", 2);

    aux = intToString(process->pid, buffer);
    writeString(1, buffer, aux);
    writeString(1, "       ", 7);

    aux = intToString(process->priority, buffer);
    writeString(1, buffer, aux);
    writeString(1, "       ", 7);

    aux = intToString((uint64_t)process->rsp, buffer);
    writeString(1, buffer, aux);
    writeString(1, "  ", 2);

    aux = intToString((uint64_t)((char *)process->rsb + MAX_STACK - 1), buffer);
    writeString(1, buffer, aux);
    writeString(1, "  ", 2);

    writeString(1, stateArray[process->state], 1);

    if (process->isForeground)
        writeString(1, "+", 1);

    writeString(1, "\n", 1);

}


void printProcesses() {
    writeString(1, "NAME  PID  PRIORITY    RSP        RBP    STATE\n", 47);
    PCB *iter;

    for(int i = 0 ; i < PRIORITY_AMOUNT ; i++){
        iter = priorityArray[i];
        while (iter != NULL) {
            printProcess(iter);
            iter = iter->next;
        }
    }
}

int64_t kill(uint64_t pid) {
    PCB * process = findProcess(pid);
    if(process == NULL)
        return -1;
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
    if(aux == NULL)
        return;

    uint64_t myPID = getPID();
    aux->waitingPID[aux->waitingAmount++] = myPID;
    block(myPID);
}