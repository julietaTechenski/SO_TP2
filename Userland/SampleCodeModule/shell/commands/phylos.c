// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../include/phylos.h"
#define MAX_PHYLOS 10
#define MIN_PHYLOS 3

void phylo(int i);
void think(int phy);
void eat(int phy);
void add();
void remove();
void reprint();

static int thinkers = 5;

static char * palillos[MAX_PHYLOS-1];  // semaphore array

static int state[MAX_PHYLOS-1]; // print phylos array

static char*  mutex = "access_array";
static char*  char_mutex = "char_mutex";
static char*  controllers_mutex = "controllers_mutex";


void phylos(){
    // initializing
    sem_init(mutex, 1);
    sem_init(char_mutex, 1);

    char * c = malloc(sizeof(char));
    char* argv[1];
    argv[0] = c;


    my_createProcess(&controllers_handler, "controllers_handler", 1, argv, 1);

    for(int i= 0; i < thinkers; i++){
        char *sem = (char*)malloc(sizeof(char)*2); // me guardo la direccion de chars
        intToString(i, sem);
        sem_init(sem, 1);
        palillos[i] = sem;
    }

    while(1){
        sem_wait(char_mutex);
        if(*c!=0) {
            switch (*c) {
                case 'a':
                    *c = 0;
                    sem_post(char_mutex);
                    add();
                    clear_screen();
                    sem_wait(mutex);
                    reprint();
                    sem_post(mutex);
                    break;
                case 'r':
                    *c=0;
                    sem_post(char_mutex);
                    remove();
                    clear_screen();
                    sem_wait(mutex);
                    reprint();
                    sem_post(mutex);
                    break;
                default:
                    *c =0;
                    sem_post(char_mutex);
                    break;
            }
        } else {
            sem_post(char_mutex);
        }
        phylo(customRandInRange(0, thinkers-1));
    }
}


void phylo(int i){
    int left = i;
    int right = (i + 1) % thinkers;
    if(i % 2){ //odd philosopher
        sem_wait(palillos[left]);
        sem_wait(palillos[right]);
    } else { //even philosopher
        sem_wait(palillos[right]);
        sem_wait(palillos[left]);
    }
    eat(i);
    sem_post(palillos[left]);
    sem_post(palillos[right]);
    think(i);
}

void think(int phy){
    sem_wait(mutex);
    state[phy] = 0;
    reprint();
    sem_post(mutex);
}

void eat(int phy){
    sem_wait(mutex);
    state[phy] = 1;
    reprint();
    sem_post(mutex);
}

void add(){
    if(thinkers+1<=MAX_PHYLOS){
        thinkers++;
        char *s = (char*) malloc(sizeof(char)*1);
        intToString(thinkers-1, s);
        sem_init(s, 1);
        palillos[thinkers-1] = s;
    }
}

void remove(){
    if(thinkers-1>MIN_PHYLOS){
        free(palillos[thinkers-1]);
        thinkers--;
    }
}

void controllers_handler(int argc, char * argv[]){
    while(1){
        char aux = getChar();
        sem_wait(char_mutex);
        *argv[0] = aux;
        sem_post(char_mutex);
    }
}

void reprint(){
    printf("%d ->\t ",thinkers);
    for(int i = 0; i< thinkers; i++){
        printf("%s",state[i] == 1? "E   ": ".   ");
    }
    printf("\n\n");
}