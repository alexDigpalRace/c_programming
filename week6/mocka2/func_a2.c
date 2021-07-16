#include "common.h"

char* strnodupes(const char* s)
{
   char* copy;
   short a, b, c, d;
   short stringLength;
   stringLength = strlen(s);
   copy = (char*) calloc(stringLength*2, sizeof(char));
   /*copy constant string into a non constant one*/
   /*COULD HAVE USED STRCPY*/
   for(d = 0; d < stringLength; d++){
      copy[d] = *s++;
   }
   /*strcpy(copy, s);*/

   /*remove duplicates*/
   for(a = 0; a < stringLength - 1; a++){
      for(b = a + 1; b < stringLength; b++){
         if(copy[a] == copy[b] && copy[a] != '\0'){
            c = b + 1;
            d = b;
            while(copy[d] != '\0'){
               copy[d++] = copy[c++];
            }
            b--;
         }
      }
   }
   stringLength = strlen(copy);
   /*printf("%s %d\n", copy, stringLength);*/
   return copy;
}
