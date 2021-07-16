#include "prime.h"

int main(int argc, char *argv[])
{
   int primeFactors[MXARR] = {0};
   int j = 0;
   int currentNum = 0, count = 0;
   assert(argc == 2);

   if(primeFacts(atoi(argv[1]), primeFactors, nextPrime(0))){
      /*printing*/
      printf("Factors are: ");
      printf("1 x ");
      while(primeFactors[j] != 0){
            if(currentNum != primeFactors[j]){
               if(count == 1){
                  printf("%d x ", currentNum);
               }
               else if(count > 1){
                  printf("%d^%d x ", currentNum, count);
               }
               currentNum = primeFactors[j];
               count = 0;
            }
            if(currentNum == primeFactors[j]){
               count++;
            }
         j++;
      }
      if(count > 1){
         printf("%d^%d\n", currentNum, count);
      }
      else if(count == 1){
         printf("%d\n", currentNum);
      }
   }
   else{
      printf("Number is a prime\n");
   }

   return 1;
}
