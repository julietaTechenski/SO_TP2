
#include "../include/snake.h"

/*
 * The screen is 1024x768.
 * A menu bar was created at the left of the screen (1024-768), this way
 * a perfect square was created for the board.
 * The menu contains the scores and simple instructions.
 * The board was formed into a matrix of 24x24.
 *
 * The snakes always start in the same spots. Starting positions were programmers decisions.
 * Snakes move one square at a time.
 * Snake starting size is 3 and was programmers decision.
 * Snakes are stored in a matrix and there is a Points array for order.
 *
 * If a snake "eats" an apple, then it grows by 1 and scores 1 point.
 *
 * Player can lose two different ways:
 *              - Hitting a boarder with head.
 *              - Hitting with head their own body, or if there are two players,
 *                hitting other players body.
 * If player loses there are two option, to quit game or restart it completely.
 */

//============================== Variables fijas ==============================

//Board colors
/*
 * 1: baby_blue_eyes
 * 2: columbia_blue
 */
static unsigned char baby_blue_eyes[3] = {153, 204, 255};
static unsigned char columbia_blue[3] = {153, 221, 255};

//Apple colors
static unsigned char red[3] = {255, 51, 51};
static unsigned char black[3] = {0, 0, 0};
static unsigned char apple_green[3] = {157, 192, 35};
static unsigned char brown[3] = {139, 69, 19};

//Snake colors
static unsigned char purple[3] = {139, 0, 139};
static unsigned char pink[3] = {255, 0, 128};

//Font color
static unsigned char white[3] = {255, 255, 255};

static int apple[12][11] = {
        {0,0,0,0,1,0,1,1,1,0,0},
        {0,0,0,1,3,1,4,4,1,0,0},
        {0,0,0,0,1,3,4,4,1,0,0},
        {0,0,1,1,1,3,1,1,1,0,0},
        {0,1,2,2,1,1,1,2,2,1,0},
        {1,2,2,2,2,2,2,2,2,2,1},
        {1,2,2,2,2,2,2,2,2,2,1},
        {1,2,2,2,2,2,2,2,2,2,1},
        {1,2,2,2,2,2,2,2,2,2,1},
        {0,1,2,2,2,2,2,2,2,1,0},
        {0,0,1,2,2,2,2,2,1,0,0},
        {0,0,0,1,1,1,1,1,0,0,0},

};

//Controllers drawing
static int player1controllers[CONTROLLER_HEIGHT][CONTROLLER_BASE] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
        {0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0},
        {0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1},
        {0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1},
        {0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1},
        {0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1},
        {0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1},
        {0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1},
        {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
        {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

static int player2controllers[CONTROLLER_HEIGHT][CONTROLLER_BASE] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
        {0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0},
        {0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1},
        {0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1},
        {0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1},
        {0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1},
        {0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1},
        {0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1},
        {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
        {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

//Sound
static int gotAppleSnd[3][2] ={{C4,110},{E4,110},{G4,110}};
static int lose[4][2] = {{C3, 500}, {B2, 500}, {A2, 500}, {G2, 500}};

//============================== Function Declarations ==============================

void setScreen();
void defineCantPlayers();
void defineDifficulty();
void initializePointsToNull();
void initializeSnakes();
void initializeVariables();
void initializeBoard();
void printChessboard();
void printMenu();
void printInstructions();
void printLeft();
void setBackround();
void printBoardGame();
void printApple();
void printSnake();
void printSnakeHead(int direction, Point head);
void rePrintTail(Point tail);
void generateRandomPoint();
int getsApple();
int okWithSnakes(int x, int y);
void move();
void moveSnake(int snake, Point *head, int direction);
void updateHead(Point newPoint, int snake, int length);
void updateTail(Point snake[MAX_POINTS], int snakeMartix[CANT_SQUARES][CANT_SQUARES], int * length);
void changeDirection(int newDirection, int player);
void controllers();
void loseSnd();
void appleSnd();
void startGame();
void playGame();
void game_over();


//============================== Variables to play ==============================

static int board[CANT_SQUARES][CANT_SQUARES] = {{0}};

//Amount of players
int players = 0;

//Level being played
int level = 0;

//Boolean matrix that represents the snakes
int snakeMatrix1[CANT_SQUARES][CANT_SQUARES] = {{0}};
int snakeMatrix2[CANT_SQUARES][CANT_SQUARES] = {{0}};

//Snakes
Point snake1[MAX_POINTS];
Point snake2[MAX_POINTS];


//Snake starting points
Point head1 = {START_X, START_X};
Point head2 = {START_Y, START_Y};

//Directions in which the snakes start moving
/*
 * 0: up
 * 1: right
 * 2: down
 * 3: left
 */
int direction1 = S1_DIREC;
int direction2 = S2_DIREC;

//Players score
int score1 = START_SCORE;
int score2 = START_SCORE;

//Strings to do intToString with
char * stringScore1 = "0";
char * stringScore2 = "0";

//Snakes size
int length1 = START_LIVES;
int length2 = START_LIVES;

//If nobody loses 1
int playing = PLAYING;

//Apple starting position
Point applePos = {START_X, START_Y};

//================================ Command SNAKE function ================================

void play(uint64_t argc,char * args[]) {
    setScreen();
    playGame();
    clearScreen();
}

//====================================== Functions ======================================

// PRE GAME USER INTERACTIONS ------------------------------------------------------------

//Sets the desired font size
void setScreen(){
    int size = 30;
    while (size){
        decSize();
        size--;
    }
    incSize();
}

/* Sets amount of players, only reads 1 character
 * waits for user to insert 1 or 2
 * if not, asks again
 */
void defineCantPlayers() {
    clearScreen();
    char cant = '0';

    while(cant != '1' && cant != '2'){
        printf("\nInsert amount of players (1 or 2): ");
        read(0,&cant, 1);
        printf("\n");
/*        if(cant == EXIT){
            clearScreen();
            playing = 0;
        }*/
    }
    players = charToInt(cant);
    clearScreen();
}

/* Sets speed difficulty, only reads 1 character
 * waits for user to insert 1, 2 or 3
 * if not, asks again
 */
void defineDifficulty() {
    clearScreen();
    char num = '0';

    while(num != '1' && num != '2' && num != '3'){
        printf("\nInsert level (1, 2 or 3): ");
        read(0,&num, 1);
        printf("\n");
    }
    level = charToInt(num);
    switch (level) {
        case 1:
            level = LEVEL1;
            break;
        case 2:
            level = LEVEL2;
            break;
        case 3:
            level = LEVEL3;
            break;
    }
    clearScreen();
}

void indication() {
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf(" \t\t\t\t\tPRESS 'SPACE' TO START");
    while (getChar() != RESTART){
        playing = 1;
    }
};

// INITIATORS ----------------------------------------------------------------------------

//Makes null the Point * snakes
void initializePointsToNull() {
    for (int i = 0; i < MAX_POINTS; i++) {
        snake1[i].x = -1;
        snake1[i].y = -1;
        if(players == 2){
            snake2[i].x = -1;
            snake2[i].y = -1;
        }
    }
}

//Initializes snakes matrix and arrays
void initializeSnakes() {
    // Initialize snake1 in points (11,11), (11,12), (11,13)
    snakeMatrix1[11][11] = 1;
    snakeMatrix1[12][11] = 1;
    snakeMatrix1[13][11] = 1;

    snake1[0].x = 11;
    snake1[0].y = 11;
    snake1[1].x = 11;
    snake1[1].y = 12;
    snake1[2].x = 11;
    snake1[2].y = 13;

    // Initialize snake2 in points (4,4), (3,4), (2,4)
    if(players == 2){
        snakeMatrix2[4][4] = 1;
        snakeMatrix2[4][3] = 1;
        snakeMatrix2[4][2] = 1;

        snake2[0].x = 4;
        snake2[0].y = 4;
        snake2[1].x = 3;
        snake2[1].y = 4;
        snake2[2].x = 2;
        snake2[2].y = 4;
    }
}

//Accommodates variables to start game
void initializeVariables() {
    for(int i = 0 ; i < CANT_SQUARES ; i++){
        for(int j = 0 ; j < CANT_SQUARES ; j++){
            snakeMatrix1[i][j] = 0;
            snakeMatrix2[i][j] = 0;
        }
    }

    initializePointsToNull();

    initializeSnakes();

    head1.x = START_X;
    head1.y = START_X;

    head2.x = START_Y;
    head2.y = START_Y;

    direction1 = S1_DIREC;
    direction2 = S2_DIREC;

    score1 = START_SCORE;
    score2 = START_SCORE;

    length1 = START_LIVES;
    length2 = START_LIVES;

    applePos.x = START_X;
    applePos.y = START_Y;

    playing = PLAYING;
}

//Initializes board matrix with 1s and 2s to define square colors
//This makes reprinting easier
void initializeBoard() {
    int aux = 1;  // Variable to alternate between 1 and 2

    for (int i = 0; i < CANT_SQUARES; i++) {
        for (int j = 0; j < CANT_SQUARES; j++) {
            board[i][j] = aux;
            aux = 3 - aux;  // alternate between 1 and 2 (3 - 1 = 2, 3 - 2 = 1)
        }
        aux = 3 - aux;
    }
}

// PRINTS -------------------------------------------------------------------------------

//Prints board
void printChessboard() {
    for (int i = 0; i < CANT_SQUARES; i++) {
        for (int j = 0; j < CANT_SQUARES; j++) {
            if(board[j][i] == 1){
                printRectangle(baby_blue_eyes, MENU_WIDTH + (i*SQUARE_SIZE), j*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE);
            } else {
                printRectangle(columbia_blue, MENU_WIDTH + (i*SQUARE_SIZE), j*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE);
            }
        }
    }
}

//Prints left menu bar
void printMenu() {
    int aux = 0;
    while(aux < 10){
        decSize();
        aux++;
    }
    incSize();
    incSize();
    putStringXY("\n   SNAKE\n", 10, 0, 0);
    decSize();
    if(players == 1){
        putStringXY("Score: ", 7, 0, SQUARE_SIZE*2);
        intToString(score1, stringScore1);
        putStringXY(stringScore1, digits(score1), 110, SQUARE_SIZE*2);
    } if (players == 2){
        putStringXY("PLAYER 1:\n", 10, 0, SQUARE_SIZE*2);
        putStringXY("Score: ", 7, 0, SQUARE_SIZE*3);
        intToString(score1, stringScore1);
        putStringXY(stringScore1, digits(score1), 110, SQUARE_SIZE*3);

        putStringXY("PLAYER 2:\n", 10, 0, SQUARE_SIZE*4);
        putStringXY("Score: ", 7, 0, SQUARE_SIZE*5);
        intToString(score2, stringScore2);
        putStringXY(stringScore2, digits(score2), 110, SQUARE_SIZE*5);
    }
}

//Print left instructions
void printInstructions() {
    if(players == 1){
        putStringXY("CONTROLLERS: \n", 14, 0, 170);
        for(int x = 0 ; x < CONTROLLER_BASE ; x++){
            for(int y = 0 ; y < CONTROLLER_HEIGHT ; y++){
                if(player1controllers[y][x] == 1){
                    printRectangle(white, x*CONTROLLER_PIXEL_THICKNESS + 30, (y+(2*SQUARE_SIZE))*CONTROLLER_PIXEL_THICKNESS, CONTROLLER_PIXEL_THICKNESS, CONTROLLER_PIXEL_THICKNESS);
                }
            }
        }
    } else {
        putStringXY("CONTROLLERS: \n", 14, 0, 230);
        putStringXY("PLAYER 1: \n", 11, 0, 270);
        for(int x = 0 ; x < CONTROLLER_BASE ; x++){
            for(int y = 0 ; y < CONTROLLER_HEIGHT ; y++){
                if(player1controllers[y][x] == 1){
                    printRectangle(purple, x*CONTROLLER_PIXEL_THICKNESS + 30, 10+(y+(3*SQUARE_SIZE))*CONTROLLER_PIXEL_THICKNESS, CONTROLLER_PIXEL_THICKNESS, CONTROLLER_PIXEL_THICKNESS);
                }
            }
        }

        putStringXY("PLAYER 2: \n", 11, 0, 380);
        for(int x = 0 ; x < CONTROLLER_BASE ; x++){
            for(int y = 0 ; y < CONTROLLER_HEIGHT ; y++){
                if(player2controllers[y][x] == 1){
                    printRectangle(pink, x*CONTROLLER_PIXEL_THICKNESS + 30, 20+(y+(4*SQUARE_SIZE))*CONTROLLER_PIXEL_THICKNESS, CONTROLLER_PIXEL_THICKNESS, CONTROLLER_PIXEL_THICKNESS);
                }
            }
        }
    }
}

// Prints whole left (menu bar + instructions)
void printLeft() {
    clearScreen();
    printMenu();
    printInstructions();
}

// Initiates and prints board
void setBackround() {
    initializeBoard();
    printChessboard();
}

//Prints screen backround
void printBoardGame() {
    printLeft();
    setBackround();
    printApple();
}

//Prints apple
void printApple() {
    int apple_x = MENU_WIDTH+(applePos.x*SQUARE_SIZE)+5;
    int apple_y = applePos.y*SQUARE_SIZE+5;
    for(int i = 0 ; i < APPLE_HEIGHT ; i++){
        for(int j = 0 ; j < APPLE_BASE ; j++){
            if(apple[i][j] == 1){
                printRectangle(black, (j*APPLE_PIXEL_THICKNESS)+apple_x, (i*APPLE_PIXEL_THICKNESS)+apple_y, APPLE_PIXEL_THICKNESS, APPLE_PIXEL_THICKNESS);
            }
            if(apple[i][j] == 2){
                printRectangle(red, (j*APPLE_PIXEL_THICKNESS)+apple_x, (i*APPLE_PIXEL_THICKNESS)+apple_y, APPLE_PIXEL_THICKNESS, APPLE_PIXEL_THICKNESS);
            }
            if(apple[i][j] == 3){
                printRectangle(brown, (j*APPLE_PIXEL_THICKNESS)+apple_x, (i*APPLE_PIXEL_THICKNESS)+apple_y, APPLE_PIXEL_THICKNESS, APPLE_PIXEL_THICKNESS);
            }
            if(apple[i][j] == 4){
                printRectangle(apple_green, (j*APPLE_PIXEL_THICKNESS)+apple_x, (i*APPLE_PIXEL_THICKNESS)+apple_y, APPLE_PIXEL_THICKNESS, APPLE_PIXEL_THICKNESS);
            }
        }
    }
}

//Prints snakes
void printSnake() {
    for(int x = 0 ; x < CANT_SQUARES ; x++){
        for(int y = 0 ; y < CANT_SQUARES ; y++){
            if(snakeMatrix1[y][x] == 1){
                printRectangle(purple,  MENU_WIDTH+(x*SQUARE_SIZE), y*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE);
            }
            if(players == 2 && snakeMatrix2[y][x] == 1){
                printRectangle(pink, MENU_WIDTH+(x*SQUARE_SIZE), y*SQUARE_SIZE,  SQUARE_SIZE, SQUARE_SIZE);
            }
        }
    }
    printSnakeHead(direction1, head1);
    if(players == 2){
        printSnakeHead(direction2, head2);
    }
}

void printSnakeHead(int direction, Point head) {
    switch (direction) {
        case 0:
            printRectangle(white, 5+MENU_WIDTH+(head.x*SQUARE_SIZE), 5+head.y*SQUARE_SIZE, 7, 7);
            printRectangle(white, 20+MENU_WIDTH+(head.x*SQUARE_SIZE), 5+head.y*SQUARE_SIZE, 7, 7);
            printRectangle(black, 7+MENU_WIDTH+(head.x*SQUARE_SIZE), 6+head.y*SQUARE_SIZE, 4, 4);
            printRectangle(black, 22+MENU_WIDTH+(head.x*SQUARE_SIZE), 6+head.y*SQUARE_SIZE, 4, 4);
            return;
        case 1:
            printRectangle(white, 20+MENU_WIDTH+(head.x*SQUARE_SIZE), 5+head.y*SQUARE_SIZE, 7, 7);
            printRectangle(white, 20+MENU_WIDTH+(head.x*SQUARE_SIZE), 20+head.y*SQUARE_SIZE, 7, 7);
            printRectangle(black, 21+MENU_WIDTH+(head.x*SQUARE_SIZE), 7+head.y*SQUARE_SIZE, 4, 4);
            printRectangle(black, 21+MENU_WIDTH+(head.x*SQUARE_SIZE), 22+head.y*SQUARE_SIZE, 4, 4);
            return;
        case 2:
            printRectangle(white, 5+MENU_WIDTH+(head.x*SQUARE_SIZE), 20+head.y*SQUARE_SIZE, 7, 7);
            printRectangle(white, 20+MENU_WIDTH+(head.x*SQUARE_SIZE), 20+head.y*SQUARE_SIZE, 7, 7);
            printRectangle(black, 7+MENU_WIDTH+(head.x*SQUARE_SIZE), 21+head.y*SQUARE_SIZE, 4, 4);
            printRectangle(black, 22+MENU_WIDTH+(head.x*SQUARE_SIZE), 21+head.y*SQUARE_SIZE, 4, 4);
            return;
        case 3:
            printRectangle(white, 5+MENU_WIDTH+(head.x*SQUARE_SIZE), 5+head.y*SQUARE_SIZE, 7, 7);
            printRectangle(white, 5+MENU_WIDTH+(head.x*SQUARE_SIZE), 20+head.y*SQUARE_SIZE, 7, 7);
            printRectangle(black, 6+MENU_WIDTH+(head.x*SQUARE_SIZE), 7+head.y*SQUARE_SIZE, 4, 4);
            printRectangle(black, 6+MENU_WIDTH+(head.x*SQUARE_SIZE), 22+head.y*SQUARE_SIZE, 4, 4);
            return;
    }
}

//When the snake moves forward, it covers the old tail
void rePrintTail(Point tail) {
    if(board[tail.y][tail.x] == 1){
        printRectangle(baby_blue_eyes, MENU_WIDTH + (tail.x)*SQUARE_SIZE, (tail.y)*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE);
    } else {
        printRectangle(columbia_blue, MENU_WIDTH + (tail.x)*SQUARE_SIZE, (tail.y)*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE);
    }
}

// APPLE -------------------------------------------------------------------------------

//Generates random point for apple
void generateRandomPoint() {
    while(!okWithSnakes(applePos.x, applePos.y)) {
        // Generar un valor aleatorio en el rango [0, 23] para x
        applePos.x = customRandInRange(0, 23);
        // Generar un valor aleatorio en el rango [0, 23] para y
        applePos.y = customRandInRange(0, 23);
    }
}

//Checks if snakes got the apple
//Returns the number snake that got it, else 0
int getsApple() {
    int flag = 0;
    if(head1.x == applePos.x && head1.y == applePos.y){
        score1++;
        length1++;
        flag = 1;
    } else if(players == 2 && head2.x == applePos.x && head2.y == applePos.y){
        score2++;
        length2++;
        flag = 2;
    } else {
        return flag;
    }
    level--;    //increases game speed
    appleSnd();
    printMenu();
    generateRandomPoint();
    printApple();
    return flag;
}

// SNAKE MOVEMENT ---------------------------------------------------------------------------

//Checks that the snakes are not in that point
int okWithSnakes(int x, int y) {
    return (snakeMatrix1[y][x] == 0) && (snakeMatrix2[y][x] == 0);
}

//Moves each snake
void move() {
    moveSnake(1, &head1, direction1);
    if(players == 2){
        moveSnake(2, &head2, direction2);
    }
}

//Moves one snake
void moveSnake(int snake, Point *head, int direction) {
    int aux = 0;
    //According to the direction, it checks whether it will hit the wall or another snake
    switch (direction) {
        case 0:
            aux = head->y - 1;
            if (aux < 0 || !okWithSnakes(head->x, aux)) {
                game_over();
            }
            head->y--;
            break;
        case 1:
            aux = head->x + 1;
            if (aux == CANT_SQUARES || !okWithSnakes(aux, head->y)) {
                game_over();
            }
            head->x++;
            break;
        case 2:
            aux = head->y + 1;
            if (aux == CANT_SQUARES || !okWithSnakes(head->x, aux)) {
                game_over();
            }
            head->y++;
            break;
        case 3:
            aux = head->x - 1;
            if (aux < 0 || !okWithSnakes(aux, head->y)) {
                game_over();
            }
            head->x--;
            break;
    }

    //Updates head
    if(playing) {
        if (snake == 1) {
            snakeMatrix1[head->y][head->x] = 1;
            Point newHead;
            newHead.x = head1.x;
            newHead.y = head1.y;
            updateHead(newHead, 1, length1);
        }
        if (snake == 2) {
            snakeMatrix2[head->y][head->x] = 1;
            Point newHead;
            newHead.x = head2.x;
            newHead.y = head2.y;
            updateHead(newHead, 2, length2);

        }

        //Checks if got apple
        //If so, does not update tail because snake grows
        //else updates tail
        int gotApple = getsApple();
        switch (gotApple) {
            case 0:
                if(snake == 1){
                    updateTail(snake1, snakeMatrix1, &length1);
                }
                if (players == 2 && snake == 2) {
                    updateTail(snake2, snakeMatrix2, &length2);
                }
                break;
            case 1:
                if (players == 2 && snake == 2){
                    updateTail(snake2, snakeMatrix2, &length2);
                }
                break;
            case 2:
                if(snake == 1){
                    updateTail(snake1, snakeMatrix1, &length1);
                }
                break;
        }

        printSnake();
    }
}

//Adds new head to Point arrays
void updateHead(Point newPoint, int snake, int length) {
    //Moves array points
    for (int i = length; i > 0 ; i--) {
        if(snake == 1){
            snake1[i] = snake1[i-1];
        } else {
            snake2[i] = snake2[i-1];
        }
    }

    // Updates head
    if(snake == 1){
        snake1[0] = newPoint;
    } else {
        snake2[0] = newPoint;
    }
}

//When snakes moves, modifies tail and covers old one
void updateTail(Point snake[MAX_POINTS], int snakeMartix[CANT_SQUARES][CANT_SQUARES], int * length) {
    int aux = *length;
    Point auxPoint = snake[aux];
    rePrintTail(auxPoint);
    snakeMartix[auxPoint.y][auxPoint.x] = 0;
}

// Changes snake#player direction
void changeDirection(int newDirection, int player) {
    if(player == 1){
        if(direction1+2 == newDirection || direction1-2 == newDirection){
            return;
        }
        direction1 = newDirection;
    } else {
        if(direction2+2 == newDirection || direction2-2 == newDirection){
            return;
        }
        direction2 = newDirection;
    }
}

// Busca las teclas ingresadas por el usuario para mover las serpientes
void controllers() {
    switch (getChar()) {
        //snake1 moves
        case UP1:
            changeDirection(0, 1);
            return;
        case RIGHT1:
            changeDirection(1, 1);
            return;
        case DOWN1:
            changeDirection(2, 1);
            return;
        case LEFT1:
            changeDirection(3, 1);
            return;
        //snake2 moves
        case UP2:
            changeDirection(0, 2);
            return;
        case RIGHT2:
            changeDirection(1, 2);
            return;
        case DOWN2:
            changeDirection(2, 2);
            return;
        case LEFT2:
            changeDirection(3, 2);
            return;
        //leaves game
        case EXIT:
            clearScreen();
            playing = 0;
            return;
        //if lost, restarts
        case RESTART:
            if(playing == 0){
                playGame();
            }
            return;
    }
}

// SOUND -------------------------------------------------------------------------------

void loseSnd() {
    for (int j = 0; j < 4; ++j) {
        _playSound(lose[j][0],lose[j][1]);
    }
}

void appleSnd() {
    for(int j = 0; j< 3; ++j){
        _playSound(gotAppleSnd[j][0], gotAppleSnd[j][1]);
    }
}

// PLAY --------------------------------------------------------------------------------

void startGame() {
    initializeVariables();
    printBoardGame();
}

void playGame() {
    defineCantPlayers();
    defineDifficulty();
    indication();
    startGame();
    while(playing){
        sleep(level);
        move();
        controllers();
    }
}

// LOSE --------------------------------------------------------------------------------

void game_over() {
    //Prints message
    clearScreen();
    loseSnd();
    printf("\n\n\n\n\n\n\n\n\n\n");
    printf("\n\n\n\n\n\n\n\n\n\n");
    printf("\t\t\t\t\t\t\tGAME OVER\n\t\t\t\t\tPRESS 'SPACE' TO PLAY AGAIN\n\t\t\t\t\tPRESS 'X' TO EXIT\n");
    //Waits for user to choose if exit or restart
    char status = '0';
    while(status != EXIT && status != RESTART){
        status = getChar();
    }
    switch (status) {
        case EXIT:
            playing = 0;
            return;
        case RESTART:
            playGame();
            return;
    }
}

