#ifndef PRINTTIME_H
#define PRINTTIME_H

#include "../../include/library.h"


typedef struct{
    unsigned int segs;
    unsigned int mins;
    unsigned int hours;
} TimeFormat;

// The Assembly function that obtains the time
extern void getTimeRTC(TimeFormat* t);

void printTime(uint64_t argc,char * args[]);


#endif //PRINTTIME_H
