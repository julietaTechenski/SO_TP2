#include "../include/phylos.h"
#define MAX_PHYLOS 10
#define MIN_PHYLOS 3
static int thinkers = 5;

static char * palillos[MAX_PHYLOS-1];  // semaphore array

static int state[MAX_PHYLOS-1]; // print phylos array

static char*  mutex = "access_array";

void phylos(){
    // initializing
    sem_init(mutex, 1);

    for(int i= 0; i < thinkers; i++){
        char *sem = (char*)malloc(sizeof(char)*2); // me guardo la direccion de chars
        intToString(i, sem);
        sem_init(sem, 1);
        palillos[i] = sem;
    }

    while(1){
        switch(getChar()){
            case 'a':
                add();
                clear_screen();
                sem_wait(mutex);
                reprint();
                sem_post(mutex);
                break;
            case 'r':
                remove();
                clear_screen();
                sem_wait(mutex);
                reprint();
                sem_post(mutex);
                break;
            default:
                break;
        }
        phylo(customRandInRange(0, thinkers-1));
    }
}


void phylo(int i){
    // come necesito modificar el valor de array de filósofos
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
    sleep(3);
}

void eat(int phy){
    sem_wait(mutex);
    state[phy] = 1;
    reprint();
    sem_post(mutex);
    sleep(3);
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

void reprint(){
    printf("CANTIDAD DE FILOSOFOS: %d\n",thinkers);
    for(int i = 0; i< thinkers; i++){
        printf("%s", state[i] == 1? "\tE\t": "\t.\t");
    }
    printf("\n\n");
}