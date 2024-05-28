
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
#include <stdint.h>
#include <stdarg.h>

#define MAX_SIZE 100
#define USERNAME_MAX_SIZE 20
#define AMOUNT_COMMANDS 10


typedef struct tcommand{
    char * name;                //command
    void (*fn)(char * args[]);  //function
}tcommand;

void shell();

void test_mm_wrapper(char * args[]);

#endif //SHELL_H
