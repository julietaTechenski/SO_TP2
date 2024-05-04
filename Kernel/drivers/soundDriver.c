#include "../include/soundDriver.h"

void playSound(uint32_t freq, uint32_t ms){
    if(freq != 0){
        sound(freq);
    }
    sleep(ms);
    noSnd();
}

