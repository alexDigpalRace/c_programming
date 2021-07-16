#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<stdbool.h>
#include "neillsimplescreen.h"

#define LENGTH 30
#define WIDTH 80
#define TREE '@'
#define EMPTY ' '
#define FIRE '*'
#define GFACTOR 250
#define LFACTOR GFACTOR*10
#define CHECKWIDTH 3
#define STARTITERATION 1
#define ORIGINALBOARD 0
#define FIRSTNEIGHCOOD 1
#define RANDOMSSEED 1

void test(void);
void printBoard(char board[LENGTH][WIDTH], short iteration);
void growTree(char board[LENGTH][WIDTH], char output[LENGTH][WIDTH], short growthRate);
void copyArray(char toCopy[LENGTH][WIDTH], char output[LENGTH][WIDTH]);
bool noCellType(char board[LENGTH][WIDTH], char cellType);
void lightning(char boar[LENGTH][WIDTH], char output[LENGTH][WIDTH], short strikeRate);
void burn(char board[LENGTH][WIDTH], char output[LENGTH][WIDTH]);
void fillBoard(char board[LENGTH][WIDTH], char cellType);

int main(void)
{
   char startBoard[LENGTH][WIDTH] = {0};
   char nextBoard[LENGTH][WIDTH] = {0};
   short iterations = 1000;
   short a;

   /*clear the console screen*/
/*   neillclrscrn();*/
   /*move console to home*/
/*   neillcursorhome();*/

   /*Board produces the same map each time,
   change RANDOMSSEED value for different map,
   didn't want to use a srand based off of time()
   or clock but would be easy to do...*/
   srand(RANDOMSSEED);

/*   test();*/
   fillBoard(startBoard, EMPTY);
   copyArray(startBoard, nextBoard);
   printBoard(startBoard, ORIGINALBOARD);

   for(a = 0; a < iterations; a++){
      burn(startBoard, nextBoard);
      copyArray(nextBoard, startBoard);
      lightning(startBoard, nextBoard, LFACTOR);
      copyArray(nextBoard, startBoard);
      growTree(startBoard, nextBoard, GFACTOR);
      copyArray(nextBoard, startBoard);
      printBoard(startBoard, STARTITERATION + a);
   }
   /*reset colours back to white*/
   neillfgcol(white);
   return 0;
}

/*tests functions*/
void test(void)
{
   unsigned char a, b;
   short c;
   char testArr[LENGTH][WIDTH] = {0};
   char testArr2[LENGTH][WIDTH] = {0};
   short iterNum = 10 - STARTITERATION;

   fillBoard(testArr, EMPTY);
/*   printBoard(testArr, STARTITERATION);*/
   copyArray(testArr, testArr2);
   /*tests copyArray*/
   for(a = 0; a < LENGTH; a++){
      for(b = 0; b < WIDTH; b++){
         if(testArr[a][b] != testArr2[a][b]){
            printf("\n!!!!!!!error in copy array!!!!!!\n");
         }
      }
   }
   assert(!noCellType(testArr, EMPTY));
   assert(noCellType(testArr, FIRE));
   assert(noCellType(testArr2, TREE));
   testArr[0][0] = TREE;
   testArr[0][1] = TREE;
   testArr[0][2] = TREE;
   testArr[1][0] = TREE;
   testArr[1][1] = FIRE;
   testArr[1][2] = TREE;
   testArr[2][0] = TREE;
   testArr[2][1] = TREE;
   testArr[2][2] = TREE;
   assert(!noCellType(testArr, TREE));
   assert(!noCellType(testArr, EMPTY));
   assert(!noCellType(testArr, FIRE));
   printBoard(testArr, ORIGINALBOARD);
   /*loop through the rules iterNum times*/
   for(c = 0; c < iterNum; c++){
      burn(testArr, testArr2);
      copyArray(testArr2, testArr);
      lightning(testArr, testArr2, LFACTOR);
      copyArray(testArr2, testArr);
      growTree(testArr, testArr2, GFACTOR);
      copyArray(testArr2, testArr);
      printBoard(testArr, c + STARTITERATION);
   }
}

/*prints out a given board, numbered for debugging and iteration label,
left in numbering as a comment as i thought it might be useful to you...*/
void printBoard(char board[LENGTH][WIDTH], short iteration)
{
   unsigned char i, j;
   printf("Iteration %d\n", iteration); /*iteration number*/
   for(i = 0; i < LENGTH; i++){
/*      printf("%3d ", i + 1);*/        /*line numbering*/
      for(j = 0; j < WIDTH; j++){
         if(board[i][j] == TREE){
            neillfgcol(green);
         }
         else if(board[i][j] == FIRE){
            neillfgcol(red);
         }
         printf("%c", board[i][j]);
      }
      printf("\n");
   }
}

/*randomly choose cells to grow somewhere on the board
TODO: Protection against cell be picked to grow multiple times?*/
void growTree(char board[LENGTH][WIDTH], char output[LENGTH][WIDTH], short growthRate)
{
   unsigned char randomRow, randomCol;
   short i = 0;
   /*generate random cells, check they're empty, then fill with */
   while(i < LENGTH * WIDTH){
      randomRow = (rand() % 30);
      randomCol = (rand() % 80);
      if(board[randomRow][randomCol] == EMPTY){
         output[randomRow][randomCol] = TREE;
         i += growthRate;
      }
      /*protection from no EMPTY left in array*/
      else if(noCellType(output, EMPTY)){
         i += growthRate;
      }
   }
}

/*copies an array to another*/
void copyArray(char toCopy[LENGTH][WIDTH], char output[LENGTH][WIDTH])
{
   unsigned char i, j;
   for(i = 0; i < LENGTH; i++){
      for(j = 0; j < WIDTH; j++){
         output[i][j] = toCopy[i][j];
      }
   }
}

/*checks if array has any cells of cellType left*/
bool noCellType(char board[LENGTH][WIDTH], char cellType)
{
   short i, j;
   for(i = 0; i < LENGTH; i++){
      for(j = 0; j < WIDTH; j++){
         if(board[i][j] == cellType){
            return false;
         }
      }
   }
   return true;
}

/*randomly choose cells to be struck by lightning,
function similar to growTree*/
void lightning(char board[LENGTH][WIDTH], char output[LENGTH][WIDTH], short strikeRate)
{
   unsigned char randomRow, randomCol;
   short i = 0;
   while(i < LENGTH * WIDTH){
      randomRow = (rand() % 30);
      randomCol = (rand() % 80);
      if(board[randomRow][randomCol] == TREE){
         output[randomRow][randomCol] = FIRE;
         i += strikeRate;
      }
      else if(noCellType(output, TREE)){
         i += strikeRate;
      }
   }
}

/*turn cells on fire into empty cells and neighbouring trees to fire cells*/
void burn(char board[LENGTH][WIDTH], char output[LENGTH][WIDTH])
{
   unsigned char i, j, k, m;
   short currentRow, currentCol;
   /*turns burning trees into empty cells on the next iteration*/
   for(i = 0; i < LENGTH; i++){
      for(j = 0; j < WIDTH; j++){
         if(board[i][j] == FIRE){
            output[i][j] = EMPTY;
            /*find and make neighbours burn, starting at cell diagonally left up*/
            currentRow = i - FIRSTNEIGHCOOD;
            currentCol = j - FIRSTNEIGHCOOD;
            /*check eight neighbours for tree and turn them to fire*/
            for(k = 0; k < CHECKWIDTH; k++){
               for(m = 0; m < CHECKWIDTH; m++){
                  /*protect against evaluating cells that dont exist*/
                  if(currentRow >= 0 && currentRow < LENGTH && currentCol >= 0 && currentCol < WIDTH){
                     /*dont evaluate self*/
                     if(!(currentRow == i && currentCol == j)){
                        if(board[currentRow][currentCol] == TREE){
                           output[currentRow][currentCol] = FIRE;
                        }
                     }
                  }
                  currentCol++;
               }
               currentCol = j - FIRSTNEIGHCOOD;
               currentRow++;
            }
         }
      }
   }
}

/*Fill array with specified char*/
void fillBoard(char board[LENGTH][WIDTH], char cellType)
{
   unsigned char a, b;
   for(a = 0; a < LENGTH; a++){
      for(b = 0; b < WIDTH; b++){
         board[a][b] = cellType;
      }
   }
}
