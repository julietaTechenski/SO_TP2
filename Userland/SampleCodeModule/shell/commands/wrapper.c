
#include "../include/wrapper.h"


int64_t killProcess(uint64_t argc, char * argv[]){
    int64_t pid;
    pid = satoi(argv[0]);
    return kill(pid);
}
