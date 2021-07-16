#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

#define HEIGHT 10
#define WIDTH 10
#define W '#'
#define H ' '
#define X 'X'
#define P '.'
#define STARTROW 1
#define STARTCOL 0
#define ENDROW 8
#define ENDCOL 9
#define MXROW 9
#define MXCOL 9

typedef enum bool {false, true} bool;

void test(void);
void printMaze(unsigned char maze[HEIGHT][WIDTH], short row, short col);
bool findRoute(unsigned char maze[HEIGHT][WIDTH], unsigned char row, unsigned char col);
