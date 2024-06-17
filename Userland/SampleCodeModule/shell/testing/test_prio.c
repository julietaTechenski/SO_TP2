// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../include/test_processes.h"

#define MINOR_WAIT 10 // TODO: Change this value to prevent a process from flooding the screen
#define WAIT 10000      // TODO: Change this value to make the wait long enough to see theese processes beeing run at least twice

#define TOTAL_PROCESSES 3
#define LOWEST 9  // TODO: Change as required
#define MEDIUM 5  // TODO: Change as required
#define HIGHEST 0 // TODO: Change as required

int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};

void test_prio() {
    int64_t pids[TOTAL_PROCESSES];
    char *argv[] = {0};
    char arg[20];
    intToString(MINOR_WAIT, arg);
    argv[0] = arg;
    uint64_t i;

    for (i = 0; i < TOTAL_PROCESSES; i++){
        void *fd[2];
        fd[0] = NULL;
        fd[1] = NULL;
        pids[i] = my_createProcess(&endless_loop_print, "endless_loop_print", 1, argv, 0, fd);
    }

    sleep(WAIT);
    printf("\nCHANGING PRIORITIES...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        nice(pids[i], prio[i]);

    sleep(WAIT);
    printf("\nBLOCKING...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        block(pids[i]);

    printf("CHANGING PRIORITIES WHILE BLOCKED...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        nice(pids[i], MEDIUM);

    printf("UNBLOCKING...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        unblock(pids[i]);

    sleep(WAIT);
    printf("\nKILLING...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++){
        kill(pids[i]);
    }

    printf("Success\n");
}
