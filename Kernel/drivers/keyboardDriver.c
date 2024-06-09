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
        writeString(1, &eofile, 1);
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
    if(fd != 0){
        return 0;
    }
    int i = 0;
    char c;
    uint64_t pid = getPID();
    blocked->pid = pid;
    blocked->count = count;
    block(pid);
    while(i < count && (c = getEntry()) != 0){
        buffer[i++] = c;
    }
    return i;
}
