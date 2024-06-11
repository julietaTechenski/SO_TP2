// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../include/wrapper.h"


int64_t killProcess(uint64_t argc, char * argv[]){
    if(argc!=1)
        return -1;
    int64_t pid;
    pid = satoi(argv[0]);
    return kill(pid);
}


int64_t niceProcess(uint64_t argc, char * argv[]){
    if(argc != 2)
        return -1;

    int64_t pid = satoi(argv[0]);
    int64_t newPrio = satoi(argv[1]);
    return nice(pid, newPrio);
}

int64_t blockProcess(uint64_t argc, char * argv[]){
    if(argc != 1)
        return -1;
    int64_t pid = satoi(argv[0]);
    return block(pid);
}

int64_t unblockProcess(uint64_t argc, char * argv[]){
    if(argc != 1)
        return -1;
    int64_t pid = satoi(argv[0]);
    return unblock(pid);
}

