#include "draw.h"

int main(void)
{
   unsigned char screen[ROW][COL] = {0};
   fillArray(screen);
   /*start at the top left length 9*/
   draw(screen, STARTROW, STARTCOL, ROW*COL);
   return 0;
}

bool isValid(unsigned char screen[ROW][COL], char row, char col)
{
   /*stay on screen, up, left, down, right*/
   if(row < 0 || col < 0 || row >= ROW || col >= COL){
     return false;
   }
   /*if square already traversed*/
   if(screen[(unsigned)row][(unsigned)col] != EMPTY){
     return false;
   }
   return true;
}
/*recursive draw to unlock function*/
void draw(unsigned char screen[ROW][COL], char row, char col, unsigned char len)
{
   static int pathCount = 0;
   char up, down, right, left;
   up = row - 1;
   down = row + 1;
   right = col + 1;
   left = col - 1;

   if(len == 1){
      screen[(unsigned)row][(unsigned)col] = END;
      printScreen(screen);
      printf("%d\n\n", ++pathCount);
      screen[(unsigned)row][(unsigned)col] = EMPTY;
   }
   /*up*/
   if(isValid(screen, up, col)){
      screen[(unsigned)row][(unsigned)col] = UP;
      draw(screen, up, col, len-1);
      screen[(unsigned)row][(unsigned)col] = EMPTY;
   }
   /*down*/
   if(isValid(screen, down, col)){
      screen[(unsigned)row][(unsigned)col] = DOWN;
      draw(screen, down, col, len-1);
      screen[(unsigned)row][(unsigned)col] = EMPTY;
   }
   /*left*/
   if(isValid(screen, row, left)){
      screen[(unsigned)row][(unsigned)col] = LEFT;
      draw(screen, row, left, len-1);
      screen[(unsigned)row][(unsigned)col] = EMPTY;
   }
   /*right*/
   if(isValid(screen, row, right)){
      screen[(unsigned)row][(unsigned)col] = RIGHT;
      draw(screen, row, right, len-1);
      screen[(unsigned)row][(unsigned)col] = EMPTY;
   }
}

void fillArray(unsigned char array[ROW][COL])
{
   unsigned char a, b;
   for(a = 0; a < ROW; a++){
      for(b = 0; b < COL; b++){
         array[a][b] = EMPTY;
      }
   }
}

void printScreen(unsigned char array[ROW][COL])
{
   unsigned char a, b;
   for(a = 0; a < ROW; a++){
      for(b = 0; b < COL; b++){
         printf("%c", array[a][b]);
      }
      printf("\n");
   }
}
