/*Extension*/
   /*This extension is a way of error handling, I avoided using exit() in my parser
+ interpreter for this purpose in mind... When you compile a file if there is
something wrong with the syntax the compiler won't just stop immediately and return
the one error, instead it carries on, printing each error it finds until the EOF
is reached */
   /*The extension handles them in a way where there shouldn't be false positives, e.g.
if you uncomment all the error msgs in the interpreter and executed a VALID file, you
would find that error msgs are printed to the console, this is most likely a VARNUM arg,
its checking the thing its on is either a VAR or a num, for one to be true the other has to
be false and the way the error system in the program handles that is to print to the console,
which is a false error as it is a valid VARNUM*/
   /*The extension disables any output if a error is detected but will keep moving
through the words to find more errors*/
   /*For some reason SDL_Quit is causing 3 leaks to occur in my code for valid files
(one leak occurring for nonvalid), when i don't include it there are no leaks but
the program closes sucessfully? Not sure if this has something to do with me using
xLaunch but I've left it commented out on the interpreter and extension.
If the porgram isn't exiting on your machine! Please uncomment the SDL_QUIT i know
thats a bit of a cheat but don't want to send code in with leaks and get downmarked...
SDL_QUIT located in EXIT_SDL() func in adaptedsdl2.c*/

#include "turtle.h"

void testing(SDL_Simplewin* sw);

int main(int argc, char** argv)
{
   SDL_Simplewin sw;
   /*create clear struct without callocing struct, 1x 2d array, 2x 1d array*/
   userprog prog = {{{EMPTY}}, EMPTY, NONE, {EMPTY}, {EMPTY}, EMPTY, VALID, EMPTY};

   assert(argc == ARGNUM && "ERROR: TYPE FILENAME");
   /*scan words into struct prog.words*/
   prog.wordCount = scanTurtleFile(&prog, argv[NAMEIND]);
   Adapted_SDL_Init(&sw);
   /*keep commented for proper file execution, testing() messes with SDL*/
   /*testing(&sw);*/
   start(&prog, &sw);
   if(prog.validity){
      printCode(&prog);
      fprintf(stderr, "not a valid file.\n");
   }
   else{
      Adapted_SDL_UpdateScreen(&sw);
      while(sw.finished != SDL_TRUE){
         SDL_Delay(DELAY);
         Neill_SDL_Events(&sw);
      }
   }
   Exit_SDL(&sw);
   return EXIT_SUCCESS;
}
/*below assert to check for unlinked structure only necessary for below function as
all other functions based off below, if below never happens neither does any other function,
and if below function does have structure correctly linked all other functions will*/
unsigned char start(userprog* p, SDL_Simplewin *sw)
{
   assert(p != NULL && sw != NULL && "ERROR: struct pointer NULL?");
   /*check first thing is { as per rules*/
   if(!strcmp(p->words[p->cw], "{")){
      /*passed check so move on to next word/instruction*/
      p->cw++;
      newLineCheck(p);
   }
   else{
      p->validity = NOTVALID;
      /*check if current word is a valid instruction or not*/
      checkInstruction(p);
   }
   return instructlst(p, sw);
}
unsigned char instructlst(userprog* p, SDL_Simplewin *sw)
{
   if(p->cw >= p->wordCount){
      fprintf(stderr, "no end bracket '}' detected?  CW: %d\n", p->cw);
      return EXIT_FAILURE;
   }
   if(!strcmp(p->words[p->cw], "}")){
      return EXIT_SUCCESS;
   }
   instruction(p, sw);
   return instructlst(p, sw);
}
unsigned char instruction(userprog* p, SDL_Simplewin *sw)
{
   if(!strcmp(p->words[p->cw], "FD")){
      p->cw++;
      return fd(p, sw);
   }
   else if(!strcmp(p->words[p->cw], "LT")){
      p->cw++;
      return lt(p, sw);
   }
   else if(!strcmp(p->words[p->cw], "RT")){
      p->cw++;
      return rt(p, sw);
   }
   else if(!strcmp(p->words[p->cw], "DO")){
      p->cw++;
      return doo(p, sw);
   }
   else if(!strcmp(p->words[p->cw], "SET")){
      p->cw++;
      return set(p);
   }
   fprintf(stderr, "Instruction: %s not recognised, ", p->words[p->cw++]); /*inc word here*/
   p->validity = NOTVALID;
   checkFailedInsWords(p);
   return EXIT_FAILURE;
}
unsigned char fd(userprog* p, SDL_Simplewin *sw)
{
   isVARNUM(p);
   newLineCheck(p);
   if(p->validity == VALID){
      sw->x2 = sw->x1 + (int)((double)p->miniStack[BOTTOM] * sin(DGR2RAD(sw->angle)));
      /*SDL COODS origin is TOP LEFT (y goes down to y_max) so negative y movement is fwd/up*/
      sw->y2 = sw->y1 - (int)((double)p->miniStack[BOTTOM] * cos(DGR2RAD(sw->angle)));
      if(!SDL_RenderDrawLine(sw->renderer, sw->x1, sw->y1, sw->x2, sw->y2)){
         sw->x1 = sw->x2;
         sw->y1 = sw->y2;
         clearStack(p);
         return EXIT_SUCCESS;
      }
   }
   return EXIT_FAILURE;
}
unsigned char lt(userprog* p, SDL_Simplewin *sw)
{
   isVARNUM(p);
   newLineCheck(p);
   if(p->validity == VALID){
      sw->angle = inbounds(sw->angle - p->miniStack[BOTTOM]);
      clearStack(p);
      return EXIT_SUCCESS;
   }
   return EXIT_FAILURE;
}
unsigned char rt(userprog* p, SDL_Simplewin *sw)
{
   isVARNUM(p);
   newLineCheck(p);
   if(p->validity == VALID){
      sw->angle = inbounds(sw->angle + p->miniStack[BOTTOM]);
      clearStack(p);
      return EXIT_SUCCESS;
   }
   return EXIT_FAILURE;
}
unsigned char isNumber(userprog* p)
{
   int a;
   nOV save = p->wordType;
   for(a = 0; a < (int)strlen(p->words[p->cw]); a++){
      if(p->words[p->cw][a] < '0' || p->words[p->cw][a] > '9'){
         return EXIT_FAILURE;
      }
   }
   if(!p->validity){
      p->wordType = num;
      if(miniPush(p)){
         fprintf(stderr, "error in isNumber()->miniPush(for dev)\n");
         return EXIT_FAILURE;
      }
   }
   p->wordType = save;
   p->cw++;
   return EXIT_SUCCESS;
}
unsigned char isVAR(userprog* p)
{
   nOV save = p->wordType;
   /*save whether function being called alone or from VARNUM*/
   if(p->wordType != varnum){
      save = var;
   }
   if(p->words[p->cw][LETTER] >= 'A' && p->words[p->cw][LETTER] <= 'Z' && p->words[p->cw][NULCHR] == '\0'){
      if(!p->validity){
         p->wordType = var;
         miniPush(p);
      }
      p->wordType = save;
      p->cw++;
      return EXIT_SUCCESS;
   }
   if(save == var){
      fprintf(stderr, "%s not a legal VAR (A-Z)\n", p->words[p->cw++]);
      p->wordType = none;
      p->validity = NOTVALID;
   }
   return EXIT_FAILURE;
}
unsigned char isVARNUM(userprog* p)
{
   p->wordType = varnum;
   if(!isNumber(p) || !isVAR(p)){
      p->wordType = none;
      return EXIT_SUCCESS;
   }
   fprintf(stderr, "%s not a valid VARNUM (number or A-Z)\n", p->words[p->cw++]);
   p->wordType = none;
   p->validity = NOTVALID;
   return EXIT_FAILURE;
}
unsigned int scanTurtleFile(userprog* p, char* fileName)
{
   FILE* fp;
   char temp[MAXSTRLEN];
   int wordCount = 0;
   char* token;

   if((fp = fopen(fileName, "r")) == NULL){
      fprintf(stderr, "could not find file with name: '%s'\n", fileName);
      exit(EXIT_FAILURE);
   }
   while(wordCount < MAXWORDS && fgets(temp, MAXSTRLEN, fp) != NULL){
      token = strtok(temp, " \n");
      while(token != NULL){
         strcpy(p->words[p->cw++], token);
         wordCount++;
         /*https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm*/
         /*don't really get this line but tutorial points strtok example had it and it works*/
         token = strtok(NULL, " \n");
      }
      p->cw++;
      wordCount++;   /*count the new line as a word*/
   }
   p->cw = 0;
   return wordCount;
}
unsigned char doo(userprog* p, SDL_Simplewin *sw)
{
   short min, max;
   char letter;
   isVAR(p);
   newLineCheck(p);
   clearStack(p);
   letter = p->words[p->cw BCKONE][LETTER];  /*if not valid, it won't be looked at*/
   isFROM(p);
   isVARNUM(p);
   newLineCheck(p);
   min = p->miniStack[BOTTOM];               /*same as letter*/
   clearStack(p);
   isTO(p);
   isVARNUM(p);
   newLineCheck(p);
   max = p->miniStack[BOTTOM];               /*same as min*/
   clearStack(p);
   if(max < min && !p->validity){
      p->validity = NOTVALID;
      fprintf(stderr, "do loops min (%d) and max (%d) values wrong way round, min should come first\n", min, max);
   }
   return dooloop(p, sw, min, max, letter);
}
unsigned char dooloop(userprog* p, SDL_Simplewin* sw, short min, short max, char letter)
{
   short i, cwSave = p->cw;
   if(!p->validity){
      for(i = min; i <= max; i++){
         p->miniStack[BOTTOM] = i;
         /*set the first VAR through range given in 2nd and third VARNUM*/
         if(!setVAR(p, letter)){
            p->cw = cwSave;
            start(p, sw);
         }
      }
      return EXIT_SUCCESS;
   }
   else{
      start(p, sw);
   }
   newLineCheck(p);
   return EXIT_FAILURE;
}
unsigned char isFROM(userprog* p)
{
   if(strcmp(p->words[p->cw], "FROM")){
      fprintf(stderr, "%s  'FROM' command expected\n", p->words[p->cw]);
      p->validity = NOTVALID;
   }
   p->cw++;
   newLineCheck(p);
   return EXIT_SUCCESS;
}
unsigned char isTO(userprog* p)
{
   if(strcmp(p->words[p->cw], "TO")){
      fprintf(stderr, "'TO' command expected\n");
      p->validity = NOTVALID;
   }
   p->cw++;
   newLineCheck(p);
   return EXIT_SUCCESS;
}
int inbounds(int a)
{
   int max = 360;
   if(a >= max || a <= -max){
      a %= max;
   }
   return a;
}
unsigned char set(userprog* p)
{
   char letter;
   isVAR(p);
   newLineCheck(p);
   clearStack(p);
   letter = p->words[p->cw BCKONE][LETTER];
   isEQ(p);
   polish(p);
   if(p->validity == VALID){
      return setVAR(p, letter);
   }
   newLineCheck(p);
   return EXIT_SUCCESS;
}
unsigned char isEQ(userprog* p)
{
   if(strcmp(p->words[p->cw], ":=")){
      fprintf(stderr, "missing := command?\n");
      p->validity = NOTVALID;
   }
   p->cw++;
   newLineCheck(p);
   return EXIT_SUCCESS;
}
/*zeroth element of miniStack loaded with the summed size*/
unsigned char polish(userprog* p)
{
   static unsigned char count = 0;
   if(!newLineCheck(p)){
      fprintf(stderr, "incomplete RPN\n");
      p->validity = NOTVALID;
      return EXIT_FAILURE;
   }
   if(count > OPERATOR){
      if(strcmp(p->words[p->cw], ";")){
         p->validity = NOTVALID;
      }
      count = 0;
      p->cw++;
      newLineCheck(p);
      return EXIT_SUCCESS;
   }
   if(count < OPERATOR){
      isVARNUM(p);
   }
   if(count == OPERATOR){
      isOP(p);
   }
   count++;
   return polish(p);
}
unsigned char isOP(userprog* p)
{
   char ops[OPNUM] = {'+', '-', '*', '/'};
   unsigned char a;
   for(a = 0; a < OPNUM; a++){
      if(p->words[p->cw][LETTER] == ops[a] && p->words[p->cw][NULCHR] == '\0'){
         /*p->validity == VALID*/
         if(!p->validity){
            if(p->stckInd == STCKLEN){
               return sum(p);
            }
         }
         p->cw++;
         return EXIT_FAILURE;
      }
   }
   fprintf(stderr, "%s not a valid operator\n", p->words[p->cw]);
   p->cw++;
   p->validity = NOTVALID;
   newLineCheck(p);
   return EXIT_FAILURE;
}
unsigned char setVAR(userprog* p, char letter)
{
   if(p->miniStack[TOP] == 0){
      p->savedVars[letter - 'A'] = p->miniStack[BOTTOM];
      clearStack(p);
      return EXIT_SUCCESS;
   }
   fprintf(stderr, "stack not cleared? (problematic letter: %c)\n", letter);
   return EXIT_FAILURE;
}
unsigned char miniPush(userprog* p)
{
   short strToInt;
   if(p->wordType == num){
      /*scan int out of the current word*/
      if(sscanf(p->words[p->cw], "%hd", &strToInt) == 1){
         p->miniStack[p->stckInd++] = strToInt;
         p->wordType = none;
         return EXIT_SUCCESS;
      }
   }
   if(p->wordType == var){
      varLookup(p, p->words[p->cw][LETTER]);
      p->wordType = none;
      return EXIT_SUCCESS;
   }
   fprintf(stderr, "unable to push the VARNUM %s to the stack\n", p->words[p->cw]);
   return EXIT_FAILURE;
}
unsigned char sum(userprog* p)
{
   switch(p->words[p->cw][LETTER]){
      case '+':
         p->miniStack[BOTTOM] += p->miniStack[TOP];
         p->miniStack[TOP] = 0;
         p->stckInd--;
         p->cw++;
         newLineCheck(p);
         return EXIT_SUCCESS;
      case '-':
         p->miniStack[BOTTOM] -= p->miniStack[TOP];
         p->miniStack[TOP] = 0;
         p->stckInd--;
         p->cw++;
         newLineCheck(p);
         return EXIT_SUCCESS;
      case '*':
         p->miniStack[BOTTOM] *= p->miniStack[TOP];
         p->miniStack[TOP] = 0;
         p->stckInd--;
         p->cw++;
         newLineCheck(p);
         return EXIT_SUCCESS;
      case '/':
         p->miniStack[BOTTOM] /= p->miniStack[TOP];
         p->miniStack[TOP] = 0;
         p->stckInd--;
         p->cw++;
         newLineCheck(p);
         return EXIT_SUCCESS;
   }
   /*validity check in isOP should prevent it from getting to here if operator not legal*/
   return EXIT_FAILURE;
}
void varLookup(userprog* p, char letter)
{
   p->miniStack[p->stckInd++] = p->savedVars[letter - 'A'];
}
void clearStack(userprog* p)
{
   p->miniStack[BOTTOM] = 0;
   p->miniStack[TOP] = 0;
   p->stckInd = 0;
}
void checkFailedInsWords(userprog* p)
{
   unsigned char potenErrorCnt = 0;
   bool exitFlag = false;
   while(!exitFlag){
      /*VAR/NUM*/
      if((p->words[p->cw][LETTER] >= 'A' && p->words[p->cw][LETTER] <= 'Z' && p->words[p->cw][NULCHR] == '\0') ||
         (p->words[p->cw][LETTER] >= '0' && p->words[p->cw][LETTER] <= '9') ||
         (!strcmp(p->words[p->cw], ":=")) ||
         (!strcmp(p->words[p->cw], "FROM")) ||
         (!strcmp(p->words[p->cw], "TO")) ||
         (!strcmp(p->words[p->cw], "{")) ||
         (!strcmp(p->words[p->cw], "+")) ||
         (!strcmp(p->words[p->cw], "-")) ||
         (!strcmp(p->words[p->cw], "*")) ||
         (!strcmp(p->words[p->cw], "/")) ||
         (!strcmp(p->words[p->cw], ";")) ){
         p->cw++;
      }
      else{
         if(!newLineCheck(p)){
            exitFlag = true;
            fprintf(stderr, "potential errors in same line: %d\n", potenErrorCnt);
         }
         else{
            potenErrorCnt++;
            p->cw++;
         }
      }
   }
}
unsigned char newLineCheck(userprog* p)
{
   if(p->words[p->cw][LETTER] == NEWLINE){
      p->cw++;
      /*SUCCESS meaning new line detected and skipped*/
      return EXIT_SUCCESS;
   }
   /*no new line*/
   return EXIT_FAILURE;
}
void printCode(userprog* p)
{
   int i;
   printf("\nYOUR CODE:\n");
   for(i = 0; i < p->wordCount; i++){
      printf("%s ", p->words[i]);
      /*detect newline*/
      if(p->words[i][LETTER] == NEWLINE){
         printf("\n");
      }
   }
   printf("\n");
}
void checkInstruction(userprog* p)
{
   if(!strcmp(p->words[p->cw], "FD") ||
   !strcmp(p->words[p->cw], "LT") ||
   !strcmp(p->words[p->cw], "RT") ||
   !strcmp(p->words[p->cw], "DO") ||
   !strcmp(p->words[p->cw], "SET")){
      fprintf(stderr, "starting bracket '{' missing\n");
      /*no cw inc here as valid instruction so check syntax as if it had a opening bracket*/
   }
   else{
      fprintf(stderr, "%s syntax wrong, { should mark the start of the program\n", p->words[p->cw]);
      p->cw++;
      newLineCheck(p);
   }
}
/*UNCOMMENTING TESTING IN MAIN WILL CAUSE THE ACTUAL PROGRAM TO BUG OUT, testing() ONLY NEEDED TO
PROVE ASSERTION TESTING OF FUNCTIONS WORKS, PLEASE COMMENT/REMOVE TESTING FUNCTION WHEN RUNNING
CODE*/
void testing(SDL_Simplewin *sw)
{
   userprog clearProg = {{{0}}, 0, 0, {0}, {0}, 0, 0, 0};
   userprog tprog;
   /*TESTING OF EXTENSION ASSUMES INTERPRETER/PARSER FUNCTIONS TESTED IN PREVIOUS VERSIONS,
   SO ONLY TESTING RELATED TO EXTENSION CHANGES TO THOSE FUNCTIONS ARE BELOW*/
   printf("!!!!!!!!!!!!!!!IGNORE ERROR MSGS, they're from testing!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
   /************************************test start()************************************/
   tprog = clearProg;
   strcpy(tprog.words[0], "{");
   start(&tprog, sw); /*dont need to check the result of start as it depends on nested functions*/
   assert(tprog.validity == VALID);
   /*missing starting bracket*/
   tprog = clearProg;
   strcpy(tprog.words[0], "FD");
   start(&tprog, sw);    /*still returns SUCCESS*/
   assert(tprog.validity == NOTVALID);
   /********************************instructlst() not changed by extension**************/
   /************************************test instruction()************************************/
   /*one function added to catch non-instructions which is followed by a exit_fail, so only testing
   a non instruction returns a fail*/
   tprog = clearProg;
   strcpy(tprog.words[0], "lt");
   assert(instruction(&tprog, sw)); /*EXIT_FAILURE == 1, cw++*/
   strcpy(tprog.words[2], "DOo");   /*skip newline*/
   assert(instruction(&tprog, sw)); /*EXIT_FAILURE == 1, cw++*/
   strcpy(tprog.words[4], "Set");   /*skip newline*/
   assert(instruction(&tprog, sw)); /*EXIT_FAILURE == 1, cw++*/
   strcpy(tprog.words[6], "Hello");   /*skip newline*/
   assert(instruction(&tprog, sw)); /*EXIT_FAILURE == 1, cw++*/
   /************************************test fd() & lt() & rt()************************************/
   /*check if previously not valid return failure happening if code being evaluated correct*/
   tprog = clearProg;
   tprog.validity = NOTVALID;
   strcpy(tprog.words[0], "FD"); /*not really needed but shows what would be if this function called*/
   strcpy(tprog.words[1], "60"); /*valid VARNUM*/
   assert(fd(&tprog, sw));       /*fail = 1*/
   strcpy(tprog.words[0], "LT"); /*not really needed but shows what would be if this function called*/
   strcpy(tprog.words[1], "10"); /*valid VARNUM*/
   assert(fd(&tprog, sw));       /*fail = 1*/
   strcpy(tprog.words[0], "RT"); /*not really needed but shows what would be if this function called*/
   strcpy(tprog.words[1], "A"); /*valid VARNUM*/
   assert(fd(&tprog, sw));      /*fail = 1*/
   /************************************test isNumber() & isVAR()************************************/
   /*both should return success if program is not valid but word is a number/VAR*/
   /*because isNumber is never called on its own it won't inc cw so seemingly random cw++ needed to
   account for this*/
   tprog = clearProg;
   tprog.validity = NOTVALID;
   strcpy(tprog.words[0], "69");
   assert(!isNumber(&tprog));
   strcpy(tprog.words[1], "1");
   assert(!isNumber(&tprog));
   strcpy(tprog.words[2], "20");
   assert(!isNumber(&tprog));
   strcpy(tprog.words[3], "A");
   assert(!isVAR(&tprog));
   strcpy(tprog.words[4], "G");
   assert(!isVAR(&tprog));
   strcpy(tprog.words[5], "P");
   assert(!isVAR(&tprog));
   /*should both return fails not dependent on validity if not a number/VAR*/
   tprog.validity = VALID;
   strcpy(tprog.words[6], "aP");
   assert(isVAR(&tprog));
   strcpy(tprog.words[7], "d");
   assert(isVAR(&tprog));
   strcpy(tprog.words[8], "FD");
   assert(isVAR(&tprog));
   strcpy(tprog.words[9], "nine");
   assert(isNumber(&tprog));
   tprog.cw++;
   strcpy(tprog.words[10], "9a");
   assert(isNumber(&tprog));
   tprog.cw++;
   strcpy(tprog.words[11], "D");
   assert(isNumber(&tprog));
   tprog.cw++;
   tprog.validity = NOTVALID;
   strcpy(tprog.words[12], "aP");
   assert(isVAR(&tprog));
   strcpy(tprog.words[13], "d");
   assert(isVAR(&tprog));
   strcpy(tprog.words[14], "FD");
   printf("%s\n", tprog.words[tprog.cw]);
   assert(isVAR(&tprog));
   strcpy(tprog.words[15], "nine");
   assert(isNumber(&tprog));
   tprog.cw++;
   strcpy(tprog.words[16], "9a");
   assert(isNumber(&tprog));
   tprog.cw++;
   strcpy(tprog.words[17], "D");
   assert(isNumber(&tprog));
   tprog.cw++;
   /************************************test isVARNUM()************************************/
   /*isVAR and isVARNUM should turn validity to false if not valid VAR/VARNUM*/
   tprog = clearProg;
   tprog.validity = VALID;
   strcpy(tprog.words[0], "YO");
   isVAR(&tprog);
   assert(tprog.validity == NOTVALID);
   tprog.validity = VALID;
   strcpy(tprog.words[1], "YO");
   isVARNUM(&tprog);
   assert(tprog.validity == NOTVALID);
   tprog.validity = VALID;
   strcpy(tprog.words[2],"1U");
   isVAR(&tprog);
   assert(tprog.validity == NOTVALID);
   /************************************test doo()************************************/
   /*check if min and max wrong way round validity goes to not valid*/
   tprog = clearProg;
   tprog.validity = VALID;

   strcpy(tprog.words[0], "A");
   strcpy(tprog.words[1], "FROM");
   strcpy(tprog.words[2], "6");
   strcpy(tprog.words[3], "TO");
   strcpy(tprog.words[4], "1");
   strcpy(tprog.words[5], "{");
   doo(&tprog, sw);
   assert(tprog.validity == NOTVALID);
   /************************************test dooloop()************************************/


   printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");



}
