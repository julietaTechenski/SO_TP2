#include "keyboardDriver.h"

//Auxiliary functions
void addEntry(char c);
char getEntry();


static char myBuffer[INITIAL_SIZE];

static int first = 0;
static int last = 0;

static int shiftFlag = 0;
static int bloqMayus = 0;
static int ctrlFlag = 0;

typedef struct bproc {
    uint64_t pid;
    int count;
}bproc;


static bproc * blocked;

//Keyboard IRQ
void keyboard_handler(uint64_t infoRegs){

    unsigned char keyCode = getKeyPressed();
    RegAux * aux = (RegAux *) infoRegs;

    switch (keyCode) {
        case CTRL:
            ctrlFlag=1; return;
        case CTRL_RELEASE:
            ctrlFlag=0; return;
        case LSHIFT:
            shiftFlag=1; return;
        case LSHIFT_RELEASE:
            shiftFlag=0; return;
        case RSHIFT:
            shiftFlag=1; return;
        case RSHIFT_RELEASE:
            shiftFlag=0; return;
        case BLOQ_MAYUS:
            bloqMayus= !bloqMayus; return;
        case F2:
            backUp();
            clearScreen();
            printRegisters(*aux);
            writeString(1, "Press F3 to exit", 16);
            while (getKeyPressed() != F3) {
                _hlt();
            }
            refresh();
            return;
    }

    //If it is a scanCode of a key released or out of index, it ignores it
    if(keyCode >= 58){
        return;
    }

    //Differentiate between letters and special characters when pressing shift and capital letters
    char key = keyboardTable[keyCode][0];
    if(key=='c' && ctrlFlag) {
        killForeground();
        return;
    }else if(key=='d' && ctrlFlag) {
        char eofile = EOFILE;
        //writeString(1, &eofile, 1);
        addEntry(eofile);
        return;
    }else if(key >= 'a' && key <= 'z')
        key = keyboardTable[keyCode][bloqMayus^shiftFlag];
    else
        key = keyboardTable[keyCode][shiftFlag];

    //Only adds it if it has an ASCII representation
    if(key != 0){
        blocked->count--;
        addEntry(key);
    }
}

//Adds entry to buffer
void addEntry(char c){
    if(last == (first-1)% INITIAL_SIZE)
        return;
    myBuffer[last] = c;
    last = (last + 1) % INITIAL_SIZE;
    if(blocked->count==0){
        unblock(blocked->pid);
    }
}

//Gets first entry from the buffer
char getEntry(){
    if(first == last){
        return 0;
    }
    char ret = myBuffer[first];
    first = (first + 1) % INITIAL_SIZE;
    return ret;
}

//SysCall 3
int read(unsigned int fd, char * buffer, int count){
    uint64_t pid = getPID();
    blocked->pid = pid;
    blocked->count = count;
    block(pid);
    PCB * p = findProcess(pid);
    if( p != NULL){
        int i = 0;
        if(p->fd[fd] != NULL){ //pipe read
            int c = count;
            char *rReady = "rReady";
            my_sem_open(rReady, 0); // read => rReady = 1

            char *wReady = "wReady";
            my_sem_open(wReady, 0);

            my_sem_wait(wReady); //waits for input

            while(c > 0 && p->fd[STDIN][i] !=0){
                for(;c > 0 && p->fd[STDIN][i] !=0 && i <128; i++, c--){
                    buffer[i] = p->fd[STDIN][i];
                }
                if(i==128){
                    i=0;
                    my_sem_post(rReady);
                    my_sem_wait(wReady);
                }
                if(p->fd[STDIN][i-1] ==0){
                    i--;
                }
            }
            my_sem_post(rReady);
            return 0;
        }else{ //reads from keyboard interrupts
            char c;
            while(i < count && (c = getEntry()) != 0){
                buffer[i++] = c;
            }
            return i;
        }
    }
    return 0; //error
}


//system call 4
int write(unsigned int fd, char * string, int count){
    uint64_t ppid = getPID();
    PCB *p = findProcess(ppid);
    if(p->fd[fd] != NULL){  //pipe write
        char *rReady = "rReady";
        my_sem_open(rReady, 0);

        char *wReady = "wReady";
        my_sem_open(wReady, 0);

        int i=0;
        int c = count;
        while(c > 0 && string[i] != 0){
            for( ;c > 0 && string[i]!=0 && i <128; i++, c--){
                p->fd[fd][i] = string[i];
            }
            if(i==128){
                i=0; //restart buffer pos counters
                my_sem_post(wReady);
                my_sem_wait(rReady);  // waiting buffer to be read
            }
            if(string[i-1]==0){
                my_sem_post(wReady);
                i--;
            }
        }
        my_sem_post(wReady);
    } else{ // STDOUT
        writeString(STDOUT, string, count);
    }
    return count;
}

