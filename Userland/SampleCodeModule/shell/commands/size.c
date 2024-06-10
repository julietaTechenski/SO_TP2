// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "../include/size.h"

static char* ADD = "+";
static char* SUBSTRACT = "-";

//================================ Command SIZE function ================================

void size(uint64_t argc,char * args[]){
    // User inserted 'size +'
    if(strcmp(ADD, args[0]) > 0){
        incSize();
        return;
    }
    // User inserted 'size -'
    if(strcmp(SUBSTRACT, args[0]) > 0){
        decSize();
        return;
    }
    // User inserted invalid parameter
    setColor(255, 51, 51);
    printf("Invalid parameters\nOptions are '+' or '-'.\n");
    setColor(255, 255, 255);

    return;
}