#ifndef SOUNDDRIVER_H
#define SOUNDDRIVER_H

#include <stdint.h>
#include "time.h"

void playSound(uint32_t freq, uint32_t time);
extern void sound(uint32_t f);
extern void noSnd();

#endif //TP_SOUNDDRIVER_H
