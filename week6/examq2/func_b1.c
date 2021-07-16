#include "common.h"

bool negativeedge(int a[][10], int h)
{
   short i, j, d = 0;
   short lastIndex = 0;
   short intCount = 0;

   for(i = 0; i < h; i++){
      /*count elements in array, which is the actual array width*/
      while(d < h){
         if(a[h - 1][d] > 0 || a[h - 1][d] < 0 ){
            lastIndex = d;
            intCount++;
         }
         d++;
      }
      if(lastIndex > 0){
         if(intCount == 1){
            return true;
         }
         for(j = 0; j < lastIndex; j++){
            /*find the edge of array, last number*/
            if(j > 0 && a[i][j] < 0){
               return false;
            }
         }
      }
   }
   return true;
}
