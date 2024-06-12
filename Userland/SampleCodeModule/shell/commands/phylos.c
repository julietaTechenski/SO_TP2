// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../include/phylos.h"
#include "../include/test_util.h"
#define MAX_PHYLOS 10
#define MIN_PHYLOS 3
#define INITIAL_THINKERS 5


static void phyloProcess(int argc, char *argv[]);
static void think(int phy);
static void eat(int phy);
static void reprint();
static void controllers_handler();
static void add_philosopher(int phy);


typedef struct phylo {
    char * sem;
    int state; // State array for philosophers
} phylo;

#define MUTEX_ARRAY "access_array"
static phylo philosophers[MAX_PHYLOS];

#define MUTEX_THINKERS "thinkers_mutex"
static int thinkers = INITIAL_THINKERS;

void phylosProcess() {
    // Initializing semaphores
    sem_init(MUTEX_ARRAY, 1);
    sem_init(MUTEX_THINKERS, 1);

    void *fd[2] = {NULL};
    my_createProcess(&controllers_handler, "controllers_handler", 0, NULL, 1, fd);


    // Initialize semaphores for forks and create philosopher processes
    for (int i = 0; i < MAX_PHYLOS; i++)
        add_philosopher(i);

    while (1) {
        reprint();
        sleep(5);
    }

}

static void phyloProcess(int argc, char *argv[]) {
    int id = satoi(argv[0]);
    int left = id;
    int right = (id + 1) % MAX_PHYLOS;

    while (1) {
        if (id == MAX_PHYLOS - 1) {
            sem_wait(philosophers[right].sem);
            sem_wait(philosophers[left].sem);
        } else {
            sem_wait(philosophers[left].sem);
            sem_wait(philosophers[right].sem);
        }

        eat(id);

        sem_post(philosophers[left].sem);
        sem_post(philosophers[right].sem);

        think(id);
    }
}

static void think(int phy) {
    sem_wait(MUTEX_ARRAY);
    philosophers[phy].state = 0;
    sem_post(MUTEX_ARRAY);
    sleep(GetUniform(phy));
}

static void eat(int phy) {
    sem_wait(MUTEX_ARRAY);
    philosophers[phy].state = 1;
    sem_post(MUTEX_ARRAY);
    sleep(GetUniform(phy));
}

static void add_philosopher(int phy) {
    char * indexStr = malloc(sizeof(char) * 2);
    intToString(phy, indexStr);

    sem_init(indexStr, 1);
    philosophers[phy].sem = indexStr;

    char *philo_argv[1] = { indexStr };
    void *fd[2] = { NULL };
    my_createProcess(&phyloProcess, "phylo", 1, philo_argv, 0, fd);
}

static void controllers_handler() {
    while (1) {
        char aux = getChar();

        switch (aux) {
            case 'a':
                sem_wait(MUTEX_THINKERS);
                if(thinkers < MAX_PHYLOS)
                    thinkers++;
                sem_post(MUTEX_THINKERS);
                break;
            case 'r':
                sem_wait(MUTEX_THINKERS);
                if(thinkers > MIN_PHYLOS)
                    thinkers--;
                sem_post(MUTEX_THINKERS);
                break;
        }

    }
}

static void reprint() {
    sem_wait(MUTEX_ARRAY);
    sem_wait(MUTEX_THINKERS);
    printf("%d ->\t ", thinkers);
    for (int i = 0; i < thinkers; i++)
        printf("%s", philosophers[i].state == 1 ? "E   " : ".   ");
    printf("\n\n");
    sem_post(MUTEX_THINKERS);
    sem_post(MUTEX_ARRAY);
}