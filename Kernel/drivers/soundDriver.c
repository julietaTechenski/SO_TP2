// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../include/soundDriver.h"

void playSound(uint32_t freq, uint32_t ms){
    if(freq != 0){
        sound(freq);
    }
    sleep(ms);
    noSnd();
}

