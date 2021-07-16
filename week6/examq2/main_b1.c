/*
   For a 2D array, a of width 10 and height h, check that the array is
   valid. Validity here is defined as; all negative numbers in the array
   (if there are any) can only be on the boundary (edges) of the array.
*/

#include "common.h"

bool negativeedge(int a[][10], int h);

int main(void)
{

   int i;
   int a[1][10] = {{-2}};
   int b[5][10] = {{0}};
   int c[10][10] = {{0}};
   int d[2][10] = {{0}};
   a[0][5] = -17.5;
   a[0][1] = 10.3;

   assert(negativeedge(a, 1));
   assert(negativeedge(b, 5));
   assert(negativeedge(c, 5));
   assert(negativeedge(d, 2));
   for(i=0; i<100; i++){
      d[rand()%2][rand()%10] = -5;
      a[0][rand()%10] = -5;
      assert(negativeedge(d, 2));
      assert(negativeedge(a, 1));
   }
   b[2][2] ^= -7;
   assert(!negativeedge(b, 5));
   b[2][2] ^= -7;
   assert(negativeedge(b, 5));
     
   return 0;
}
