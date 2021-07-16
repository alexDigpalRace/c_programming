#include<assert.h>
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<time.h>

#define BOARDLENGTH 20
#define SEARCH 5000000
#define SWAP(A,B) {unsigned char temp; temp=A;A=B;B=temp;}

typedef enum bool {false, true} bool;

int calculateScore(unsigned char board[], const unsigned char n);
void copyArray(unsigned char destBoard[BOARDLENGTH], unsigned char srcBoard[BOARDLENGTH], unsigned char arraySize);
void swap(unsigned char board[BOARDLENGTH], unsigned char arraySize);

struct bestboard{
   unsigned short lowestScore;
   unsigned char lowestCombo[BOARDLENGTH];
};
