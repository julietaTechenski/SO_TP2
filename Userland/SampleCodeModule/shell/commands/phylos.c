// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../include/phylos.h"
#include "../include/test_util.h"
#define MAX_PHYLOS 10
#define MIN_PHYLOS 3

static void phylo(int argc, char *argv[]);
static void think(int phy);
static void eat(int phy);
static void add_philosopher();
static void remove_philosopher();
static void reprint();
static void controllers_handler();

static int thinkers = 5;

static char *palillos[MAX_PHYLOS];  // Semaphore array for forks
static int state[MAX_PHYLOS]; // State array for philosophers
static int pids[MAX_PHYLOS]; // Store PIDs of philosophers

static char *mutex = "access_array";
static char *char_mutex = "char_mutex";
static char command = 0;  // Shared command variable

void phylos() {
    // Initializing semaphores
    sem_init(mutex, 1);
    sem_init(char_mutex, 1);
    void *fd[2];
    fd[0] = NULL;
    fd[1] = NULL;
    my_createProcess(&controllers_handler, "controllers_handler", 0, NULL, 1, fd);

    // Initialize semaphores for forks and create philosopher processes
    for (int i = 0; i < thinkers; i++) {
        char *sem = malloc(sizeof(char) * 2);
        intToString(i, sem);
        sem_init(sem, 1);
        palillos[i] = sem;

        char *index_str = malloc(sizeof(char) * 2);
        intToString(i, index_str);
        char *philo_argv[1] = { index_str };
        pids[i] = my_createProcess(&phylo, "phylo", 1, philo_argv, 1, fd);
    }

    while (1) {
        sem_wait(char_mutex);
        if (command != 0) {
            switch (command) {
                case 'a':
                    command = 0;
                    printf("hola");
                    sem_post(char_mutex);
                    add_philosopher();
                    sem_wait(mutex);
                    reprint();
                    sem_post(mutex);
                    break;
                case 'r':
                    command = 0;
                    sem_post(char_mutex);
                    remove_philosopher();
                    sem_wait(mutex);
                    reprint();
                    sem_post(mutex);
                    break;
                default:
                    command = 0;
                    sem_post(char_mutex);
                    break;
            }
        } else {
            sem_post(char_mutex);
        }
        sleep(1);
    }
}

static void phylo(int argc, char *argv[]) {
    int i = satoi(argv[0]);  // Get philosopher index
    while (1) {
        int left = i;
        int right = (i + 1) % thinkers;

        if (i % 2) { // odd philosopher
            sem_wait(palillos[left]);
            sem_wait(palillos[right]);
        } else { // even philosopher
            sem_wait(palillos[right]);
            sem_wait(palillos[left]);
        }

        eat(i);

        sem_post(palillos[left]);
        sem_post(palillos[right]);

        think(i);
    }
}

static void think(int phy) {
    sem_wait(mutex);
    state[phy] = 0;
    reprint();
    sem_post(mutex);
    sleep(2); // Simulate thinking
}

static void eat(int phy) {
    sem_wait(mutex);
    state[phy] = 1;
    reprint();
    sem_post(mutex);
    sleep(2); // Simulate eating
}

static void add_philosopher() {
    if (thinkers + 1 <= MAX_PHYLOS) {
        char *sem = malloc(sizeof(char) * 2);
        intToString(thinkers, sem);
        sem_init(sem, 1);
        palillos[thinkers] = sem;

        char *index_str = malloc(sizeof(char) * 2);
        intToString(thinkers, index_str);
        char *philo_argv[1] = { index_str };
        void *fd[2] = { NULL, NULL };
        pids[thinkers] = my_createProcess(&phylo, "phylo", 1, philo_argv, 1, fd);

        thinkers++;
    }
}

static void remove_philosopher() {
    if (thinkers - 1 >= MIN_PHYLOS) {
        // Terminate the last philosopher
        thinkers--;
        // Kill the philosopher process
        killProcess(pids[thinkers]);
        // Free the semaphore memory
        sem_close(palillos[thinkers]);
        free(palillos[thinkers]);
    }
}

static void controllers_handler() {
    while (1) {
        char aux = getChar();
        sem_wait(char_mutex);
        command = aux;
        sem_post(char_mutex);
    }
}

static void reprint() {
    printf("%d ->\t ", thinkers);
    for (int i = 0; i < thinkers; i++) {
        printf("%s", state[i] == 1 ? "E   " : ".   ");
    }
    printf("\n\n");
}