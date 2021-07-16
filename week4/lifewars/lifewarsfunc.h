/*header file has the #includes, #define and typedefs and function
prototypes*/

#include<stdlib.h>
#include<stdio.h>
#include<assert.h>

#define HEIGHT 90
#define WIDTH 150
#define PLAYER1 '+'
#define PLAYER2 '@'
#define EMPTY ' '
#define MAXFILELENGTH 100
#define GENERATIONS 5000
#define MAXFILENAME 25
#define FIRSTLINE 1
#define TEMPSIZE 5
#define LIFECOODCOL 2
#define LIFECOODROW 99

typedef enum bool {false, true} bool;

void test(void);
int scanFile(char fileName[MAXFILENAME], short lifeCood[LIFECOODROW][LIFECOODCOL]);
bool convertCood(unsigned char player, unsigned char board[HEIGHT][WIDTH], short lifeCood[LIFECOODROW][LIFECOODCOL], unsigned char maxCoods);
void printBoard(unsigned char board[HEIGHT][WIDTH]);
void startBoard(unsigned char board[HEIGHT][WIDTH]);
bool boardCheckHelper(unsigned char board1[HEIGHT][WIDTH], unsigned char board2[HEIGHT][WIDTH]);
