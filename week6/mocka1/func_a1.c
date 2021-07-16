#include "common.h"
#include "ctype.h"

bool struses(const char* s1, const char* s2)
{
   short i;
   bool found = false;
   short stringLength = strlen(s2);

   while(*s1 != '\0'){
      for(i = 0; i < stringLength; i++){
         if(*s1 == *(s2 + i)){
            found = true;
         }
      }
      if(!found){
         return false;
      }
      s1++;
      found = false;
   }
   return true;
}
