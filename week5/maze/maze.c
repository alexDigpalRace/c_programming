/*recursively work way through a maze*/

#include "maze.h"

int main(void)
{
   unsigned char maze[HEIGHT][WIDTH] = {{W,W,W,W,W,W,W,W,W,W},
                                        {H,H,W,H,H,H,H,H,H,W},
                                        {W,H,W,H,W,H,W,W,H,W},
                                        {W,H,W,H,W,W,W,W,H,W},
                                        {W,H,W,H,H,H,H,W,H,W},
                                        {W,H,W,H,W,W,W,W,H,W},
                                        {W,H,W,H,H,H,H,W,H,W},
                                        {W,H,W,W,W,W,H,W,H,W},
                                        {W,H,H,H,H,H,H,W,H,X},
                                        {W,W,W,W,W,W,W,W,W,W}};

   printMaze(maze, HEIGHT, WIDTH);
   if(findRoute(maze, STARTROW, STARTCOL)){
      printf("\n");
      maze[STARTROW][STARTCOL] = P;
      maze[ENDROW][ENDCOL] = X;
      printMaze(maze, HEIGHT, WIDTH);
   }

   return 0;
}
/*for testing...*/
void test(void)
{
}
/*prints route through maze in fullstops*/
void printMaze(unsigned char maze[HEIGHT][WIDTH], short row, short col)
{
   short a, b;
   for(a = 0; a < row; a++){
      for(b = 0; b < col; b++){
         fprintf(stdout, "%c", maze[a][b]);
      }
      printf("\n");
   }
}
/*finds route through the maze using recursion*/
bool findRoute(unsigned char maze[HEIGHT][WIDTH], unsigned char row, unsigned char col)
{
   unsigned char right, down, up, left;
   /*can't go negative due to unsigned*/
   up = row - 1;
   left = col - 1;
   /*binds max value to 9 to keep on board*/
   if((right = col + 1) > MXCOL){
      right = MXCOL;
   }
   if((down = row + 1) > MXROW){
      down = MXROW;
   }

   /*found exit*/
   if(maze[row][col] == X){
      return true;
   }
   /*stay within wall*/
   if(maze[row][col] != H){
      return false;
   }

   /*block going back on self*/
   maze[row][col] = P;

   /*try up*/
   if(findRoute(maze, up, col)){
      /*maze[row][col] = P;*/
      return true;
   }
   /*try right*/
   if(findRoute(maze, row, right)){
      /*maze[row][col] = P;*/
      return true;
   }
   /*try left*/
   if(findRoute(maze, row, left)){
      /*maze[row][col] = P;*/
      return true;
   }
   /*try down*/
   if(findRoute(maze, down, col)){
      /*maze[row][col] = P;*/
      return true;
   }
   /*if no route exists*/
   return false;
}
