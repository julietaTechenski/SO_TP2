
#ifndef VIDEODRIVER_H
#define VIDEODRIVER_H

#include <stdint.h>
#include "lib.h"

//Struct that simulates the pushState that interrupts.asm does to in other to get the rsp
typedef struct Reg{
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rbp;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;
} RegAux;


typedef struct Color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
}Color;

/**
 * @def prints the current state of registers
 * @param regs struct with all registers
 */
void printRegisters(RegAux r);

/**
 * @def auxiliary function to write on the screen
 * @param hexColor color to print pixel in rgb format
 * @param x horizontal coordinate where to print pixel
 * @param y vertical coordinate where to print pixel
 */
void putPixel(Color hexColor, uint64_t x, uint64_t y);

/**
 * @def writes on terminal
 * @param fd file descriptor
 * @param string char * to print
 * @param count lenght of string
 * @return number of characters written
 */
int writeString(unsigned int fd, char * string, int count);

/**
 * @def clears the terminal screen
 */
void clearScreen();

/**
 * @def modifies the font color
 * @param aux color in rgb format
 */
void setForeGroundColor(Color aux);

/**
 * @def increase font size
 */
void incSize();

/**
 * @def decrease font size
 */
void decSize();

/**
 * @def writes on terminal in specified coordinate
 * @param fd file descriptor
 * @param string char * to print
 * @param count lenght of string
 * @param x horizontal coordinate where to print pixel
 * @param y vertical coordinate where to print pixel
 * @return
 */
int writeXY(unsigned int fd, char * string, int count, int x, int y);

/**
 * @def backs up the cursors coordinate
 */
void backUp();

/**
 * @def backs up the cursors coordinate
 */
void refresh();


#endif //VIDEODRIVER_H
