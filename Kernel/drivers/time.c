#include "../include/time.h"

static unsigned long ticks = 0;

void timer_handler() {
	ticks++;
}

void sleep(unsigned int ms){

    int count = ticks_elapsed();

    //Cada tick son 55 milisegundos
    while(ticks_elapsed() < count + ms/55)
        _hlt();

}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}
