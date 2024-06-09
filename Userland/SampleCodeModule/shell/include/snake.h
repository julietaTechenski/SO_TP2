
#ifndef SNAKE_H
#define SNAKE_H

#include "../../include/library.h"



//Screen dimensions
#define BASE 1024
#define HEIGHT 768

#define MENU_WIDTH 256

//Due to the left menu, the board is 768x768
#define SCREEN_DIM 768

//Square data
#define CANT_SQUARES 24
#define SQUARE_SIZE (768/24)

//Player 1 controllers
#define UP1 'w'
#define RIGHT1 'd'
#define DOWN1 's'
#define LEFT1 'a'

//Player 2 controllers
#define UP2 'i'
#define RIGHT2 'l'
#define DOWN2 'k'
#define LEFT2 'j'

//Key to exit game
#define EXIT 'x'

//Key to restart the game if player lose
#define RESTART ' '

//Controller variables
#define CONTROLLER_BASE 39
#define CONTROLLER_HEIGHT 26
#define CONTROLLER_PIXEL_THICKNESS 3

//Apple variables
#define APPLE_BASE 11
#define APPLE_HEIGHT 12
#define APPLE_PIXEL_THICKNESS 2
#define APPLE_WIDTH 12

//Apple first position
#define START_X 11
#define START_Y 4

//Snakes first direction
#define S1_DIREC 0
#define S2_DIREC 1

//Initial score
#define START_SCORE 0

//Initial lives
#define START_LIVES 3

//If players are playing (if not lost) or not (lost)
#define PLAYING 1
#define NOT_PLAYING 0

//Variable to define max snake size
#define MAX_POINTS (24*24)

//Game level (varies speed)
#define LEVEL1 1000
#define LEVEL2 500
#define LEVEL3 250

//Notes Library (sound effects)
#define A2 110
#define B2 123
#define C2 65
#define D2 73
#define E2 82
#define F2 87
#define G2 98
#define C3 130
#define G3 196
#define A4 440
#define Bb4 466
#define B4 493
#define C4 261
#define D4 293
#define E4 329
#define F4 349
#define G4 392
#define C5 523
#define R 0

extern void _playSound(int freq, int durationInSeconds);

typedef struct {
    int x;
    int y;
} Point;

void play(uint64_t argc,char * args[]);


#endif //SNAKE_H
