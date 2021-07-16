#include "common.h"

int numdiff(const int l[], const int n)
{
   short a, b, count = 0;
   bool duplicate = false;

   if(l == NULL){
      return 0;
   }

   for(a = 0; a < n - 1; a++){
      for(b = a + 1; b < n; b++){
         if(l[a] == l[b]){
            duplicate = true;
         }
      }
      if(!duplicate){
         count++;
      }
      duplicate = false;
   }
   /*add on last element, if it is a duplicate those would have been skipped*/
   count++;
   return count;
}
