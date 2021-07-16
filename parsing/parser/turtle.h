/*Parsing version only*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#define MAXWORDS 100
#define MAXSTRLEN 50
#define ALPHABETLEN 26
#define OPNUM 4
#define RPNMAXOP 3
#define ARGNUM 2
#define NAMEIND 1

typedef enum bool {false, true} bool;
typedef enum curIns {Fd, Lt, Rt, Doo, Set} curIns;

typedef struct userprog {
   char words[MAXWORDS][MAXSTRLEN];
   short cw;
   curIns ci;                    /*for testing branching only*/
   short savedVars[ALPHABETLEN]; /*for thinking ahead to the interpreter*/
} userprog;

/*PARSING FUNCTIONS - checking validity*/
/*checks for starting bracket*/
unsigned char start(userprog* p);
/*checks for end bracket or jump to instruction then call self*/
unsigned char instructlst(userprog* p);
/*branch to words matching function*/
unsigned char instruction(userprog* p);
/*checks valid varnum*/
unsigned char fd(userprog* p);
/*checks valid varnum*/
unsigned char lt(userprog* p);
/*checks valid digit varnum*/
unsigned char rt(userprog* p);
/*checks if ascii code represents 0-9*/
unsigned char isNumber(userprog* p);
/*scans in file*/
unsigned int scanTurtleFile(userprog* p, char* fileName);
/*chains func to check for a valid DO line*/
unsigned char doo(userprog* p);
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

/********************FULL PARSER******************
<MAIN>         ::= "{" <INSTRUCTLST>
<INSTRUCTLST>  ::= <INSTRUCTION> <INSTRUCTLST> | "}"
<INSTRUCTION>  ::= <FD> | <LT> | <RT> | <DO> | <SET>
<FD>           ::= "FD" <VARNUM>
<LT>           ::= "LT" <VARNUM>
<RT>           ::= "RT" <VARNUM>
<DO>           ::= "DO" <VAR> "FROM" <VARNUM> "TO" <VARNUM> "{" <INSTRUCTLST>
<VAR>          ::= [A-Z]
<VARNUM>       ::= number | VAR
<SET>          ::= "SET" <VAR> ":=" <POLISH>
<POLISH>       ::= <OP> <POLISH> | <VARNUM> <POLISH> | ";"
<OP>           ::= "+" | "-" | "*" | "/"
**********************************************************/
