/*
   Copy s1 into s2, except that every vowel in string s1
   has an asterisk before and after it in s2,
   so "bat" becomes "b*a*t" and "Care" becomes "C*a*r*e*"
*/

#include "common.h"

void strflagvowel(const char* s1, char* s2);

int main(void)
{

   char str[1000];

   strflagvowel("bat", str); assert(strcmp(str,"b*a*t")==0);
   strflagvowel("Care", str); assert(strcmp(str,"C*a*r*e*")==0);
   strflagvowel("Hello World!", str); assert(strcmp(str,"H*e*ll*o* W*o*rld!")==0);
   strflagvowel("IoU", str); assert(strcmp(str,"*I**o**U*")==0);
   strflagvowel("", str); assert(strcmp(str,"")==0);

   return 0;
}
