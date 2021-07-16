/*Parsing version only*/
   /*I've not used exit() but simply return success or failure back through each function, made testing
a lot easier, although meant error msg were being displayed when there wasnt an error e.g. VARNUM
checks both number and var, one has to fail and that fail was triggering a fail msg, this is part
of the reason why I commented them out*/
   /*Decided whilst doing the interpreter that I wasn't going to allow for floating point numbers to be
read in e.g. from polish, had orginially let the parser pass through '.' but to keep in theme with
interpreter I've gotten rid of it (commented to the side where it was) so any test files you use
with floating point number will fail...*/
   /*Decided to print a not valid output to console if EXIT_FAILURE*/
#include "turtle.h"

void testing(void);

int main(int argc, char** argv)
{
   int row;
   userprog prog;
   assert(argc == ARGNUM);
   /*testing();*/

   /*intialize struct vars*/
   prog.cw = 0;
   for(row = 0; row < MAXWORDS; row++){
      prog.words[row][0] = '\0';
   }
   /*scan words into struct prog.words*/
   scanTurtleFile(&prog, argv[NAMEIND]);

   if(!start(&prog)){
      return EXIT_SUCCESS;
   }
   fprintf(stderr, "not valid.\n");
   return EXIT_FAILURE;
}
unsigned char start(userprog* p)
{
   /*below line in all funcs to remind me/developer struct not linked correctly*/
   assert(p != NULL && "ERROR: struct pointer NULL?");
   /*check first thing is { as per rules*/
   if(strcmp(p->words[p->cw], "{")){
      /*fprintf(stderr, "starting bracket '{' not detected...\n");*/
      return EXIT_FAILURE;
   }
   p->cw++;
   return instructlst(p);
}
unsigned char instructlst(userprog* p)
{
   assert(p != NULL && "ERROR: struct pointer NULL?");
   if(p->cw >= MAXWORDS){
      /*fprintf(stderr, "no end bracket '}' detected? Too many words?\n");*/
      return EXIT_FAILURE;
   }
   if(!strcmp(p->words[p->cw], "}")){
      return EXIT_SUCCESS;
   }
   if(instruction(p) == EXIT_FAILURE){
      return EXIT_FAILURE;
   }
   return instructlst(p);
}
unsigned char instruction(userprog* p)
{
   assert(p != NULL && "ERROR: struct pointer NULL?");
   if(!strcmp(p->words[p->cw], "FD")){
      p->cw++;
      p->ci = Fd;
      return fd(p);
   }
   else if(!strcmp(p->words[p->cw], "LT")){
      p->cw++;
      p->ci = Lt;
      return lt(p);
   }
   else if(!strcmp(p->words[p->cw], "RT")){
      p->cw++;
      p->ci = Rt;
      return rt(p);
   }
   else if(!strcmp(p->words[p->cw], "DO")){
      p->cw++;
      p->ci = Doo;
      return doo(p);
   }
   else if(!strcmp(p->words[p->cw], "SET")){
      p->cw++;
      p->ci = Set;
      return set(p);
   }
   /*fprintf(stderr, "Instruction %s not recognised... (is there a closing bracket }?)\n", p->words[p->cw]);*/
   return EXIT_FAILURE;
}
/*following three functions seem a bit pointless but thinking a head for the interpreter I've seperated,
them out*/
unsigned char fd(userprog* p)
{
   if(!isVARNUM(p)){
      return EXIT_SUCCESS;
   }
   /*fprintf(stderr, "%s not a valid VARNUM (e.g. 60 or A)\n", p->words[p->cw]);*/
   return EXIT_FAILURE;
}
unsigned char lt(userprog* p)
{
   if(!isVARNUM(p)){
      return EXIT_SUCCESS;
   }
   /*fprintf(stderr, "%s not a valid VARNUM (e.g. 60 or A)\n", p->words[p->cw]);*/
   return EXIT_FAILURE;
}
unsigned char rt(userprog* p)
{
   if(!isVARNUM(p)){
      return EXIT_SUCCESS;
   }
   /*fprintf(stderr, "%s not a valid VARNUM (e.g. 60 or A)\n", p->words[p->cw]);*/
   return EXIT_FAILURE;
}
unsigned char isNumber(userprog* p)
{
   int a;
   assert(p != NULL && "ERROR: struct pointer NULL?");
   for(a = 0; a < (int)strlen(p->words[p->cw]); a++){
      if(!isdigit((int)p->words[p->cw][a])){       /* && p->words[p->cw][a] != '.' removed*/
         /*fprintf(stderr, "%s not a number\n", p->words[p->cw]);*/
         return EXIT_FAILURE;
      }
   }
   p->cw++;
   return EXIT_SUCCESS;
}
unsigned int scanTurtleFile(userprog* p, char* fileName)
{
   FILE* fp;
   char name[MAXSTRLEN];
   char temp[MAXSTRLEN];
   int wordCount = 0;

   strcpy(name, fileName);

   if((fp = fopen(name, "r")) == NULL){
      fprintf(stderr, "could not find file with name: '%s'\n", name);
      exit(EXIT_FAILURE);
   }
   while(wordCount < MAXWORDS){
      if(fscanf(fp, "%s", temp) == 1){
         wordCount++;
         strcpy(p->words[p->cw++], temp);
      }
      else{
         p->cw = 0;
         return wordCount;
      }
   }
   /*the only func that exit() is used, and I've left the comment in, as this shouldn't go wrong*/
   /*fprintf(stderr, "No End of File Char read? Program probably has too many words...\n");*/
   fprintf(stderr, "error in scanning file?\n");
   exit(EXIT_FAILURE);
}
/*like fd, lt etc, doesn't DO a whole lot in this version*/
unsigned char doo(userprog* p)
{
   assert(p != NULL && "ERROR: struct pointer NULL?");
   if(!isVAR(p)){
      if(!isFROM(p)){
         if(!isVARNUM(p)){
            if(!isTO(p)){
               if(!isVARNUM(p)){
                  return start(p);
               }
            }
         }
      }
   }
   return EXIT_FAILURE;
}
unsigned char isVAR(userprog* p)
{
   assert(p != NULL && "ERROR: struct pointer NULL?");
   if(p->words[p->cw][0] >= 'A' && p->words[p->cw][0] <= 'Z'){
      /*string should only have one char in it*/
      if(p->words[p->cw][1] == '\0'){
         p->cw++;
         return EXIT_SUCCESS;
      }
   }
   /*fprintf(stderr, "%s not a valid VAR\n", p->words[p->cw]);*/
   return EXIT_FAILURE;
}
unsigned char isFROM(userprog* p)
{
   assert(p != NULL && "ERROR: struct pointer NULL?");
   if(strcmp(p->words[p->cw], "FROM")){
      /*fprintf(stderr, "'FROM' command expected\n");*/
      return EXIT_FAILURE;
   }
   p->cw++;
   return EXIT_SUCCESS;
}
unsigned char isVARNUM(userprog* p)
{
   assert(p != NULL && "ERROR: struct pointer NULL?");
   if(!isNumber(p) || !isVAR(p)){
      return EXIT_SUCCESS;
   }
   return EXIT_FAILURE;
}
unsigned char isTO(userprog* p)
{
   assert(p != NULL && "ERROR: struct pointer NULL?");
   if(strcmp(p->words[p->cw], "TO")){
      /*fprintf(stderr, "'TO' command expected\n");*/
      return EXIT_FAILURE;
   }
   p->cw++;
   return EXIT_SUCCESS;
}
/*similar to other instruction funcs, just a shell for parser*/
unsigned char set(userprog* p)
{
   assert(p != NULL && "ERROR: struct pointer NULL?");
   if(!isVAR(p)){
      if(!isEQ(p)){
         if(!polish(p)){
            return EXIT_SUCCESS;
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
      return EXIT_SUCCESS;
   }
   /*fprintf(stderr, "missing := command?\n");*/
   return EXIT_FAILURE;
}
/*saw you wanted to allow nonlegal RPN formulas in the parser as long as its,
legal to formal grammar so allowing things like + 8 ; 9 9 9 9 + ; B A - / ;*/
unsigned char polish(userprog* p)
{
   assert(p != NULL && "ERROR: struct pointer NULL?");
   if(!strcmp(p->words[p->cw], ";")){
      p->cw++;
      return EXIT_SUCCESS;
   }
   if((!isVARNUM(p) || !isOP(p))){
      return polish(p);
   }
   /*fprintf(stderr, "invalid polish notation\n");*/
   return EXIT_FAILURE;
}
unsigned char isOP(userprog* p)
{
   char ops[OPNUM] = {'+', '-', '*', '/'};
   unsigned char a;
   assert(p != NULL && "ERROR: struct pointer NULL?");
   for(a = 0; a < OPNUM; a++){
      if(p->words[p->cw][0] == ops[a] && p->words[p->cw][1] == '\0'){
         p->cw++;
         return EXIT_SUCCESS;
      }
   }
   /*fprintf(stderr, "%s not a valid operator\n", p->words[p->cw]);*/
   return EXIT_FAILURE;
}
void testing(void)
{
   userprog testprog;
   strcpy(testprog.words[0], "{");
   strcpy(testprog.words[1], "}");
   strcpy(testprog.words[3], "}");
   testprog.cw = 0;
   /************************************test start()************************************/
   /*assert buried in start so checking a NULL triggers it correctly...*/
   assert(start(&testprog) == EXIT_SUCCESS);
   /*causes error/program termination*/
   /*start(NULL);*/
   /*testprog.words[0][0] = '\0';
   start(&testprog);*/
   assert(testprog.cw == 1);
   /************************************test instructlst()************************************/
   /*instructlst(NULL);*/
   assert(instructlst(&testprog) == 0);
   /************************************test instruction()************************************/
   strcpy(testprog.words[2], "60");    /*to prevent nested functions complaining*/
   /*causes exit failure*/
   /*instruction(NULL);*/
   /*strcpy(testprog.words[1], "fd");
   instruction(&testprog);*/
   /*strcpy(testprog.words[1], "DF");
   instruction(&testprog);*/
   /*strcpy(testprog.words[1], "!%");
   instruction(&testprog);*/
   testprog.cw = 1;
   strcpy(testprog.words[1], "FD");
   assert(!instruction(&testprog));
   assert(testprog.ci == Fd);
   testprog.cw = 1;
   strcpy(testprog.words[1], "LT");
   assert(!instruction(&testprog));
   assert(testprog.ci == Lt);
   testprog.cw = 1;
   strcpy(testprog.words[1], "RT");
   assert(!instruction(&testprog)); /* EXIT_SUCCESS == 0 */
   assert(testprog.ci == Rt);
   testprog.cw = 1;
   strcpy(testprog.words[1], "DO");
   instruction(&testprog);
   assert(testprog.ci == Doo);
   testprog.cw = 1;
   strcpy(testprog.words[1], "SET");
   instruction(&testprog);
   assert(testprog.ci == Set);
   /************************************test isNumber()************************************/
   /*Successes*/
   testprog.cw = 1;
   strcpy(testprog.words[1], "50");
   assert(isNumber(&testprog) == EXIT_SUCCESS);
   testprog.cw = 1;
   strcpy(testprog.words[1], "5");
   assert(!isNumber(&testprog));
   testprog.cw = 1;
   strcpy(testprog.words[1], "800");
   assert(!isNumber(&testprog));
   testprog.cw = 1;
   strcpy(testprog.words[1], "050");
   assert(!isNumber(&testprog));
   /*Fails*/
   testprog.cw = 1;
   strcpy(testprog.words[1], "FD");
   assert(isNumber(&testprog) == EXIT_FAILURE);
   testprog.cw = 1;
   strcpy(testprog.words[1], "Fifty");
   assert(isNumber(&testprog));
   testprog.cw = 1;
   strcpy(testprog.words[1], "!50");
   assert(isNumber(&testprog));
   /*below 3 funcs fd, rt, lt almost identical but seperated out in diff funcs for SDL purposes,
   later on*/
   /************************************test fd()************************************/
   /*Completion of function should increment cw by 1 and return EXIT_SUCCESS if varnum vaild*/
   strcpy(testprog.words[5], "}");
   strcpy(testprog.words[2], "50");
   strcpy(testprog.words[1], "FD");
   testprog.cw = 2;
   assert(fd(&testprog) == EXIT_SUCCESS);
   assert(testprog.cw == 2+1);
   strcpy(testprog.words[3], "50");
   assert(fd(&testprog) == EXIT_SUCCESS);
   assert(testprog.cw == 3+1);
   strcpy(testprog.words[4], "50");
   assert(fd(&testprog) == EXIT_SUCCESS);
   assert(testprog.cw == 4+1);
   testprog.cw = 2;
   /*check fails, !!!!!!!PRINTS 3 ERROR MSGS TO CONSOLE*/
   strcpy(testprog.words[2], "ft");
   strcpy(testprog.words[3], "7Y");
   strcpy(testprog.words[4], "LS");
   assert(fd(&testprog) == EXIT_FAILURE);  /*"}" increments cw*/
   assert(fd(&testprog) == EXIT_FAILURE);  /*"ft increments cw"*/
   assert(fd(&testprog) == EXIT_FAILURE);  /*"7y" increments cw*/
   /************************************test lt()************************************/
   testprog.cw = 2;
   strcpy(testprog.words[1], "LT"); /*not needed but for this func to have occured this would have happened*/

   strcpy(testprog.words[2], "9");
   strcpy(testprog.words[3], "45");
   strcpy(testprog.words[4], "360");
   assert(lt(&testprog) == EXIT_SUCCESS);
   assert(testprog.cw == 2+1);
   assert(lt(&testprog) == EXIT_SUCCESS);
   assert(testprog.cw == 3+1);
   assert(lt(&testprog) == EXIT_SUCCESS);
   assert(testprog.cw == 4+1);
   /*return fails, !!!!!!!PRINTS 3 ERROR MSGS TO CONSOLE*/
   strcpy(testprog.words[2], "9Y");
   strcpy(testprog.words[3], "LT");
   strcpy(testprog.words[4], "Tr");
   testprog.cw = 2;
   assert(lt(&testprog) == EXIT_FAILURE);
   assert(lt(&testprog) == EXIT_FAILURE);
   assert(lt(&testprog) == EXIT_FAILURE);
   /************************************test rt()************************************/
   testprog.cw = 2;
   strcpy(testprog.words[1], "LT"); /*not needed but for this func to have occured this would have happened*/

   strcpy(testprog.words[2], "99");
   strcpy(testprog.words[3], "5");
   strcpy(testprog.words[4], "30");
   assert(rt(&testprog) == EXIT_SUCCESS);
   assert(testprog.cw == 2+1);
   assert(rt(&testprog) == EXIT_SUCCESS);
   assert(testprog.cw == 3+1);
   assert(rt(&testprog) == EXIT_SUCCESS);
   assert(testprog.cw == 4+1);
   /*return fails, !!!!!!!PRINTS 3 ERROR MSGS TO CONSOLE*/
   strcpy(testprog.words[2], "RT");
   strcpy(testprog.words[3], "Tfd");
   strcpy(testprog.words[4], "LT");
   testprog.cw = 2;
   assert(rt(&testprog) == EXIT_FAILURE);
   assert(rt(&testprog) == EXIT_FAILURE);
   assert(rt(&testprog) == EXIT_FAILURE);
   /***************************END OF DUMBED DOWN PARSER TESTING****************************************/
   /************************************test isVAR()************************************/
   /*Successes*/
   testprog.cw = 7;
   strcpy(testprog.words[7], "G");
   assert(isVAR(&testprog) == EXIT_SUCCESS);
   testprog.cw = 7;
   strcpy(testprog.words[7], "T");
   assert(!isVAR(&testprog));
   testprog.cw = 7;
   strcpy(testprog.words[7], "A");
   assert(!isVAR(&testprog));
   testprog.cw = 7;
   strcpy(testprog.words[7], "Z");
   assert(!isVAR(&testprog));
   testprog.cw = 7;
   /*Fails*/
   strcpy(testprog.words[7], "t");
   assert(isVAR(&testprog) == EXIT_FAILURE);
   strcpy(testprog.words[7], "1");
   assert(isVAR(&testprog));
   strcpy(testprog.words[7], "AT");
   assert(isVAR(&testprog));
   /************************************test isFROM()************************************/
   /*Success*/
   testprog.cw = 8;
   strcpy(testprog.words[8], "FROM");
   assert(!isFROM(&testprog));
   /*Failure*/
   testprog.cw = 8;
   strcpy(testprog.words[8], "NOTFROM");
   assert(isFROM(&testprog));
   strcpy(testprog.words[8], "from");
   assert(isFROM(&testprog));
   /************************************test isVARNUM()************************************/
   /*Success*/
   testprog.cw = 9;
   strcpy(testprog.words[9], "A");
   assert(!isVARNUM(&testprog));
   testprog.cw = 9;
   strcpy(testprog.words[9], "90");
   assert(!isVARNUM(&testprog));
   testprog.cw = 9;
   strcpy(testprog.words[9], "7");
   assert(!isVARNUM(&testprog));
   testprog.cw = 9;
   strcpy(testprog.words[9], "Z");
   assert(!isVARNUM(&testprog));
   /*Fails*/
   testprog.cw = 9;
   strcpy(testprog.words[9], "a");
   assert(isVARNUM(&testprog));
   strcpy(testprog.words[9], "ninety");
   assert(isVARNUM(&testprog));
   strcpy(testprog.words[9], "a7");
   assert(isVARNUM(&testprog));
   strcpy(testprog.words[9], "ZZ");
   assert(isVARNUM(&testprog));
   /************************************test isTO()************************************/
   /*Success*/
   testprog.cw = 10;
   strcpy(testprog.words[10], "TO");
   assert(!isTO(&testprog));
   /*Failure*/
   testprog.cw = 10;
   strcpy(testprog.words[10], "to");
   assert(isTO(&testprog));
   strcpy(testprog.words[10], "FROM");
   assert(isTO(&testprog));
   /************************************test doo()************************************/
   testprog.cw = 1;
   strcpy(testprog.words[0], "DO"); /*would have been detected in instruction()*/
   strcpy(testprog.words[1], "A");
   strcpy(testprog.words[2], "FROM");
   strcpy(testprog.words[3], "1");
   strcpy(testprog.words[4], "TO");
   strcpy(testprog.words[5], "60");
   strcpy(testprog.words[6], "{");
   strcpy(testprog.words[7], "FD");
   strcpy(testprog.words[8], "A");
   strcpy(testprog.words[9], "}");
   assert(doo(&testprog) == EXIT_SUCCESS);
   /*Fail*/
   testprog.cw = 1;
   strcpy(testprog.words[0], "DO"); /*would have been detected in instruction()*/
   strcpy(testprog.words[1], "60");
   strcpy(testprog.words[2], "FROM");
   strcpy(testprog.words[3], "1");
   strcpy(testprog.words[4], "TO");
   strcpy(testprog.words[5], "60");
   strcpy(testprog.words[6], "{");
   strcpy(testprog.words[7], "FD");
   strcpy(testprog.words[8], "A");
   strcpy(testprog.words[9], "}");
   assert(doo(&testprog) == EXIT_FAILURE);
   /*success*/
   testprog.cw = 1;
   strcpy(testprog.words[0], "DO"); /*would have been detected in instruction()*/
   strcpy(testprog.words[1], "Z");
   strcpy(testprog.words[2], "FROM");
   strcpy(testprog.words[3], "A");
   strcpy(testprog.words[4], "TO");
   strcpy(testprog.words[5], "60");
   strcpy(testprog.words[6], "{");
   strcpy(testprog.words[7], "FD");
   strcpy(testprog.words[8], "Z");
   strcpy(testprog.words[9], "}");
   assert(doo(&testprog) == EXIT_SUCCESS);
   /************************************test isEQ()************************************/
   testprog.cw = 10;
   strcpy(testprog.words[10], ":=");
   assert(isEQ(&testprog) == EXIT_SUCCESS);
   /*Fails*/
   testprog.cw = 10;
   strcpy(testprog.words[10], "=:");
   assert(isEQ(&testprog) == EXIT_FAILURE);
   strcpy(testprog.words[10], "10");
   assert(isEQ(&testprog) == EXIT_FAILURE);
   /************************************test isOP()************************************/
   testprog.cw = 0;
   strcpy(testprog.words[0], "+");
   assert(!isOP(&testprog));
   testprog.cw = 0;
   strcpy(testprog.words[0], "-");
   assert(!isOP(&testprog));
   testprog.cw = 0;
   strcpy(testprog.words[0], "*");
   assert(!isOP(&testprog));
   testprog.cw = 0;
   strcpy(testprog.words[0], "/");
   assert(!isOP(&testprog));
   testprog.cw = 0;
   strcpy(testprog.words[0], "%");
   assert(isOP(&testprog));
   /************************************test polish()************************************/
   testprog.cw = 1;
   strcpy(testprog.words[0], ":="); /*would have been detected in instruction()*/
   strcpy(testprog.words[1], "Z");
   strcpy(testprog.words[2], "A");
   strcpy(testprog.words[3], "+");
   strcpy(testprog.words[4], ";");
   assert(!polish(&testprog));

   testprog.cw = 1;
   strcpy(testprog.words[0], ":="); /*would have been detected in instruction()*/
   strcpy(testprog.words[1], "6");
   strcpy(testprog.words[2], "50");
   strcpy(testprog.words[3], "/");
   strcpy(testprog.words[4], ";");
   assert(!polish(&testprog));

   testprog.cw = 1;
   strcpy(testprog.words[0], ":="); /*would have been detected in instruction()*/
   strcpy(testprog.words[1], "Z");
   strcpy(testprog.words[2], "90");
   strcpy(testprog.words[3], "-");
   strcpy(testprog.words[4], ";");
   assert(!polish(&testprog));

   testprog.cw = 1;
   strcpy(testprog.words[0], ":="); /*would have been detected in instruction()*/
   strcpy(testprog.words[1], "G");
   strcpy(testprog.words[2], "U");
   strcpy(testprog.words[3], "*");
   strcpy(testprog.words[4], ";");
   assert(!polish(&testprog));
   /* FAILS */
   /*not allowing more than 2 operands for RPN*/
   testprog.cw = 1;
   /* C = (A*B)+20, below RPN valid but for simplicity not allowing more than 2 operands per command*/
   /* so user would have to do C = A*B and then C += 20 if they want above formula (2 lines) */
   strcpy(testprog.words[0], "C"); /*would have been detected in instruction()*/
   strcpy(testprog.words[1], ":=");
   strcpy(testprog.words[2], "20");
   strcpy(testprog.words[3], "A");
   strcpy(testprog.words[4], "B");
   strcpy(testprog.words[5], "*");
   strcpy(testprog.words[6], "+");
   strcpy(testprog.words[7], ";");
   assert(polish(&testprog));
   /*not a legal RPN*/
   testprog.cw = 1;
   strcpy(testprog.words[0], "C"); /*would have been detected in instruction()*/
   strcpy(testprog.words[1], ":=");
   strcpy(testprog.words[2], "20");
   strcpy(testprog.words[3], "A");
   strcpy(testprog.words[4], "B");
   strcpy(testprog.words[5], "*");
   strcpy(testprog.words[6], ";");
   assert(polish(&testprog));
   /************************************test isEQ()************************************/
}
