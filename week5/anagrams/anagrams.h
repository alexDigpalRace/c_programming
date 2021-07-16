#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>

#define LINELENGTH 100
#define MAXWORDS 100

typedef enum bool {false, true} bool;

void test(void);
int readFile(char fileName[LINELENGTH], char wordList[MAXWORDS][LINELENGTH]);
int printList(char wordList[MAXWORDS][LINELENGTH]);
int findMatches(char anagram[LINELENGTH], char wordList[MAXWORDS][LINELENGTH]);
void sortString(char string[LINELENGTH], short stringLength);
