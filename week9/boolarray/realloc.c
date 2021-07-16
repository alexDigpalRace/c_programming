#include "boolarr.h"

/*create a boolarr and return a pointer to it*/
boolarr* boolarr_init(void)
{
   boolarr* ba = (boolarr*)ncalloc(1, sizeof(boolarr));
   ba->arr = (datatype*)ncalloc(FIXEDSIZE, sizeof(datatype));
   ba->capacity = FIXEDSIZE;
   ba->numbits = 0;
   return ba;
}
/* Create boolarr based on string e.g. "1100001" */
boolarr* boolarr_initstr(const char* str)
{
   int i, j, startIndex, mul, ans;
   datatype tmp, mask = MSB;
   bool flag = true;
   int n = strlen(str);
   boolarr* new = boolarr_init();
   /*make sure new array is big enough to store string*/
   while(new->capacity*BITSPERCHAR < n){
      new->arr = nrecalloc(new->arr, new->capacity, new->capacity*SCALEFACTOR);
      new->capacity *= SCALEFACTOR;
   }
   /**/
   j = 0;
   mul = 1;
   ans = (int)((double)n / (double)BITSPERCHAR + 0.9);
   while(flag){
      startIndex = n - BITSPERCHAR*mul;
      for(i = startIndex; i < startIndex + BITSPERCHAR; i++){
         if(i < 0){
            tmp = 0;
         }
         else{
            /*str cell in ascii codes so need to -'0' to get to decimal*/
            tmp = str[i] - '0';
         }
         if(tmp){
            new->arr[j] |= mask;
         }
         mask >>= 1;
      }
      j++;
      mul++;
      mask = MSB;
      if(mul > ans){
         flag = false;
      }
   }
   return new;
}
/* Return a deep copy */
boolarr* boolarr_clone(const boolarr* ba)
{
   int i, j;
   datatype mask = MSB;
   boolarr* clone = boolarr_init();
   while(clone->capacity != ba->capacity){
      clone->arr = nrecalloc(clone->arr, clone->capacity, clone->capacity*SCALEFACTOR);
      clone->capacity *= SCALEFACTOR;
   }
   j = 0;
   for(i = clone->capacity * BITSPERCHAR; i > 0; i--){
      if(i != clone->capacity * BITSPERCHAR && i % BITSPERCHAR == 0){
         j++;
         mask = MSB;
      }
      clone->arr[j] |= ba->arr[j] & mask;
      mask >>= 1;
   }
   return clone;
}
/* Get number of bits in array */
unsigned int boolarr_size(const boolarr* ba)
{
   if(ba == NULL){
      return 0;
   }
   return ba->numbits;
}
/* Return number of bits that are set true */
unsigned int boolarr_count1s(const boolarr* ba)
{
   int i, j = 0, count = 0;
   datatype ans, mask = MSB;
   if(ba == NULL){
      return 0;
   }
   for(i = 0; i < ba->capacity*BITSPERCHAR; i++){
      if(i != 0 && i % BITSPERCHAR == 0){
         mask = MSB;
         j++;
      }
      ans = ba->arr[j] & mask;
      if(ans != 0){
         count++;
      }
      mask >>= 1;
   }
   return count;
}
/* Set nth bit on/off */
/*[7 6 5 4 3 2 1 0, 15 14 13 12 11 10 9 8, 23 22 21 20 19 18 17 16]*/
bool boolarr_set(boolarr* ba, const unsigned int n, const bool b)
{
   int i;
   datatype bit = LSB;
   if(ba == NULL){
      return false;
   }
   /*check if current size of array is big enough, if not nrecalloc more space*/
   while (n > (unsigned)((BITSPERCHAR * ba->capacity) - 1)){
      ba->arr = nrecalloc(ba->arr, ba->capacity, ba->capacity*SCALEFACTOR);
      ba->capacity *= SCALEFACTOR;
   }
   i = n / BITSPERCHAR;
   bit <<= (n % BITSPERCHAR);
   if(b){
      ba->arr[i] |= bit;
   }
   else{
      ba->arr[i] &= ~bit;
   }
   return true;
}
/* Get nth bit */
bool boolarr_get(const boolarr* ba, const unsigned int n, bool* b)
{
   datatype mask = LSB;
   datatype ans;
   int i;
   if(ba == NULL){
      return false;
   }
   i = n / BITSPERCHAR;
   mask <<= n % BITSPERCHAR;
   ans = mask & ba->arr[i];
   if(ans == 0){
      *b = false;
   }
   else{
      *b = true;
   }
   return true;
}
/* Return if two arrays are the same (bitwise) */
bool boolarr_issame(const boolarr* b1, const boolarr* b2)
{
   char bOne[WORD] = {0};
   char bTwo[WORD] = {0};
   if(b1 == NULL && b2 == NULL){
      return true;
   }
   else if(b1 == NULL || b2 == NULL){
      return false;
   }
   boolarr_tostring(b1, bOne);
   boolarr_tostring(b2, bTwo);
   if(strcmp(bOne, bTwo) == 0){
      return true;
   }
   return false;
}
/* Store to string - rightmost bit is LSB */
bool boolarr_tostring(const boolarr* ba, char* str)
{
   int i, j;
   int mask = MSB;
   int ans;
   bool unblock = false;

   if(ba == NULL || str == NULL){
      return false;
   }
   /*reset string*/
   for(i = 0; i < WORD; i++){
      str[i] = '\0';
   }
   j = ba->capacity - 1;
   for(i = ba->capacity*BITSPERCHAR; i > 0; i--){
      if(i % BITSPERCHAR == 0 && i != ba->capacity*BITSPERCHAR){
         j--;
         mask = MSB;
      }
      ans = ba->arr[j] & mask;
      if(ans == 0){
         if(unblock){
            strcat(str, "0");
         }
      }
      else{
         strcat(str, "1");
         unblock = true;
      }
      mask >>= 1;
   }
   return true;
}
/* Print out array & meta info */
bool boolarr_print(const boolarr* ba)
{
   char temp[WORD] = {0};
   if(boolarr_tostring(ba, temp))
   {
      printf("%s\n", temp);
      return true;
   }
   return false;
}
/* Flip all bits */
bool boolarr_negate(boolarr* ba)
{
   int i;
   if(ba == NULL){
      return false;
   }
   for(i = 0; i < ba->capacity; i++){
      ba->arr[i] = ~ba->arr[i];
   }
   return true;
}
/* Functions dealing with 2 bitwise-arrays */
/* Must be the same length */
boolarr* boolarr_bitwise(const boolarr* ba1, const boolarr* ba2, const logicalop l)
{
   int n, i;
   boolarr* new = boolarr_init();
   if(ba1->capacity > ba2->capacity){
      n = ba1->capacity;
   }
   else{
      n = ba2->capacity;
   }
   while(n > new->capacity){
      new->arr = nrecalloc(new->arr, new->capacity, new->capacity*SCALEFACTOR);
      new->capacity *= SCALEFACTOR;
   }
   switch(l){
      case or:
         for(i = 0; i < new->capacity; i++){
            new->arr[i] = ba1->arr[i] | ba2->arr[i];
         }
         return new;
      case and:
         for(i = 0; i < new->capacity; i++){
            new->arr[i] = ba1->arr[i] & ba2->arr[i];
         }
         return new;
      case xor:
         for(i = 0; i < new->capacity; i++){
            new->arr[i] = ba1->arr[i] ^ ba2->arr[i];
         }
         return new;
   }
   return NULL;
}
/* Clears all space */
bool boolarr_free(boolarr* p)
{
   free(p->arr);
   free(p);
   return true;
}
