// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/semaphore.h"

#define MAX_PID 25

//-----------------------Queu for sem-----------------------------------

typedef struct Queue {
    int pid[MAX_PID];
    int first;
    int last;
} Queue;

Queue* initQueue() {
    Queue * queue = (Queue *) mm_alloc(sizeof(Queue));
    queue->first = queue->last = 0;
    return queue;
}

void enqueue(Queue *queue, int pid) {
    queue->pid[queue->last] = pid;
    queue->last = (queue->last + 1) % MAX_PID;
}

int dequeue(Queue *queue) {
    int aux = queue->pid[queue->first];
    queue->first = (queue->first + 1) % MAX_PID;
    return aux;
}

int isEmpty(Queue *queue) {
    return queue->first == queue->last;
}

void freeQueue(Queue *queue) {
    mm_free(queue);
}

//------------------------------------------------------------------
//----------------------------List----------------------------------

typedef struct ListNode {
    char name[MAX_LENGTH_NAME];
    int count;
    int32_t mutex;
    Queue * processWaitingQueue;
    uint32_t amountOfOpen;
    struct ListNode *next;
} ListNode;

ListNode * insertSem(ListNode **head, char * name, int initialValue) {
    ListNode * newNode = (ListNode *) mm_alloc(sizeof(ListNode));
    my_strcpy(newNode->name, name);
    newNode->count=initialValue;
    newNode->mutex = 0;
    newNode->processWaitingQueue = initQueue();

    newNode->next = *head;
    *head = newNode;

    return newNode;
}

void deleteSem(ListNode **head, char * name){
    ListNode *temp = *head;
    ListNode *prev = NULL;

    if (temp != NULL && my_strcmp(temp->name, name) == 1) {
        *head = temp->next;
        freeQueue(temp->processWaitingQueue);
        mm_free(temp);
        return;
    }

    while (temp != NULL && my_strcmp(temp->name, name) != 1) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        return;
    }

    prev->next = temp->next;
    freeQueue(temp->processWaitingQueue);
    mm_free(temp);
}


//------------------------------------------------------------------
//---------------------------Semaphore------------------------------


static ListNode* semaphoresHead = NULL;
static int32_t ListMutex = 0;

static ListNode* find_sem(char * sem_id){
    ListNode * aux = semaphoresHead;
    while(aux != NULL){
        if(my_strcmp(aux->name, sem_id) == 1) {
            return aux;
        }
        aux = aux->next;
    }
    return NULL;
}

int64_t my_sem_open(char *sem_id, uint64_t initialValue){
    acquireLock(&ListMutex);

    ListNode * aux = find_sem(sem_id);
    if(aux == NULL)
        aux = insertSem(&semaphoresHead, sem_id, initialValue);
    aux->amountOfOpen++;

    releaseLock(&ListMutex);
    return 0;
}

int64_t my_sem_wait(char *sem_id){
    acquireLock(&ListMutex);

    ListNode * node;
    if((node = find_sem(sem_id)) == NULL)
        return 1;


    if(node->count == 0) {
        int pid = getPID();
        enqueue(node->processWaitingQueue, pid);
        releaseLock(&ListMutex);
        block(pid);
    } else {
        node->count--;
        releaseLock(&ListMutex);
    }

    return 0;
}

int64_t my_sem_post(char *sem_id){
    acquireLock(&ListMutex);

    ListNode * node;
    if((node = find_sem(sem_id)) == NULL)
        return 1;


    if(isEmpty(node->processWaitingQueue)) {
        node->count++;
    } else {
        int pid = dequeue(node->processWaitingQueue);
        unblock(pid);
    }

    releaseLock(&ListMutex);
    return 0;
}

int64_t my_sem_close(char * sem_id){
    acquireLock(&ListMutex);

    ListNode * aux = find_sem(sem_id);
    if(aux != NULL) {
        if(aux->amountOfOpen > 0) {
            aux->amountOfOpen--;
        } else {
            deleteSem(&semaphoresHead, sem_id);
        }
    }

    releaseLock(&ListMutex);
    return 0;
}

