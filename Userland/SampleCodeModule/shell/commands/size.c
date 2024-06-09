
#include "../include/size.h"

static char* ADD = "+";
static char* SUBSTRACT = "-";

//================================ Command SIZE function ================================

void size(uint64_t argc,char * args[]){
    // User inserted 'size +'
    if(strcmp(ADD, args[0])){
        incSize();
        return;
    }
    // User inserted 'size -'
    if(strcmp(SUBSTRACT, args[0])){
        decSize();
        return;
    }
    // User inserted invalid parameter
    setColor(255, 51, 51);
    printf("Invalid parameters\nOptions are '+' or '-'.\n");
    setColor(255, 255, 255);

    return;
}