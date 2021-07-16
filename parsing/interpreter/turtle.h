/*Combined parser and interpreter*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include "adaptedsdl2.h"

#define MAXWORDS 100    /*user program can only be have 100 words */
#define MAXSTRLEN 50    /*max word length 50?*/
#define ALPHABETLEN 26
#define OPNUM 4
#define RPNMAXOP 3
#define ARGNUM 2
#define NAMEIND 1
#define STCKLEN 2
#define WHITE 255
#define DELAY 100
#define DGR2RAD(a) (a * 0.017453292519)
#define BOTTOM 0
#define TOP 1
#define BCKONE -1
#define LETTER 0
#define NULCHR 1
#define FILEWORD 1

typedef enum bool {false, true} bool;
typedef enum numOrVar {none, num, var} nOV;

typedef struct userprog {
   char words[MAXWORDS][MAXSTRLEN];
   short cw;
   nOV wordType;
   short savedVars[ALPHABETLEN];
   short miniStack[STCKLEN];
   unsigned char stckInd;
} userprog;

/*PARSING FUNCTIONS - checking validity*/
/*checks for starting bracket*/
unsigned char start(userprog* p, SDL_Simplewin *sw);
/*checks for end bracket or jump to instruction then call self*/
unsigned char instructlst(userprog* p, SDL_Simplewin *sw);
/*branch to words matching function*/
unsigned char instruction(userprog* p, SDL_Simplewin *sw);
/*checks valid varnum*/
unsigned char fd(userprog* p, SDL_Simplewin *sw);
/*checks valid varnum*/
unsigned char lt(userprog* p, SDL_Simplewin *sw);
/*checks valid digit varnum*/
unsigned char rt(userprog* p, SDL_Simplewin *sw);
/*checks if ascii code represents 0-9*/
unsigned char isNumber(userprog* p);
/*scans in file, returns how many words scanned*/
unsigned int scanTurtleFile(userprog* p, char* fileName);
/*chains func to check for a valid DO line*/
unsigned char doo(userprog* p, SDL_Simplewin *sw);
/*checks valid VAR lettering*/
unsigned char isVAR(userprog* p);
/*checks for "FROM" command*/
unsigned char isFROM(userprog* p);
/*gets the varnum referenced*/
unsigned char isVARNUM(userprog* p);
/*checks for "TO" command*/
unsigned char isTO(userprog* p);
/*check for valid SET line*/
unsigned char set(userprog* p);
/*checks for :=*/
unsigned char isEQ(userprog* p);
/*polish check*/
unsigned char polish(userprog* p);
/*checks for operator*/
unsigned char isOP(userprog* p);

/*INTERPRETER FUNCTIONS*/
/*keeps magnitude below 360*/
int inbounds(int a);
/*push value onto miniStack*/
unsigned char miniPush(userprog* p);
/*decides which sum should be performed depending on op*/
unsigned char sum(userprog* p);
/*saves a var for use later*/
unsigned char setVAR(userprog* p, char letter);
/*loads var into zeroth element of stack, even when empty*/
void varLookup(userprog* p, char letter);
/*clears stack and stckInd*/
void clearStack(userprog* p);
/*carries out the do loop*/
unsigned char dooloop(userprog* p, SDL_Simplewin* sw, short min, short max, char letter);
