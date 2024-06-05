#ifndef SO_TP2_PIPES_H
#define SO_TP2_PIPES_H

typedef struct PCB {
    char name[MAX_NAME_LENGTH];
    uint64_t pid;
    int64_t priority;
    uint64_t* rsp;   //stack
    uint64_t* rbp;   //base pointer
    char isForeground;
    State state;
    int64_t timesRunning;
    struct PCB *prev;
    struct PCB *next;

} PCB;

#endif //SO_TP2_PIPES_H
