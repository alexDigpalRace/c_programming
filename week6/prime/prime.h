#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define MAX 100000
#define MXARR 100

typedef enum bool {false, true} bool;

int nextPrime(int previousPrime);
bool primeFacts(int targetNum, int primeFactors[MXARR], int nextPrimeNum);
