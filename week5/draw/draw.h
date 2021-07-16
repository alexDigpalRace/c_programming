#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define ROW 3
#define COL 3
#define EMPTY ' '
#define END '*'
#define UP '^'
#define DOWN 'v'
#define LEFT '<'
#define RIGHT '>'
#define STARTROW 0
#define STARTCOL 0

typedef enum bool {false, true} bool;

void printScreen(unsigned char array[ROW][COL]);
void fillArray(unsigned char array[ROW][COL]);
bool isValid(unsigned char screen[ROW][COL], char row, char col);
void draw(unsigned char screen[ROW][COL], char row, char col, unsigned char len);
