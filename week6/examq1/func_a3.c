#include "common.h"

bool isVowel(char c);

void strflagvowel(const char* s1, char* s2)
{
   while(*s1 != '\0'){
      if(isVowel(*s1)){
         *s2++ = '*';
         *s2++ = *s1;
         *s2++ = '*';
      }
      else{
         *s2++ = *s1;
      }
      s1++;
   }
   *s2 = '\0';
}

bool isVowel(char c)
{
   switch(c){
      case 'a':
         return true;
      case 'A':
         return true;
      case 'e':
         return true;
      case 'E':
         return true;
      case 'i':
         return true;
      case 'I':
         return true;
      case 'o':
         return true;
       case 'O':
         return true;
      case 'u':
         return true;
      case 'U':
         return true;
      default :
         return false;
   }
}
