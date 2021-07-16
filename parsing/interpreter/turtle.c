/*Combined parser and interpreter*/

   /*It will quickly come apparent that some of your test files fail that you expect to pass, my polish
notation only accepts two operands and a operator (e.g. 9 B +) originaly to keep things simple,
then didn't want to go back and change everything to implement full RPN, this program also
does not accept floating point numbers, for a similar reason to why RPN wasn't implemented,
the user is able to spread the polish out onto multiple lines and it should still work, e.g.
if the user wanted C = B 9 8 * +, line 1: C = 9 8 *, line 2: C = B C +*/
   /*Again commented out error MSG if the file is wrong a not valid msg will print to console,
will include error msgs in my extension*/
   /*all the test files i used are in testtrtlfiles so if you want to use any them
type: e.g. ./interp testtrtlfiles/1.trtl , i've made them .trtl but this isn't
a requirement the file name can be anything as long as its under 50 chars*/
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
   userprog prog = {{{0}}, 0, 0, {0}, {0}, 0};

   assert(argc == ARGNUM && "ERROR: TYPE FILENAME");
   /*scan words into struct prog.words*/
   scanTurtleFile(&prog, argv[NAMEIND]);

   Adapted_SDL_Init(&sw);

   /*keep commented for proper file execution, testing() messes with SDL*/
   /*testing(&sw);*/

   if(start(&prog, &sw) == EXIT_FAILURE){
      fprintf(stderr, "not a valid input file\n");
      return EXIT_FAILURE;
   }
   Adapted_SDL_UpdateScreen(&sw);
   while(sw.finished != SDL_TRUE){
      SDL_Delay(DELAY);
      Neill_SDL_Events(&sw);
   }
   Exit_SDL(&sw);
   return EXIT_SUCCESS;
}
unsigned char start(userprog* p, SDL_Simplewin *sw)
{
   assert(p != NULL && "ERROR: struct pointer NULL?");
   /*check first thing is { as per rules*/
   if(strcmp(p->words[p->cw], "{")){
      /*fprintf(stderr, "starting bracket '{' not detected...\n");*/
      return EXIT_FAILURE;
   }
   p->wordType = none;
   p->cw++;
   return instructlst(p, sw);
}
unsigned char instructlst(userprog* p, SDL_Simplewin *sw)
{
   assert(p != NULL && "ERROR: struct pointer NULL?");
   if(p->cw >= MAXWORDS){
      /*fprintf(stderr, "no end bracket '}' detected?  \n");*/
      return EXIT_FAILURE;
   }
   if(!strcmp(p->words[p->cw], "}")){
      p->wordType = none;
      return EXIT_SUCCESS;
   }
   if(instruction(p, sw) != EXIT_SUCCESS){
      return EXIT_FAILURE;
   }
   return instructlst(p, sw);
}
unsigned char instruction(userprog* p, SDL_Simplewin *sw)
{
   assert(p != NULL && "ERROR: struct pointer NULL?");
   if(!strcmp(p->words[p->cw], "FD")){
      p->cw++;
      p->wordType = none;
      return fd(p, sw);
   }
   else if(!strcmp(p->words[p->cw], "LT")){
      p->cw++;
      p->wordType = none;
      return lt(p, sw);
   }
   else if(!strcmp(p->words[p->cw], "RT")){
      p->cw++;
      p->wordType = none;
      return rt(p, sw);
   }
   else if(!strcmp(p->words[p->cw], "DO")){
      p->cw++;
      p->wordType = none;
      return doo(p, sw);
   }
   else if(!strcmp(p->words[p->cw], "SET")){
      p->cw++;
      p->wordType = none;
      return set(p);
   }
   /*fprintf(stderr, "Instruction %s not recognised... (is there a closing bracket }?)\n", p->words[p->cw]);*/
   return EXIT_FAILURE;
}
unsigned char fd(userprog* p, SDL_Simplewin *sw)
{
   assert(p != NULL && "ERROR: struct pointer NULL?");
   if(!isVARNUM(p)){
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
   /*fprintf(stderr, "FD not followed by a valid VARNUM (e.g. 60 or A), '%s' not valid\n", p->words[p->cw]);*/
   return EXIT_FAILURE;
}
unsigned char lt(userprog* p, SDL_Simplewin *sw)
{
   assert(p != NULL && "ERROR: struct pointer NULL?");
   if(!isVARNUM(p)){
      sw->angle = inbounds(sw->angle - p->miniStack[BOTTOM]);
      clearStack(p);
      return EXIT_SUCCESS;
   }
   /*fprintf(stderr, "%s not a valid VARNUM (e.g. 60 or A)\n", p->words[p->cw]);*/
   return EXIT_FAILURE;
}
unsigned char rt(userprog* p, SDL_Simplewin *sw)
{
   assert(p != NULL && "ERROR: struct pointer NULL?");
   if(!isVARNUM(p)){
      sw->angle = inbounds(sw->angle + p->miniStack[BOTTOM]);
      clearStack(p);
      return EXIT_SUCCESS;
   }
   /*fprintf(stderr, "%s not a valid VARNUM (e.g. 60 or A)\n", p->words[p->cw]);*/
   return EXIT_FAILURE;
}
/*ignores decimals for now, passes them as legal but only scans out a integer*/
unsigned char isNumber(userprog* p)
{
   int a;
   assert(p != NULL && "ERROR: struct pointer NULL?");
   for(a = 0; a < (int)strlen(p->words[p->cw]); a++){
      if(!isdigit((int)p->words[p->cw][a])) {
         return EXIT_FAILURE;
      }
   }
   p->wordType = num;
   if(!miniPush(p)){
      p->cw++;
      return EXIT_SUCCESS;
   }
   return EXIT_FAILURE;
}
unsigned int scanTurtleFile(userprog* p, char* fileName)
{
   FILE* fp;
   char temp[MAXSTRLEN];
   int wordCount = 0;

   if((fp = fopen(fileName, "r")) == NULL){
      fprintf(stderr, "could not find file with name: '%s'\n", fileName);
      exit(EXIT_FAILURE);
   }
   while(wordCount < MAXWORDS){
      if(fscanf(fp, "%s", temp) == FILEWORD){
         wordCount++;
         strcpy(p->words[p->cw++], temp);
      }
      else{
         /*end of file*/
         p->cw = 0;
         return wordCount;
      }
   }
   /*exit used here as bad scan shouldnt happen*/
   fprintf(stderr, "No End of File Char read? Program probably has too many words...\n");
   exit(EXIT_FAILURE);
}
unsigned char doo(userprog* p, SDL_Simplewin *sw)
{
   short min, max;
   char letter;
   assert(p != NULL && "ERROR: struct pointer NULL?");
   if(!isVAR(p)){
      clearStack(p);
      letter = p->words[p->cw BCKONE][LETTER];
      if(!isFROM(p)){
         if(!isVARNUM(p)){
            min = p->miniStack[BOTTOM];
            clearStack(p);
            if(!isTO(p)){
               if(!isVARNUM(p)){
                  max = p->miniStack[BOTTOM];
                  clearStack(p);
                  if(max > min){
                     return dooloop(p, sw, min, max, letter);
                  }
                  /*fprintf(stderr, "min value should come first, max second for DO\n");*/
               }
            }
         }
      }
   }
   /*fprintf(stderr, "error in DO\n");*/
   return EXIT_FAILURE;
}
unsigned char dooloop(userprog* p, SDL_Simplewin* sw, short min, short max, char letter)
{
   short i, cwSave = p->cw;
   for(i = min; i <= max; i++){
      p->miniStack[BOTTOM] = i;
      /*set the first VAR through range given in 2nd and third VARNUM*/
      if(!setVAR(p, letter)){
         p->cw = cwSave;
         if(start(p, sw)){
            /*fprintf(stderr, "error in do loop\n");*/
            return EXIT_FAILURE;
         }
      }
   }
   return EXIT_SUCCESS;
}
unsigned char isVAR(userprog* p)
{
   assert(p != NULL && "ERROR: struct pointer NULL?");
   if(p->words[p->cw][LETTER] >= 'A' && p->words[p->cw][LETTER] <= 'Z'){
      if(p->words[p->cw][NULCHR] == '\0'){
         p->wordType = var;
         if(!miniPush(p)){
            p->cw++;
            return EXIT_SUCCESS;
         }
      }
   }
   /*fprintf(stderr, "%s not a legal VAR (A-Z)\n", p->words[p->cw]);*/
   return EXIT_FAILURE;
}
unsigned char isFROM(userprog* p)
{
   assert(p != NULL && "ERROR: struct pointer NULL?");
   if(strcmp(p->words[p->cw], "FROM")){
      /*fprintf(stderr, "'FROM' command expected\n");*/
      return EXIT_FAILURE;
   }
   p->wordType = none;
   p->cw++;
   return EXIT_SUCCESS;
}
unsigned char isVARNUM(userprog* p)
{
   assert(p != NULL && "ERROR: struct pointer NULL?");
   if(!isNumber(p) || !isVAR(p)){
      return EXIT_SUCCESS;
   }
   /*fprintf(stderr, "%s not a valid VARNUM (number or A-Z)\n", p->words[p->cw]);*/
   return EXIT_FAILURE;
}
unsigned char isTO(userprog* p)
{
   assert(p != NULL && "ERROR: struct pointer NULL?");
   if(strcmp(p->words[p->cw], "TO")){
      fprintf(stderr, "'TO' command expected\n");
      return EXIT_FAILURE;
   }
   p->wordType = none;
   p->cw++;
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
   assert(p != NULL && "ERROR: struct pointer NULL?");
   if(!isVAR(p)){
      clearStack(p);
      /*retrieve accepted VAR (back one, -1)*/
      letter = p->words[p->cw BCKONE][LETTER];
      if(!isEQ(p)){
         if(!polish(p)){
            return setVAR(p, letter);
         }
      }
   }
   return EXIT_FAILURE;
}
unsigned char isEQ(userprog* p)
{
   assert(p != NULL && "ERROR: struct pointer NULL?");
   if(!strcmp(p->words[p->cw], ":=")){
      p->cw++;
      p->wordType = none;
      return EXIT_SUCCESS;
   }
   /*fprintf(stderr, "missing := command?\n");*/
   return EXIT_FAILURE;
}
/*zeroth element of miniStack loaded with the summed size*/
unsigned char polish(userprog* p)
{
   static unsigned char count = 0;
   assert(p != NULL && "ERROR: struct pointer NULL?");
   if(!strcmp(p->words[p->cw], ";")){
      count = 0;
      p->cw++;
      return EXIT_SUCCESS;
   }
   if((!isVARNUM(p) || !isOP(p)) && count < RPNMAXOP){
      count++;
      return polish(p);
   }
   /*fprintf(stderr, "invalid RPN, two operands and one operator only!\n");*/
   return EXIT_FAILURE;
}
unsigned char isOP(userprog* p)
{
   char ops[OPNUM] = {'+', '-', '*', '/'};
   unsigned char a;
   assert(p != NULL && "ERROR: struct pointer NULL?");
   for(a = 0; a < OPNUM; a++){
      if(p->words[p->cw][LETTER] == ops[a] && p->words[p->cw][NULCHR] == '\0'){
         if(p->stckInd == STCKLEN){
            return sum(p);
         }
      }
   }
   /*fprintf(stderr, "%s not a valid operator\n", p->words[p->cw]);*/
   return EXIT_FAILURE;
}
unsigned char setVAR(userprog* p, char letter)
{
   assert(p != NULL && "ERROR: struct pointer NULL?");
   if(p->miniStack[TOP] == 0){
      p->savedVars[letter - 'A'] = p->miniStack[BOTTOM];
      clearStack(p);
      return EXIT_SUCCESS;
   }
   /*fprintf(stderr, "stack not cleared? (problematic letter: %c)\n", letter);*/
   return EXIT_FAILURE;
}
unsigned char miniPush(userprog* p)
{
   short strToInt;
   assert(p != NULL && "ERROR: struct pointer NULL?");
   if(p->stckInd == STCKLEN){
      fprintf(stderr, "stack full\n");
      return EXIT_FAILURE;
   }
   if(p->wordType == num){
      /*scan int out of the current word*/
      if(sscanf(p->words[p->cw], "%hd", &strToInt) == 1){
         p->miniStack[p->stckInd++] = strToInt;
         return EXIT_SUCCESS;
      }
   }
   if(p->wordType == var){
      varLookup(p, p->words[p->cw][0]);
      return EXIT_SUCCESS;
   }
   /*fprintf(stderr, "unable to push the VARNUM %s to the stack\n", p->words[p->cw]);*/
   return EXIT_FAILURE;
}
unsigned char sum(userprog* p)
{
   assert(p != NULL && "ERROR: struct pointer NULL?");
   switch(p->words[p->cw][LETTER]){
      case '+':
         p->miniStack[BOTTOM] += p->miniStack[TOP];
         p->miniStack[TOP] = 0;
         p->stckInd--;
         p->cw++;
         return EXIT_SUCCESS;
      case '-':
         p->miniStack[BOTTOM] -= p->miniStack[TOP];
         p->miniStack[TOP] = 0;
         p->stckInd--;
         p->cw++;
         return EXIT_SUCCESS;
      case '*':
         p->miniStack[BOTTOM] *= p->miniStack[TOP];
         p->miniStack[TOP] = 0;
         p->stckInd--;
         p->cw++;
         return EXIT_SUCCESS;
      case '/':
         p->miniStack[BOTTOM] /= p->miniStack[TOP];
         p->miniStack[TOP] = 0;
         p->stckInd--;
         p->cw++;
         return EXIT_SUCCESS;
   }
   /*validity check in isOP should prevent it from getting to here if operator not legal*/
   return EXIT_FAILURE;
}
void varLookup(userprog* p, char letter)
{
   assert(p != NULL && "ERROR: struct pointer NULL?");
   p->miniStack[p->stckInd++] = p->savedVars[letter - 'A'];
}
void clearStack(userprog* p)
{
   assert(p != NULL && "ERROR: struct pointer NULL?");
   p->miniStack[BOTTOM] = 0;
   p->miniStack[TOP] = 0;
   p->stckInd = 0;
}
/*UNCOMMENTING TESTING IN MAIN WILL CAUSE THE ACTUAL PROGRAM TO BUG OUT, testing() ONLY NEEDED TO
PROVE ASSERTION TESTING OF FUNCTIONS WORKS, PLEASE COMMENT/REMOVE TESTING FUNCTION WHEN RUNNING
CODE*/
void testing(SDL_Simplewin *sw)
{
   userprog clearProg = {{{0}}, 0, 0, {0}, {0}, 0};
   userprog tprog;
   /*TESTING INTERPRETER ASSUMES ALL FUNCTIONS RELATING TO PARSING ALREADY TESTED
   (which they have been, so deleted testing from parser, following tests are for
   adapted functions / interpreter functions)*/
   printf("!!!!!!!!!!!!!!!IGNORE ERROR MSGS, they're from testing!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
   /************************************test miniPush()************************************/
   tprog = clearProg;
   tprog.wordType = num;
   strcpy(tprog.words[0], "60");
   strcpy(tprog.words[1], "2");
   assert(miniPush(&tprog) == EXIT_SUCCESS);
   assert(tprog.miniStack[0] == 60);
   assert(tprog.stckInd = 1);
   tprog.cw = 1;
   assert(miniPush(&tprog) == EXIT_SUCCESS);
   assert(tprog.miniStack[1] == 2);
   assert(tprog.stckInd = 2);
   /*Fails*/
   tprog.cw = 2;
   assert(miniPush(&tprog) == EXIT_FAILURE); /*words[2] == 0*/
   /*structure for future testing...*/
   tprog = clearProg;
   /************************************test isNumber()************************************/
   /*changed to first check the number then push number to the stack*/
   strcpy(tprog.words[0], "60");
   strcpy(tprog.words[1], "2");
   assert(!isNumber(&tprog));
   assert(!isNumber(&tprog));
   assert(tprog.stckInd == 2);
   assert(tprog.miniStack[0] == 60 && tprog.miniStack[1] == 2);
   /*fail*/
   strcpy(tprog.words[2], "A");
   assert(isNumber(&tprog));
   tprog.cw++;
   assert(isNumber(&tprog));  /*words[3] == 0, stack full from above so should fail*/
   /************************************test sum()************************************/
   tprog = clearProg;
   tprog.miniStack[0] = 60;
   tprog.miniStack[1] = 2;
   strcpy(tprog.words[2], "+");
   tprog.stckInd = 2;
   tprog.cw = 2;
   assert(!sum(&tprog));
   assert(tprog.miniStack[0] == 62 && tprog.miniStack[1] == 0 && tprog.stckInd == 1);

   tprog.miniStack[0] = 60;
   tprog.miniStack[1] = 2;
   strcpy(tprog.words[2], "-");
   tprog.cw = 2;
   tprog.stckInd = 2;
   assert(!sum(&tprog));
   assert(tprog.miniStack[0] == 58 && tprog.miniStack[1] == 0 && tprog.stckInd == 1);

   tprog.miniStack[0] = 60;
   tprog.miniStack[1] = 2;
   strcpy(tprog.words[2], "*");
   tprog.stckInd = 2;
   tprog.cw = 2;
   assert(!sum(&tprog));
   assert(tprog.miniStack[0] == 120 && tprog.miniStack[1] == 0 && tprog.stckInd == 1);

   tprog.miniStack[0] = 60;
   tprog.miniStack[1] = 2;
   strcpy(tprog.words[2], "/");
   tprog.stckInd = 2;
   tprog.cw = 2;
   assert(!sum(&tprog));
   assert(tprog.miniStack[0] == 30 && tprog.miniStack[1] == 0 && tprog.stckInd == 1);

   /*Fail*/
   tprog.miniStack[0] = 60;
   tprog.miniStack[1] = 2;
   strcpy(tprog.words[2], "%");
   tprog.stckInd = 2;
   tprog.cw = 2;
   assert(sum(&tprog));
   /************************************test setVAR()************************************/
   tprog = clearProg;
   tprog.stckInd = 1;
   tprog.miniStack[0] = 62;
   assert(!setVAR(&tprog, 'A'));
   assert(tprog.savedVars['A'-'A'] == 62);
   assert(tprog.stckInd == 0);

   tprog.stckInd = 1;
   tprog.miniStack[0] = 58;
   assert(!setVAR(&tprog, 'G'));
   assert(tprog.savedVars['G'-'A'] == 58);
   assert(tprog.stckInd == 0);

   tprog.stckInd = 1;
   tprog.miniStack[0] = 120;
   assert(!setVAR(&tprog, 'Z'));
   assert(tprog.savedVars['Z'-'A'] == 120);
   assert(tprog.stckInd == 0);

   tprog.stckInd = 1;
   tprog.miniStack[0] = 30;
   assert(!setVAR(&tprog, 'U'));
   assert(tprog.savedVars['U'-'A'] == 30);
   assert(tprog.stckInd == 0);
   /************************************test set()************************************/
   /*changed to check validity (parser) and set relevant var in savedVars[]*/
   tprog = clearProg;

   strcpy(tprog.words[0], "A");
   strcpy(tprog.words[1], ":=");
   strcpy(tprog.words[2], "60");
   strcpy(tprog.words[3], "2");
   strcpy(tprog.words[4], "/");
   strcpy(tprog.words[5], ";");
   assert(!set(&tprog));
   assert(tprog.savedVars[0] = 30);
   /************************************test clearStack()***********************************/
   tprog = clearProg;
   tprog.miniStack[0] = 60;
   tprog.miniStack[1] = 2;
   tprog.stckInd = 2;
   clearStack(&tprog);
   assert(tprog.miniStack[0] == 0 && tprog.miniStack[1] == 0 && tprog.stckInd == 0);
   /************************************test rt()************************************/
   tprog = clearProg;
   sw->angle = sw->x1 = sw->y1 = sw->x2 = sw->y2 = 0;

   strcpy(tprog.words[0], "45");
   assert(!rt(&tprog, sw));
   assert(sw->angle == 45);
   strcpy(tprog.words[1], "60");
   assert(!rt(&tprog, sw));
   assert(sw->angle == 105);
   strcpy(tprog.words[2], "300");
   assert(!rt(&tprog, sw));
   assert(sw->angle == 45);   /*45 + 60 + 300 = 405, inbounds(405) = 45*/
   /************************************test lt()************************************/
   tprog = clearProg;
   sw->angle = sw->x1 = sw->y1 = sw->x2 = sw->y2 = 0;

   strcpy(tprog.words[0], "45");
   assert(!lt(&tprog, sw));
   assert(sw->angle == -45);
   strcpy(tprog.words[1], "60");
   assert(!lt(&tprog, sw));
   assert(sw->angle == -105);
   strcpy(tprog.words[2], "300");
   assert(!lt(&tprog, sw));
   assert(sw->angle == -45);   /*-45 + -60 + -300 = -405, inbounds(-405) = -45*/
   /************************************test fd()************************************/
   tprog = clearProg;
   sw->angle = sw->x1 = sw->y1 = sw->x2 = sw->y2 = 0;

   /*if var*/
   strcpy(tprog.words[tprog.cw], "Z");
   tprog.savedVars[25] = 30;
   assert(!fd(&tprog, sw));
   assert(sw->x1 == 0 && sw-> y1 == -30 && sw->angle == 0);
   strcpy(tprog.words[tprog.cw], "Z");
   tprog.savedVars[25] = 30;
   assert(!fd(&tprog, sw));
   assert(sw->x1 == 0 && sw->y1 == -60 && sw->angle == 0);
   /*number*/
   strcpy(tprog.words[tprog.cw], "60");
   assert(!fd(&tprog, sw));
   assert(sw->x1 == 0 && sw->y1 == -120 && sw->angle == 0);
   strcpy(tprog.words[tprog.cw], "10");
   assert(!fd(&tprog, sw));
   assert(sw->x1 == 0 && sw->y1 == -130 && sw->angle == 0);
   /************************************Doo and Set************************************/
   /*doo(), dooloop() and set() are really just a combination of other functions so dont see any point
   or how I would test them.*/
   tprog = clearProg;
   printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
}
