/*Reads a line of a file to a string, string then sscanf to remove \n char, each string saved to a word list array*/
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
