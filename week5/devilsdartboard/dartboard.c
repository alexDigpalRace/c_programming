/*Exercise to practice recursion*/
/*Use recursion to calculate the order of the dartboard sequence which calculates
the lowest score, score calculated by summing the square of a sum of 3 neighbours
e.g. (20 + 1 + 18)^2 + (1 + 18 + 4)^2 + (18 + 4 + 13)^2...(5 + 20 + 1)^2 = 20478*/

#include "dartboard.h"

int main(void)
{
   unsigned char dartboard[BOARDLENGTH] = {20, 1, 18, 4, 13, 6, 10, 15, 2, 17, 3, 19, 7, 16, 8, 11, 14, 9, 12, 5};
   unsigned int a;
   unsigned short score;
   struct bestboard bestDartboard;

   /*calculate the intial score and set it to the lowestScore*/
   bestDartboard.lowestScore = calculateScore(dartboard, BOARDLENGTH);
   copyArray(bestDartboard.lowestCombo, dartboard, BOARDLENGTH);

   for(a = 0; a < SEARCH; a++){
      SWAP(dartboard[a % BOARDLENGTH], dartboard[(a + 1) % BOARDLENGTH]);
      score = calculateScore(dartboard, BOARDLENGTH);
      if(score < bestDartboard.lowestScore){
         bestDartboard.lowestScore = score;
      }
      else{
         SWAP(dartboard[a % BOARDLENGTH], dartboard[(a + 1) % BOARDLENGTH]);
      }
      printf("%d a: %d, a+1: %d\n", a, a % BOARDLENGTH, (a+1) % BOARDLENGTH);
   }

   copyArray(bestDartboard.lowestCombo, dartboard, BOARDLENGTH);
   fprintf(stdout, "Total = %d : ", bestDartboard.lowestScore);
   for(a = 0; a < BOARDLENGTH; a++){
      fprintf(stdout, "%d ", bestDartboard.lowestCombo[a]);
   }
   printf("\n");
   return 0;
}
/*recursive function that calculates the score of */
int calculateScore(unsigned char board[BOARDLENGTH], unsigned char arraySize)
{
   unsigned char a, b, c;
   unsigned short ans = 0;
   for(a = 0; a < arraySize; a++){
      b = a + 1;
      c = a + 2;
      if(c >= arraySize){
         c -= arraySize;
      }
      if(b >= arraySize){
         b -= arraySize;
      }
      ans += pow((board[a] + board[b] + board[c]), 2);
   }
   return ans;
}

void copyArray(unsigned char destBoard[BOARDLENGTH], unsigned char srcBoard[BOARDLENGTH], unsigned char arraySize)
{
   unsigned short a;
   for(a = 0; a < arraySize; a++){
      destBoard[a] = srcBoard[a];
   }
}
