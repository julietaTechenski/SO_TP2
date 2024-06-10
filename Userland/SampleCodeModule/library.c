// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "library.h"
#define MAX_SIZE 128


char charBuf;

//============================== Function Declarations ==============================

int readDelim(int fd, char * buffer, int count, char delim);
int getSeconds();

//================================= Writing Functions ==================================

int writeXY(unsigned int fd, char * string, int count, int x, int y) {
    return sys_writeXY(fd, string, count, x, y);
}

void putStringXY(char * string, int count, int x, int y) {
    writeXY(1, string, count, x, y);
}

void fputChar(int fd, char c) {
    system_write(fd, &c, 1);
}

void printPixel(unsigned char * rgb, int x, int y) {
    put_pixel(rgb[2], rgb[1], rgb[0], x, y);
}

void printRectangle(unsigned char * rgb, int x, int y, int base, int height) {
    for(int i = x ; i < x+base ; i++){
        for(int j = y ; j < y+height ; j++){
            printPixel(rgb, i, j);
        }
    }
}

//================================= similar to <stdio.h> ==================================

char getChar() {
    charBuf = 0;
    system_read(0, &charBuf, 1);
    return charBuf;
}

void printf(char * string, ...) {
    va_list argptr;
    va_start(argptr,string);   //initialize list of variable arguments with the arguments passed in ...

    int i=0;
    while(string[i]!=0){
        if(string[i] == '%'){   // if an % char is present check for format tags
            char auxInt[20];
            char * auxStr;
            switch (string[i+1]) {
                case 'd':    // format tag for int found
                    intToString(va_arg(argptr, int), auxInt);  // next argument passed w type int obtained with va_arg(argptr, int)
                    printf(auxInt);   // recursive call to the function
                    break;
                case 's':  // format tag for char * found
                    auxStr = va_arg(argptr, char*); // next argument passed w type char * obtained with va_arg(argptr, char *)
                    printf(auxStr);  // recursive call to the function
                    break;
            }
            i+=2;
        }else if(string[i]=='\\') {  // if \\ present then print the special character specified '\\' , 'n' or 't'
            switch (string[i+1]) {
                case 'n':
                    putChar('\n'); break;
                case 't':
                    putChar('\t'); break;
                case '\\':
                    putChar('\\'); break;
            }
            i+=2;
        }else
            putChar(string[i++]);
    }

    va_end(argptr);
}

void putChar(char c) {
    fputChar(1, c);
}

int scanf(char* format, ...) {
    va_list args;
    va_start(args, format);

    int argRead = 0;
    int num;
    char aux[100];

    int* argD;
    char* argS;

    int i = 0;
    while( format[i] != 0 ){
        if( format[i] == '%') {
            i++;

            num = readDelim(1, aux, 100, '\n');
            aux[num] = 0;
            argRead += num;

            putChar(' ');

            switch (format[i]) {
                case 'd':
                    argD = va_arg(args, int*);
                    *argD = 0;
                    for (int j = 0; aux[j] != 0; ++j)
                        *argD = *argD * 10 + (aux[j] - '0');
                    break;
                case 's':
                    argS = va_arg(args, char*);
                    for (int j = 0; aux[j] != 0; ++j)
                        argS[j] = aux[j];
                    break;
            }
        }
        i++;

    }

    va_end(args);
    return argRead;
}

//================================= similar to <string.h> ==================================

int strcmp(char* s1, char *s2) {
    int i = 0;
    while(s1[i] != '\0' && s2[i] != '\0'){
        if(s1[i] != s2[i])
            return 0;
        i++;
    }
    if(s1[i] == '\0' && s2[i] == '\0'){
        return 1;
    }
    return 0;
}

int strlen(char * s) {
    int i = 0;
    while(s[i] != '\0'){
        i++;
    }
    return i-1;
}

//================================= similar to <stdlib.h> ==================================

int charToInt(char c) {
    if(c < '0' || c > '9'){
        return -1;
    }
    return c - '0';
}

int customRandInRange(int min, int max) {
    int seed = getSeconds();
    int theSeed = (seed * 1103515245 + 12345) & 0x7FFFFFFF;
    return min + (theSeed % (max - min + 1));
}

void exitProgram(uint64_t argc,char *args[]) {
    while(1){
        clear_screen();
    }
}

void intToString(int n, char* s) {
    int i  = 0;
    char flag = 0;

    if(n < 0)
        flag = 1;

    do{
        s[i++] = n % 10 + '0';  //adds ASCII value
        n = n / 10;
    }while(n != 0);


    if(flag) { //checks first because all numbers end up in zero when dividing
        s[i++] = '-';
    }

    s[i]='\0';

    for (int j = 0; j < (i/2); j++) {
        char temp = s[j];
        s[j] = s[i-j-1];
        s[i - j - 1] = temp;
    }
}

//================================= similar to <math.h> ==================================

int digits(int n){
    int rta = 0;
    int aux = n;
    while(aux){
        aux/=10;
        rta++;
    }
    return rta;
}

//================================= similar to <ctype.h> ==================================

char toLower(char letter){
    if(letter >= 65 && letter <= 90){
        return letter + 32;
    } else if(letter>=97 && letter <=122) return letter;
    return -1;
}

int isVowel(char letter){
    char lett = toLower(letter);
    if(lett == 'a' || lett == 'e' || lett == 'i' || lett == 'o' || lett == 'u'){
        return 1;
    }
    return 0;
}

//================================= Reading Functions ==================================

int readDelim(int fd, char * buffer, int count, char delim) {
    if(fd != 0){
        return 0;
    }

    char c;
    int i = 0; //read characters tracker

    while((c = getChar()) != delim){ //analyze the selected key to decide whether or not to keep reading

        switch (c) {
            case 0: //The buffer is empty
                    //There was a keyBoard interruption that does not provide an ASCII
                    //There was a different interruption
                break;
            case '\b':
                if(i == 0){
                    break;
                }
                putChar(c); //Print the backspace
                i--;
                break;
            default:
                if(i >= count){
                    break;
                }
                buffer[i++] = c;
                putChar(c);
        }

    }

    return i; // Returns the amount of copied characters
}

int read(int fd, char * buffer, int count) {
    int ret = readDelim(fd,buffer,count,'\n');
    putChar('\n');
    return ret;
}

//=============================== Screen Setter Functions ================================

void setColor(unsigned char r,unsigned  char g,unsigned  char b) {
    change_color(r, g, b);
}

void incSize() {
    inc_size();
}

void decSize() {
    dec_size();
}



//================================ System Call Functions =================================

void sleep(unsigned int ms){
    _sleep(ms);
}

void clearScreen() {
    clear_screen();
}

void getRegs(){
    _getRegs();
}

int getSeconds(){
    return get_seconds_elapsed();
}

void *malloc(unsigned int size){
    return system_malloc(size);
}


void free(void * ptr){
    system_free(ptr);
}

int64_t my_createProcess(void * process, char *name, uint64_t argc, char *argv[], uint64_t isForeground){
    return system_create_process(process, name, argc, argv, isForeground);
}

int64_t getpid(){
    return system_getpid();
}

int64_t kill(uint64_t pid){
    if(pid > 0){
        return system_kill(pid);
    }
    setColor(255, 51, 51);
    printf("Invalid arguments\nTry 'help kill' for more information\n");
    return -1;
}

int64_t block(uint64_t pid){
    if(pid >= 0){
        int64_t ans = system_block(pid);
        switch (ans) {
            case 0:
                printf("Process state modified\n");
                break;
            case -1:
                printf("Process is already blocked\n");
                break;
            case -2:
                printf("Process not running\n");
                break;
        }
        return ans;
    }
    setColor(255, 51, 51);
    printf("Invalid arguments\nTry 'help block' for more information\n");
    return -1;
}

int64_t unblock(uint64_t pid){
    return system_unblock(pid);
}

void wait(uint64_t pid){
    system_wait(pid);
}

int64_t yield(){
    return system_yield();
}

int64_t nice(uint64_t pid, uint64_t newPrio){
    if(pid > 0 && 0 <= newPrio && newPrio < 10){
        return system_nice(pid, newPrio);
    }
    setColor(255, 51, 51);
    printf("Invalid arguments\nnice: usage: nice <PID> <newPriority>\nTry 'help nice' for more information\n");
    return -1;
}

int64_t sem_init(char *sem_id, uint64_t initialValue){
    return system_sem_init(sem_id,initialValue);
}

int64_t sem_wait(char *sem_id) {
    return system_sem_wait(sem_id);
}

int64_t sem_post(char *sem_id) {
    return system_sem_post(sem_id);
}

int64_t sem_close(char *sem_id) {
    return system_sem_close(sem_id);
}

int pipe(void * pipefd[2]){
    return system_pipe(pipefd);
}

int dup(void * p, int oldfd, void * pipedir){
    return system_dup(p, oldfd, pipedir);
}

int64_t change_process_state(uint64_t pid, int state){
    return system_change_process_state(pid, state);
}

int64_t print_processes(){
    setColor(20, 205, 197);
    system_print_processes();
    return 0;
}

//================================= command functions ==================================

int64_t mem_state(){
    sys_mem_state();
    return 0;
}

int64_t loop(){
    while(1){
        sleep(FIVE_SEGS);
        printf("Hi! I'm process %d!\n", getpid());
    }
    return 0;
}

int64_t cat(){
    char buffer[MAX_SIZE];
    while(read(0,buffer, MAX_SIZE) != 0) {
        printf("%s\n\n", buffer);
    }
    return 0;
}

int64_t wc(){
    int count = 0;
    char read;
    do {
        read = getChar();
        if(read != EOFILE){
            putChar(read);
        }
        if(read == '\n'){
            count++;
        }
    }
    while(read != EOFILE);
    printf("\nAmount of lines: %d\n", count);
    return 0;
}

int64_t filter(){
    char read;
    do {
        read = getChar();
        if(isVowel(read) == 0){
            putChar(read);
        }
    }
    while(read != EOFILE);
    return 0;
}