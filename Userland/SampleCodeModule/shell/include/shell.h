
#ifndef SHELL_H
#define SHELL_H

#include "library.h"
#include "help.h"
#include "printTime.h"
#include "size.h"
#include "snake.h"
#include "zeroexception.h"
#include "ioexception.h"
#include "introSys.h"
#include "test_mm.h"
#include "test_processes.h"
#include "test_prio.h"
#include "test_sync.h"
#include "phylos.h"
#include "pipe.h"
#include <stdint.h>
#include <stdarg.h>

#define MAX_SIZE 100
#define USERNAME_MAX_SIZE 20
#define AMOUNT_COMMANDS 24


typedef struct tcommand{
    char * name;                //command
    void * fn;  //function
}tcommand;

void shell();


#endif //SHELL_H
