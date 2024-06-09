#include "../include/printTime.h"

//================================ Command TIME function ================================

void printTime(uint64_t argc,char * args[]){
    setColor(192, 192, 192);

    // Where time will be stored
    TimeFormat time;

    // Obtains the time
    getTimeRTC(&time);

    // Modifies time to Argentina's timezone (-3hs)
    // because this project was made in Argentina
    if(time.hours >= 3) {
        time.hours -= 3;
    }else {
        time.hours += 21;
    }
    // Prints the time on the screen
    printf("\n%d:%d:%d\n\n",time.hours,time.mins,time.segs);

    return;
}