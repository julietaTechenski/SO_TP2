#ifndef LIBRARY_H
#define LIBRARY_H

#include <stdarg.h>
#include <stdint.h>

//EXTERNS ---------------------------------------------------------------------------------

extern int system_read(unsigned int fd, char * buffer, int count);
extern void system_write(unsigned int fd, char * buffer, int count);
extern void change_color(char r,  char g,  char b);
extern void inc_size();
extern void dec_size();
extern void clear_screen();
extern int get_seconds_elapsed();
extern void put_pixel(unsigned char r, unsigned char g, unsigned char b, int x, int y);
extern void change_color(char r,  char g,  char b);
extern void _sleep(unsigned int seconds);
extern void _getRegs();
extern int sys_writeXY(unsigned int fd, char * string, int count, int x, int y);
extern void * system_malloc(unsigned int size);
extern void system_free(void * ptr);
extern int64_t system_getpid(void);
extern int64_t system_fork(char *name, uint64_t argc, char *argv[]);
extern int64_t system_kill(uint64_t pid);  //cambiar pid_t por variable del tipo del id del proceso
extern int64_t system_block(uint64_t pid);
extern int64_t system_unblock(uint64_t pid);
extern int64_t system_wait(char *sem_id);
extern int64_t system_yield();
extern int64_t system_nice(uint64_t pid, uint64_t newPrio);


//FUNCTIONS -------------------------------------------------------------------------------


/**
 * @def prints 'string' in positions (x, y) on the screen
 *
 * @param string what you want to print
 * @param count amount of characters to print
 * @param x horizontal coordinate to print in
 * @param y vertical coordinate to print in
 */
void putStringXY(char * string, int count, int x, int y);

/**
 * @def prints pixel on the screen
 *
 * @param rgb color to print in
 * @param x horizontal coordinate to print in
 * @param y vertical coordinate to print in
 */
void printPixel(unsigned char * rgb, int x, int y);

/**
 * @def prints rectangle of pixels on the screen
 *
 * @param rgb color to print in
 * @param x horizontal coordinate to print in
 * @param y vertical coordinate to print in
 * @param base base of the rectangle to print
 * @param height height of the rectangle to print
 */
void printRectangle(unsigned char * rgb, int x, int y, int base, int height);

/**
 * @def takes a single input character from standard input
 *
 * @return char read or 0
 */
char getChar();

/**
 * @def Attempt to recreate the printf function with variable number of arguments implemented in the c lbrary
 *
 * @param string parameter that contains a pointer to a series of char
 * @param ...  It can optionally contain format tags that are replaced by the values specified.
 *              Format tags prototype is %[flags]specifier, which is explained below:
 *             | SPECIFIER   |    OUTPUT                    |
 *             | d           |    Signed decimal integer    |
 *             | s           |    String of characters      |
 */
void printf(char * string, ...);

/**
 * @def prints char on the screen
 *
 * @param c what you want to print
 */
void putChar(char c);

/**
 * @def stands for Scan Formatted String.
 *      It is used to read data from stdin
 *      and then writes the result into the given arguments.
 * @param format is a string that contains the format specifiers(s).
 *             | SPECIFIER   |    OUTPUT                    |
 *             | d           |    Signed decimal integer    |
 *             | s           |    String of characters      |
 * @param ... variables where to obtain the specifiers
 * @return amount of arguments successfully read
 */
int scanf(char* format, ...);

/**
 * @def used for string comparison
 * @param s1 string to compare
 * @param s2 other string to compare
 * @return 1 if they are the same, 0 if not
 */
int strcmp(char* s1, char *s2);

/**
 * @def length of string
 * @param s string to get length of
 * @return length of string s
 */
int strlen(char * s);


/**
 * @def turns a char into an integer
 * @param c char to turn
 * @return char converted
 */
int charToInt(char c);

/**
 * @def generates a random number between min and max
 * @param min minimum value of number
 * @param max maximum value of number
 * @return number generated
 */
int customRandInRange(int min, int max);

/**
 * @def terminates process
 * @param args 0
 */
void exitProgram(char *args[]);

/**
 * @def turns an integer into a string, char *
 * @param n number to turn
 * @param s char * where to save newString
 */
void intToString(int n, char* s);

/**
 * @def returns the number of digits in a number
 * @param n number to get digit amount
 * @return digit amount of number
 */
int digits(int n);

/**
 * @def reads from fd specified and stores it in the buffer
 * @param fd : file descriptor
 * @param buffer : destination of read characters
 * @param n : amount of characters to read
 * @return
 */
int read(int fd, char * buffer, int count);

/**
 * @def sets the color of the font to print
 * @param r red
 * @param g green
 * @param b blue
 */
void setColor(unsigned char r,unsigned  char g,unsigned  char b);

/**
 * @def increases the font size
 */
void incSize();

/**
 * @def decreases the font size
 */
void decSize();

/**
 * @def deletes everything on the screen
 */
void clearScreen();

/**
 * @def suspends the execution of the calling thread until
 * the time specified in milliseconds has elapsed
 * @param milliseconds
 */
void sleep(unsigned int ms);

/**
 * @def obtains the registers' current values at the time
 */
void getRegs();

/**
 *
 * @param size
 * @return
 */
void *malloc(unsigned int size);

/**
 *
 * @param ptr
 */
void free(void * ptr);

/**
 *
 * @return current process id
 */
int64_t getpid(void);

/**
 * @def creates a child process
 * @return child process pid is returned in the parent and 0 is returned in the child.
 * On failure, -1 is returned in the parent
 */
int64_t fork(char *name, uint64_t argc, char *argv[]);


/**
 * @def terminates de process specified with the pid
 * @param pid
 * @return 0 on success, else -1
 */
int64_t kill(uint64_t pid);

/**
 *
 * @param pid
 * @return
 */
int64_t block(uint64_t pid);

/**
 *
 * @param pid
 * @return
 */
int64_t unblock(uint64_t pid);

/**
 *
 * @param sem
 * @param pshared
 * @param value
 * @return
 */
int sem_init(/*sem_t*/int *sem, int pshared, unsigned int value);

/**
 * @def wait for process to change state
 * @param wstatus
 * @return
 */
int64_t wait(char *sem_id);

/**
 * @def process voluntarily gives up control of the processor
 * @return
 */
int64_t yield();

/**
 *@def starts a process with a set priority or changes a process' priority
 * @param pid
 * @param newPrio
 * @return
 */
int64_t nice(uint64_t pid, uint64_t newPrio);

#endif //LIBRARY_H


