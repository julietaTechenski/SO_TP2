#include "include/semaphore.h"
//-----------------------Queu for sem-----------------------------------

typedef struct QueueNode {
    int pid;
    struct QueueNode * next;
} QueueNode;

typedef struct Queue {
    QueueNode *front;
    QueueNode *rear;
} Queue;

Queue* initQueue() {
    Queue *queue = (Queue *) mm_alloc(sizeof(Queue));
    queue->front = NULL;
    queue->rear = NULL;
    return queue;
}

void enqueue(Queue *queue, int pid) {
    QueueNode * newNode = (QueueNode *) mm_alloc(sizeof(QueueNode));
    newNode->pid = pid;
    newNode->next = NULL;

    if (queue->rear == NULL)
        queue->front = newNode;
    else
        queue->rear->next = newNode;

    queue->rear = newNode;
}

int dequeue(Queue *queue) {
    QueueNode *temp = queue->front;
    int value = temp->pid;
    queue->front = queue->front->next;

    if (queue->front == NULL)
        queue->rear = NULL;

    mm_free(temp);
    return value;
}

int isEmpty(Queue *queue) {
    return queue->front == NULL;
}

void freeQueue(Queue *queue) {
    while (!isEmpty(queue)) {
        dequeue(queue);
    }
    mm_free(queue);
}

//------------------------------------------------------------------
//----------------------------List----------------------------------

typedef struct ListNode {
    char name[MAX_LENGTH_NAME];
    int count;
    int32_t mutex;
    Queue * processWaitingQueue;
    struct ListNode *next;
} ListNode;

void insertSem(ListNode **head, char * name, int initialValue) {
    ListNode * newNode = (ListNode *) mm_alloc(sizeof(ListNode));
    my_strcpy(newNode->name, name);
    newNode->count=initialValue;
    newNode->mutex = 1;
    newNode->processWaitingQueue = initQueue();

    if (*head == NULL) {
        *head = newNode;
    } else {
        ListNode *temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
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

ListNode* find_sem(char * sem_id){
    ListNode * aux = semaphoresHead;
    while(aux != NULL){
        if(my_strcmp(aux->name, sem_id) == 1)
            return aux;
        aux = aux->next;
    }
    return NULL;
}

int64_t my_sem_open(char *sem_id, uint64_t initialValue){
    if(find_sem(sem_id) == NULL)
        insertSem(&semaphoresHead, sem_id, initialValue);
    return 0;
}

int64_t my_sem_wait(char *sem_id){
    ListNode * node;
    if((node = find_sem(sem_id)) == NULL)
        return 1;

    writeString(1,"lock get", 8);
    acquireLock(&(node->mutex));
    writeString(1,"lock acq", 8);
    if(node->count == 0) {
        releaseLock(&(node->mutex));
        int pid = getPID();
        enqueue(node->processWaitingQueue, pid);
        block(pid);
    } else {
        node->count--;
        releaseLock(&(node->mutex));
    }

    return 0;
}

int64_t my_sem_post(char *sem_id){
    ListNode * node;
    if((node = find_sem(sem_id)) == NULL)
        return 1;

    acquireLock(&(node->mutex));
    if(isEmpty(node->processWaitingQueue))
        node->count++;
    else {
        int pid = dequeue(node->processWaitingQueue);
        unblock(pid);
    }
    releaseLock(&(node->mutex));
    return 0;
}

int64_t my_sem_close(char * sem_id){
    deleteSem(&semaphoresHead, sem_id);
    return 0;
}

