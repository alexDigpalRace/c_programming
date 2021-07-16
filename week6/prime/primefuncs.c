#include "prime.h"

bool primeFacts(int targetNum, int primeFactors[MXARR], int nextPrimeNum)
{
   static int i = 0;
   int ans;

   /*last prime found so exit out of program*/
   if(targetNum == nextPrimeNum){
      primeFactors[i] = nextPrimeNum;
      return true;
   }
   /*find factors*/
   if(targetNum % nextPrimeNum == 0){
      primeFactors[i] = nextPrimeNum;
      i++;
      ans = targetNum / nextPrimeNum;
      primeFacts(ans, primeFactors, nextPrimeNum);
   }
   else{
      primeFacts(targetNum, primeFactors, nextPrime(nextPrimeNum));
   }
   return true;
}

int nextPrime(int previousPrime)
{
   int i, j;
   bool skip = false;
   if(previousPrime < 2){
      return 2;
   }
   for(i = previousPrime+1; i < MAX; i++){
      for(j = i-1; j > 1; j--){
         if(i % j == 0){
            skip = true;
         }
      }
      if(!skip){
         return i;
      }
      skip = false;
   }
   return 0;
}
