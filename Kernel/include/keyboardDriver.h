#ifndef KEYBOARDDRIVER_H
#define KEYBOARDDRIVER_H

#include "time.h"
#include "videoDriver.h"
#include "interrupts.h"
#include "process.h"

#define INITIAL_SIZE 10

#define LSHIFT 0x2A
#define LSHIFT_RELEASE (LSHIFT+0x80)
#define RSHIFT 0x36
#define RSHIFT_RELEASE (RSHIFT+0x80)
#define BLOQ_MAYUS 0x3A
#define CTRL 0x1D
#define CTRL_RELEASE (CTRL + 0x80)
#define F2 60
#define F3 61

#define EOFILE '\377'

//Scancode Set 2 (key=scancode - value=ascii)
const static char keyboardTable[][2] = {
        {0,0},
        {0,0},
        {'1', '!'},
        {'2', '"'},
        {'3', '#'},
        {'4', '$'},
        {'5', '%'},
        {'6', '&'},
        {'7', '/'},
        {'8', '('},
        {'9', ')'},
        {'0', '='},
        {'\'', '?'},
        {0, 0},
        {'\b', '\b'},
        {'\t', '\t'},
        {'q', 'Q'},
        {'w', 'W'},
        {'e', 'E'},
        {'r', 'R'},
        {'t', 'T'},
        {'y', 'Y'},
        {'u', 'U'},
        {'i', 'I'},
        {'o', 'O'},
        {'p', 'P'},
        {0, 0},
        {'+', '*'},
        {'\n', '\n'},
        {0, 0},
        {'a', 'A'},
        {'s', 'S'},
        {'d', 'D'},
        {'f', 'F'},
        {'g', 'G'},
        {'h', 'H'},
        {'j', 'J'},
        {'k', 'K'},
        {'l', 'L'},
        {'<', '>'},
        {'{', '['},
        {'}', ']'},
        {0, 0},
        {'\\', '|'},
        {'z', 'Z'},
        {'x', 'X'},
        {'c', 'C'},
        {'v', 'V'},
        {'b', 'B'},
        {'n', 'N'},
        {'m', 'M'},
        {',', ';'},
        {'.', ':'},
        {'-', '_'},
        {0, 0},
        {0, 0},
        {0, 0},
        {' ', ' '},
};

/**
 * @def Keyboard IRQ
 * @param infoRegs Struct of registers
 */
void keyboard_handler(uint64_t infoRegs);

/**
 * @def Reads from fd into buffer up to "count" characters
 * @param fd file descriptor
 * @param buffer char * to store string
 * @param count maximum amount of characters
 * @return number of characters read
 */
int read(unsigned int fd, char * buffer, int count);


#endif //KEYBOARDDRIVER_H
