
#include "../include/wrapper.h"


int64_t killProcess(uint64_t argc, char * argv[]){
    if(argc!=1)
        return -1;
    int64_t pid;
    pid = satoi(argv[0]);
    return kill(pid);
}


int64_t niceProcess(uint64_t argc, char * argv[]){
    if(argc!=2)
        return -1;

    int64_t pid = satoi(argv[0]);
    uint64_t newPrio = satoi(argv[1]);
    return nice(pid, newPrio);
}

int64_t blockProcess(uint64_t argc, char * argv[]){
    if(argc != 1)
        return -1;
    int64_t pid = satoi(argv[0]);
    return block(pid);
}

