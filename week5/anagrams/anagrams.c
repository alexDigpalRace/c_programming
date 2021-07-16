/*Allow user to enter an anagram as a executable argument,
scan a file for a list of valid words
print valid words onto console*/
#include "anagrams.h"

int main(int argc, char *argv[])
{
   int wordCount = 0;
   char potentialWords[MAXWORDS][LINELENGTH] = {0};

   test();

   /*read word file, save number of words read*/
   wordCount = readFile("words.txt", potentialWords);
   fprintf(stdout, "Words successfully read from file: %d\n\n\n", wordCount);

   /*check right amount of  arguments entered by user*/
   assert(argc == 2);
   /*compare user anagram with word list and find matches*/
   fprintf(stdout, "Number of words for this anagram is: %d\n", findMatches(argv[1], potentialWords));

   return 0;
}
/*for testing...*/
void test()
{
   /*char dummyArr[MAXWORDS][LINELENGTH] = {0};*/
   char word1[LINELENGTH] = "zxybca";
   char sortedWord1[LINELENGTH] = "abcxyz";
   char word2[LINELENGTH] = "nhjullkjhsadhfasd";
   char sortedWord2[LINELENGTH] = "aaddfhhhjjkllnssu";

   /*assert(readFile("words.txt", dummyArr) == 15);*/
   /*assert(printList(dummyArr) == 15);*/
   sortString(word1, strlen(word1));
   assert(strcmp(word1, sortedWord1) == 0);
   sortString(word2, strlen(word2));
   assert(strcmp(word2, sortedWord2) == 0);
}
/*Reads a line of a file to a string, string processed then saved to wordList*/
int readFile(char fileName[LINELENGTH], char wordList[MAXWORDS][LINELENGTH])
{
   FILE *fp;
   char word[LINELENGTH] = {0};
   short lineCount = 0;
   bool exitFlag = false;
   if((fp = fopen(fileName, "r")) == NULL){
      fprintf(stderr, "Cannot read %s\n", fileName);
      exit(EXIT_FAILURE);
   }
   do{
      if(fgets(word, LINELENGTH, fp) != NULL){
         sscanf(word, "%s", wordList[lineCount]);
         lineCount++;
      }
      else{
         exitFlag = true;
      }

   } while(exitFlag == false);
   fclose(fp);
   return (int)lineCount;
}
/*for testing - prints words to console, returns number of words printed*/
int printList(char wordList[MAXWORDS][LINELENGTH])
{
   short a;
   for(a = 0; a < MAXWORDS; a++){
      /*if all words read, return the count of read words*/
      if(wordList[a][0] == 0){
         fprintf(stdout, "%d\n", a);
         return a;
      }
      fprintf(stdout, "%s\n", wordList[a]);
   }
   return (int)a;
}
/*finds words in the word list that are matches of the user anagrams, returns the
amount of matches found*/
int findMatches(char anagram[LINELENGTH], char wordList[MAXWORDS][LINELENGTH])
{
   char sortedAnagram[LINELENGTH] = {0};
   char sortedWordList[MAXWORDS][LINELENGTH] = {0};
   short a, weight;
   char matchCount = 0;
   /*copy anagram and word list for sorting*/
   strcpy(sortedAnagram, anagram);
   for(a = 0; a < MAXWORDS; a++){
      strcpy(sortedWordList[a], wordList[a]);
   }
   /*sort copies alphabetically*/
   sortString(sortedAnagram, strlen(sortedAnagram));
   for(a = 0; a < MAXWORDS; a++){
      sortString(sortedWordList[a], strlen(sortedWordList[a]));
   }
   /*string compare sorted anagram to words in word list, if 0 printf original
   unsorted word*/
   for(a = 0; a < MAXWORDS; a++){
      weight = strcmp(sortedAnagram, sortedWordList[a]);
      if(weight == 0){
         fprintf(stdout, "%s\n", wordList[a]);
         matchCount++;
      }
   }
   return (int)matchCount;
}
/*sorts a string into alphabetical order*/
void sortString(char string[LINELENGTH], short stringLength)
{
   short a, b;
   char tmp;
   for(a = 0; a < stringLength - 1; a++){
      for(b = a + 1; b < stringLength; b++){
         if(string[b] < string[a]){
            tmp = string[a];
            string[a] = string[b];
            string[b] = tmp;
         }
      }
   }
}
