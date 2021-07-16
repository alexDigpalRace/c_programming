/*has all the functions*/
#include "lifewarsfunc.h"
/*tests....*/
void test(void)
{
   unsigned char testArr1[HEIGHT][WIDTH] = {0};
   unsigned char testArr2[HEIGHT][WIDTH] = {0};
   startBoard(testArr1);
   startBoard(testArr2);
   assert(boardCheckHelper(testArr1, testArr2));
}

/*reads a life file and outputs life co-ods, returns how many lines successfully scanned*/
int scanFile(char fileName[MAXFILENAME], short lifeCood[LIFECOODROW][LIFECOODCOL])
{
   FILE *fp = NULL;
   unsigned char newLineCount = 0;
   unsigned char i = 0;
   unsigned char row = 0, col = 0;
   int c;
   char tempCood[TEMPSIZE] = {0};

   if(!(fp = fopen(fileName, "r"))){
      fprintf(stdout, "\n\n!cannot read file!\n\n");
   }
   while(newLineCount < MAXFILELENGTH && c != EOF)
   {
      c = fgetc(fp);
       /*check if new line unsigned char to skip title and
       ensure only 99 coods get scanned*/
      if(newLineCount > 0){
         if(c != '\n' && c != ' '){
            tempCood[i++] = c;
         }
         /*space detected, first cood fully scanned, turn into a string,
         save to cood array*/
         if(c == ' '){
            tempCood[i] = '\0';
            lifeCood[row][col] = atoi(tempCood);
            i = 0;
            col++;
         }
         /*newline detected, second cood fully scanned, turn into string,
         save to cood array*/
         else if(c == '\n'){
            tempCood[i] = '\0';
            lifeCood[row][col] = atoi(tempCood);
            i = 0;
            row++;
            col = 0;
         }
      }
      if(c == '\n'){
         newLineCount++;
      }
   }
   fclose(fp);
   return newLineCount;
}
/*take the life coods inputted and convert them to relevant players cells on the board,
randomly selectes where the player origin is,
return number of coods converted*/
bool convertCood(unsigned char player, unsigned char board[HEIGHT][WIDTH], short lifeCood[LIFECOODROW][LIFECOODCOL], unsigned char maxCoods)
{
   unsigned char originRow = (rand() % HEIGHT), originCol = (rand() % WIDTH);
   short wrapRow, wrapCol;
   unsigned char a;

   fprintf(stdout, "Origin point: %d, %d\n", originRow, originCol);

   for(a = 0; a < maxCoods; a++){
      wrapRow = originRow + lifeCood[a][0];
      wrapCol = originCol + lifeCood[a][1];

      if(wrapRow > 89){ /*magic number, bottom cell*/
         wrapRow -= HEIGHT;
      }
      else if(wrapRow < 0){ /*magic, top cell*/
         wrapRow += HEIGHT;
      }
      if(wrapCol > 149){ /*magic, most right cell*/
         wrapCol -= WIDTH;
      }
      else if(wrapCol < 0){
         wrapCol += WIDTH;
      }
      fprintf(stdout, "Board point: %d, %d\n", wrapRow, wrapCol);
      if(board[wrapRow][wrapCol] == EMPTY || board[wrapRow][wrapCol] == player){
         board[wrapRow][wrapCol] = player;
      }
      else{
         /*return false;*/
      }
   }
   /*return true;*/
}
/*prints out the board to the console, for testing purposes*/
void printBoard(unsigned char board[HEIGHT][WIDTH])
{
   unsigned char row, col;
   for(row = 0; row < HEIGHT; row++){
      fprintf(stdout, "%3d ", row + 1);
      for(col = 0; col < WIDTH; col++){
         fprintf(stdout, "%c", board[row][col]);
      }
      fprintf(stdout, "\n");
   }
}
/*fills inputted board with EMPTY cells*/
void startBoard(unsigned char board[HEIGHT][WIDTH])
{
   unsigned char row, col;
   for(row = 0; row < HEIGHT; row++){
      for(col = 0; col < WIDTH; col++){
         board[row][col] = EMPTY;
      }
   }
}
/*for testing... if two boards are the same or not*/
bool boardCheckHelper(unsigned char board1[HEIGHT][WIDTH], unsigned char board2[HEIGHT][WIDTH])
{
   unsigned char row, col;
   for(row = 0; row < HEIGHT; row++){
      for(col = 0; col < WIDTH; col++){
         if(board1[row][col] != board2[row][col]){
            return false;
         }
      }
   }
   return true;
}
