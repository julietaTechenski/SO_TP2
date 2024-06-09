#include "../include/videoDriver.h"
#include "font8x8_basic.h"

#define FONT_SIZE 2


//============================== Function Declarations ==============================

void printChar(char letter, Color foreGroundAux, Color backGroundAux);
void newLine();
void deleteChar();
void printLetter(unsigned char letter, int x, int y, Color foreGround, Color backGround);
void scrollPixel(int n);
void setSizeFactor(int sizeFactorSet);
void setBackGroundColor(Color aux);

//===================================================================================


struct vbe_mode_info_structure {
    uint16_t attributes;		// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
    uint8_t window_a;			// deprecated
    uint8_t window_b;			// deprecated
    uint16_t granularity;		// deprecated; used while calculating bank numbers
    uint16_t window_size;
    uint16_t segment_a;
    uint16_t segment_b;
    uint32_t win_func_ptr;		// deprecated; used to switch banks from protected mode without returning to real mode
    uint16_t pitch;			// number of bytes per horizontal line
    uint16_t width;			// width in pixels
    uint16_t height;			// height in pixels
    uint8_t w_char;			// unused...
    uint8_t y_char;			// ...
    uint8_t planes;
    uint8_t bpp;			// bits per pixel in this mode
    uint8_t banks;			// deprecated; total number of banks in this mode
    uint8_t memory_model;
    uint8_t bank_size;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
    uint8_t image_pages;
    uint8_t reserved0;

    uint8_t red_mask;
    uint8_t red_position;
    uint8_t green_mask;
    uint8_t green_position;
    uint8_t blue_mask;
    uint8_t blue_position;
    uint8_t reserved_mask;
    uint8_t reserved_position;
    uint8_t direct_color_attributes;

    uint32_t framebuffer;		// physical address of the linear frame buffer; write here to draw to the screen
    uint32_t off_screen_mem_off;
    uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
    uint8_t reserved1[206];
} __attribute__ ((packed));

typedef struct vbe_mode_info_structure * VBEInfoPtr;

VBEInfoPtr VBE_mode_info = (VBEInfoPtr) 0x0000000000005C00;

int currentX = 0; //Current pixel X coordinate
int currentY = 0; //Current pixel Y coordinate

static Color back[768][1024];
static int backX;
static int backY;

int sizeFactor = FONT_SIZE; //DEFAULT_SIZE_FONT multiplier

Color errorMessage = {51,51, 255};      //red

Color defBackGround = {0,0,0};          //black
Color defForeGround = {255,255,255};    //white

//============================ System Call Functions =====================================

//system call 1
void printRegisters(RegAux regs) {
    char s[20];
    int longitud;

    longitud = intToString(regs.rip, s);
    writeString(1, "RIP: ", 5);
    writeString(1, s, longitud);
    writeString(1, "\n", 1);

    longitud = intToString(regs.rax, s);
    writeString(1, "RAX: ", 5);
    writeString(1, s, longitud);
    writeString(1, "\n", 1);

    longitud = intToString(regs.rbx, s);
    writeString(1, "RBX: ", 5);
    writeString(1, s, longitud);
    writeString(1, "\n", 1);

    longitud = intToString(regs.rcx, s);
    writeString(1, "RCX: ", 5);
    writeString(1, s, longitud);
    writeString(1, "\n", 1);

    longitud = intToString(regs.rdx, s);
    writeString(1, "RDX: ", 5);
    writeString(1, s, longitud);
    writeString(1, "\n", 1);

    longitud = intToString(regs.rbp, s);
    writeString(1, "RBP: ", 5);
    writeString(1, s, longitud);
    writeString(1, "\n", 1);

    longitud = intToString(regs.rdi, s);
    writeString(1, "RDI: ", 5);
    writeString(1, s, longitud);
    writeString(1, "\n", 1);

    longitud = intToString(regs.rsi, s);
    writeString(1, "RSI: ", 5);
    writeString(1, s, longitud);
    writeString(1, "\n", 1);

    longitud = intToString(regs.r8, s);
    writeString(1, "R8: ", 5);
    writeString(1, s, longitud);
    writeString(1, "\n", 1);

    longitud = intToString(regs.r9, s);
    writeString(1, "R9: ", 5);
    writeString(1, s, longitud);
    writeString(1, "\n", 1);

    longitud = intToString(regs.r10, s);
    writeString(1, "R10: ", 5);
    writeString(1, s, longitud);
    writeString(1, "\n", 1);

    longitud = intToString(regs.r11, s);
    writeString(1, "R11: ", 5);
    writeString(1, s, longitud);
    writeString(1, "\n", 1);

    longitud = intToString(regs.r12, s);
    writeString(1, "R12: ", 5);
    writeString(1, s, longitud);
    writeString(1, "\n", 1);

    longitud = intToString(regs.r13, s);
    writeString(1, "R13: ", 5);
    writeString(1, s, longitud);
    writeString(1, "\n", 1);

    longitud = intToString(regs.r14, s);
    writeString(1, "R14: ", 5);
    writeString(1, s, longitud);
    writeString(1, "\n", 1);

    longitud = intToString(regs.r15, s);
    writeString(1, "R15: ", 5);
    writeString(1, s, longitud);
    writeString(1, "\n", 1);
}

//system call 2
void putPixel(Color hexColor, uint64_t x, uint64_t y) {
    Color * framebuffer = (Color *)((uint64_t) VBE_mode_info->framebuffer);
    uint64_t offset = x  + y * VBE_mode_info->width;
    framebuffer[offset] = hexColor;
}




int writeString(unsigned int fd, char * string, int count) {
    Color aux = defForeGround;
    if (fd == 2){
        aux = errorMessage;
    } else if (fd != 1){     //A file descriptor other than 1 or 2 was not implemented
        return 0;
    }
    for (int i = 0 ; i < count ; ++i){
        printChar(string[i], aux, defBackGround);
    }

    return count;
}

//system call 5
void clearScreen() {
    currentX = 0;
    scrollPixel(VBE_mode_info->height);
    currentY = 0;
}

//system call 6
void setForeGroundColor(Color aux){
    defForeGround = aux;
}

//system call 7
void incSize() {
    setSizeFactor(++sizeFactor);
}

//system call 8
void decSize() {
    if(sizeFactor > 1){
        setSizeFactor(--sizeFactor);
    }
}

//system call 12
int writeXY(unsigned int fd, char * string, int count, int x, int y) {
    //saves writing coordinates
    int auxX = currentX;
    int auxY = currentY;

    //updates coordinates
    currentX = x;
    currentY = y;

    //writes
    int ret = writeString(fd, string, count);

    //resets coordinates
    currentX = auxX;
    currentY = auxY;
    return ret;
}

//============================== Other Functions =======================================

//external use-------------------------------------------------------------------------

void backUp() {
    memcpy((void *)back, (const void *) ((uint64_t)VBE_mode_info->framebuffer), (uint64_t) sizeof(Color) * 768 * 1024);
    backX = currentX;
    backY = currentY;
}

void refresh() {
    memcpy((void *) ((uint64_t)VBE_mode_info->framebuffer),(const void *) back, (uint64_t)sizeof(Color) * 768 * 1024);
    currentX = backX;
    currentY = backY;
}

//internal use-------------------------------------------------------------------------

//Auxiliary function that prints characters and also keeps track of the current position of the cursor
void printChar(char letter, Color foreGroundAux, Color backGroundAux) {
    if(letter == '\n'){
        newLine();
        return;
    }

    if(letter == '\b'){
        deleteChar();
        return;
    }

    if (letter == '\t'){
        for (int i = 0 ; i < 4 ; ++i)
            printChar(' ', foreGroundAux, backGroundAux);
        return;
    }

    printLetter(letter, currentX, currentY, foreGroundAux,backGroundAux);

    currentX += (sizeFactor*DEFAULT_SIZE);
    if (currentX + (sizeFactor*DEFAULT_SIZE) > VBE_mode_info->width){
        newLine();
    }

}

//Adds new line
void newLine() {
    currentX = 0;
    if(currentY + sizeFactor*DEFAULT_SIZE*2 >= VBE_mode_info->height){
        scrollPixel(sizeFactor * DEFAULT_SIZE);
    } else {
        currentY += sizeFactor * DEFAULT_SIZE;
    }
}

//Erases the last character written
void deleteChar() {
    if(currentX == 0){
        if(currentY != 0) {
            currentY -= sizeFactor * DEFAULT_SIZE;
            currentX = ((VBE_mode_info->width / (sizeFactor * DEFAULT_SIZE)) * (sizeFactor * DEFAULT_SIZE)) - (sizeFactor*DEFAULT_SIZE);
        }
    } else {
        currentX -= sizeFactor * DEFAULT_SIZE;
    }

    printLetter(' ', currentX, currentY, defForeGround,defBackGround);
}

//Auxiliary function to print characters by their bitmap
void printLetter(unsigned char letter, int x, int y, Color foreGround, Color backGround) {
    char mask[DEFAULT_SIZE] = {1,2,4,8,16,32,64,128};
    char * rowData = font8x8_basic[letter];

    // Recorre cada fila de la letra
    for (int fila = 0 ; fila < DEFAULT_SIZE ; fila++) {
        // Recorre cada bit en la fila de la letra
        for (int col = 0 ; col < DEFAULT_SIZE ; col++) {

            char aux = (rowData[fila] & mask[col]);
            for (int i = 0 ; i < sizeFactor ; ++i) {
                for (int j = 0 ; j < sizeFactor ; ++j) {
                    if (aux != 0){
                        putPixel(foreGround, x + col * sizeFactor + i, y + fila * sizeFactor + j);
                    } else {
                        putPixel(backGround, x + col * sizeFactor + i, y + fila * sizeFactor + j);
                    }
                }
            }
        }
    }

}

//Reprints the screen in higher pixels (delete the first n rows of pixels)
void scrollPixel(int n) {
    Color * framebuffer = (Color *)((uint64_t) VBE_mode_info->framebuffer);
    uint64_t offset = 0;
    for (int i = 0 ; i < VBE_mode_info->height - n ; ++i) {
        for (int j = 0 ; j < VBE_mode_info->width ; ++j){
            framebuffer[offset+j] = framebuffer[offset + j + VBE_mode_info->width*n];
        }
        offset += VBE_mode_info->width;
    }

    while(n > 0) {
        for (int i = 0 ; i < VBE_mode_info->width ; ++i){
            framebuffer[offset+i] = defBackGround;
        }
        offset += VBE_mode_info->width;
        n--;
    }
}

//Edits font size
void setSizeFactor(int sizeFactorSet) {
    sizeFactor = sizeFactorSet;
}

//Modifies background color
void setBackGroundColor(Color aux) {
    defBackGround = aux;
}

