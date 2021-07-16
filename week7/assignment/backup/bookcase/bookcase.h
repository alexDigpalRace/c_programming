#include<stdlib.h>
#include<stdio.h>
#include<assert.h>
#include<string.h>
#include<ctype.h>

#define MAXLW 9
#define MXINDX -1
#define PARCILDMX 1000000
#define ERRORHAP -999999
#define TSTLST 6
#define FIRSTBK 0
#define EXCEMPY 1
#define FSTPARID -1
#define MXNAME 30
#define LW 3
#define LWH 5
#define FINAME 1
#define ARGTWO 2
#define WITHFLE 2
#define WITHVER 3
#define ORIGPAR 0
#define ORIGCNT 1
/*for your files Neill only newline needed to be removed from the string i
scanned in from the top line, for my own test.bc files i had to remove TWO chars,
as the atom editor im using adds a cr invisible to the end of a newline,
i've left it as 1 as your files you try should be fine but thought it'd be worth
mentioning, thanks*/
#define CHARTOREMOVE 1

typedef enum bool {false, true} bool;
typedef enum bcstate {e = 0, k, r, g, y, b, m, c, w} bcstate;
typedef struct bookcase{
   int id;
   bcstate book[MAXLW][MAXLW];
   short parentID;
   char row;
   char col;
} bookcase;

void test(void);
/*so i dont have to check when i use calloc*/
void* ncalloc(int n, size_t size);
/*checks whether a whole row is emptyBookcase*/
bool shelfEmpty(bookcase* bcp, int rowNum);
/*checks whether a row has an empty*/
bool shelfHasEmpty(bookcase* bcp, int rowNum);
/*checks if a whole row is full*/
bool shelfFull(bookcase* bcp, int rowNum);
/*returns the index of the right most cell that isnt empty, assumes the shelf or empty*/
int rightestBook(bookcase* bcp, int rowNum);
/*copies shelf from src to a shelf in dest, keeps track of unique ID and parent ID*/
void copyShelf(bookcase* dest, bookcase* src);
/*returns how many moves there are to make from the board*/
int checkMoves(bookcase* bcp);
/*returns the index of the left most empty cell, assumes there is an empty cell...*/
int findEmpty(bookcase* bcp, int rowNum);
/*moves book at (bookRow,bookCol) to (newRow,newCol)*/
bool moveBook(bookcase* bcp, int bookRow, int bookCol, int newRow, int newCol);
/*generate the next child board based off the parent board and what other child boards have been done*/
bool generateChilds(bookcase* bcp, int moves);
/*prevents the number inbounds from going to max or beyond*/
int keepInbounds(int inbounds, int max);
/*checks if a row on the shelf is happy*/
bool checkHappyShelf(bookcase* bcp, int rowNum);
/*to check a happyCase happens when a colour is only on one shelf, prevents against same colour on
two different shelves but rest empty so false positive of checkHappyShelf occurring*/
bool checkColumnZero(bookcase* bcp); /*needs more testing*/
/*checks a whole bookcase is happy, by checking shelf happiness and for colours on different
shelves that should be on the same*/
bool caseHappy(bookcase* bcp);
/*carried out once, when file scanned too check the boards validty*/
bool boardValid(bookcase* bcp);
/*calculates the solution length*/
int calculateLength(bookcase* solution, bookcase list[PARCILDMX]);
/*prints board*/
void printBookcase(bookcase* bcp);
/*converts the enum state to correct the char value*/
char bcstateFntCnvtr(bcstate letter);
/*for verbose, will print solution chain in reverse order...*/
void verbose(bookcase* solution, bookcase list[PARCILDMX]);
/*converts a scanned char to its bcstate equivalent and returns the value*/
bcstate scanToBcstate(char letter);
/*scans a valid .bc file or exit failure*/
int scanBCFile(bookcase* bcp, char name[MXNAME]);
/*zeros a char array*/
void zeroArr(char arr[MAXLW]);
/*for testing generateChilds*/
bool testCmpArray(bookcase* bcp1, bookcase* bcp2);
