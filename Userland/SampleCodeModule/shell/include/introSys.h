
#ifndef MUSIC_H
#define MUSIC_H

#include "library.h"

#define MUSIC 15

#define E4 329
#define D4 293
#define C4 261
#define G3 196
#define G4 392
#define F4 349
#define Bb4 466
#define A4 440
#define R 0

void playNotes() ;

extern void _playSound(int freq, int durationInSeconds);


#endif //MUSIC_H
