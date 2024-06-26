// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "keyboardDriver.h"

#define SEM "keyboard_sem"

//Auxiliary functions
void addEntry(char c);

char getEntry();


static char myBuffer[INITIAL_SIZE];


static int first = 0;
static int last = 0;

static int shiftFlag = 0;
static int bloqMayus = 0;
static int ctrlFlag = 0;

void initKeyboard(){
    my_sem_open(SEM, 0);
}

//Keyboard IRQ
void keyboard_handler(uint64_t infoRegs) {
    unsigned char keyCode = getKeyPressed();
    RegAux *aux = (RegAux *) infoRegs;

    switch (keyCode) {
        case CTRL:
            ctrlFlag = 1;
            return;
        case CTRL_RELEASE:
            ctrlFlag = 0;
            return;
        case LSHIFT:
            shiftFlag = 1;
            return;
        case LSHIFT_RELEASE:
            shiftFlag = 0;
            return;
        case RSHIFT:
            shiftFlag = 1;
            return;
        case RSHIFT_RELEASE:
            shiftFlag = 0;
            return;
        case BLOQ_MAYUS:
            bloqMayus = !bloqMayus;
            return;
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
    if (keyCode >= 58) {
        return;
    }

    //Differentiate between letters and special characters when pressing shift and capital letters
    char key = keyboardTable[keyCode][0];
    if (key == 'c' && ctrlFlag) {
        killForeground();
        return;
    } else if (key == 'd' && ctrlFlag) {
        addEntry(EOFILE);
        return;
    } else if (key >= 'a' && key <= 'z')
        key = keyboardTable[keyCode][bloqMayus ^ shiftFlag];
    else
        key = keyboardTable[keyCode][shiftFlag];

    //Only adds it if it has an ASCII representation
    if (key != 0) {
        addEntry(key);
    }
}

//Adds entry to buffer
void addEntry(char c) {
    if (last == (first - 1) % INITIAL_SIZE)
        return;
    myBuffer[last] = c;
    last = (last + 1) % INITIAL_SIZE;
    my_sem_post(SEM);

}

//Gets first entry from the buffer
char getEntry() {
    my_sem_wait(SEM);
    char ret = myBuffer[first];
    first = (first + 1) % INITIAL_SIZE;
    return ret;
}

//SysCall 3
int read(unsigned int fd, char *buffer, int count) {
    uint64_t pid = getPID();
    PCB *p = findProcess(pid);

    if (p != NULL) {
        if (!p -> isForeground) {
            return 0;
        }
        int i = 0;
        if (p->fd[STDIN] != NULL) { // pipe read
            return readPipe(p->fd[STDIN], buffer, count);
        } else { // reads from keyboard interrupts
            char c;
            while (i < count) {
                c = getEntry();
                if(c == EOFILE || c == '\n')
                    break;

                switch (c) {
                    case '\b':
                        if(i == 0)
                            break;
                        writeString(1,&c,1); //Print the backspace
                        i--;
                        break;
                    default:
                        buffer[i++] = c;
                        writeString(1,&c,1); //Print the backspace
                }
            }
            writeString(1,"\n",1);
            return i;
        }
    }
    return 0; // error
}


//system call 4
int write(unsigned int fd, char *string, int count) {
    string[count] = '\0';
    PCB *p = findProcess(getPID());
    if (p->fd[STDOUT] != NULL) {  // pipe write
        writePipe(p->fd[STDOUT], string, count);
    } else { // STDOUT
        writeString(STDOUT, string, count);
    }
    return count;
}




