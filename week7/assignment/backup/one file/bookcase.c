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
#define TMEOUT 100
#define LW 4
#define LWH 6
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
#define TITLE 1
#define NWLNE 1

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

int main(int argc, char* argv[])
{
   bookcase* bookcaseList;
   bool verboseFlag = false;
   bool exitFlag = false;
   bool noSolFlag = false;
   char strVerb[MXNAME] = "verbose";
   int moves, solutionID;
   int a, b;
   int slot = 0;
   int nextFreeSlot = slot + 1;
   bookcaseList = (bookcase*) ncalloc(PARCILDMX, sizeof(bookcase));

   test();

   /*check either 2 or 3 cmd line args*/
   if(argc != WITHFLE && argc != WITHVER){
      fprintf(stderr, "incorrect amount of cmdline arguments\n");
      exit(EXIT_FAILURE);
   }
   /*check if verbose has been entered*/
   if(argc == WITHVER && !(strcmp(argv[ARGTWO], strVerb))){
      verboseFlag = true;
   }

   /*load in bookcase to zeroth slot in array structure, dont do anything with returned lineCount*/
   scanBCFile(&bookcaseList[slot], argv[FINAME]);
   bookcaseList[slot].id = slot;
   /*CHECK LOADED IN BOARD IS VALID*/
   if(!boardValid(&bookcaseList[slot])){
      fprintf(stderr, "scanned board is not valid, exiting...\n");
      exit(EXIT_FAILURE);
   }
   /*check if its happy*/
   if(caseHappy(&bookcaseList[slot])){
      exitFlag = true;
      solutionID = slot;
   }

   /*loop until solution is found*/
   while(!exitFlag){
      /*calculate moves the board can make*/
      moves = checkMoves(&bookcaseList[slot]);
       /*load in a copy of dummy board to the next (moves) amount of the array*/
      for(a = nextFreeSlot; a < nextFreeSlot+moves; a++){
         if(a >= PARCILDMX){
            exitFlag = true;
            noSolFlag = true;
         }
         else{
            copyShelf(&bookcaseList[a], &bookcaseList[slot]);
            bookcaseList[a].id = a;
         }
      }
      b = nextFreeSlot;
      nextFreeSlot += moves;
      /*generate next moves of parent in the child slots*/
      while(generateChilds(&bookcaseList[b], moves) && !exitFlag){
         /*child generated so check if child bookcase is happy, if it
         is flag to stop generating childs*/
         if(caseHappy(&bookcaseList[b])){
            exitFlag = true;
            solutionID = b;
         }
         b++;
      }
      if(!exitFlag){
         slot++;
      }
   }
   if(noSolFlag){
      printf("No Solution?\n");
   }
   else{
      /*calculate solution length by chasing the parent back to 0 and including 0*/
      printf("%d\n", calculateLength(&bookcaseList[solutionID], bookcaseList));
      if(verboseFlag){
         printf("\n");
         verbose(&bookcaseList[solutionID], bookcaseList);
      }
   }
   free(bookcaseList);
   return 1;
}

void test(void)
{
   int movesMade = 0;
   short a, b;
   bookcase *testList;
   bookcase testBookcase[11];
   bookcase savedBookcase;
   char word[10] = "hello";
   bookcase tmp0 = {0, {{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0}}, 0, 0, 0};
   bookcase tmp1 = {0, {{1,1,2,3,0,0},{2,8,0,0,0,0},{1,0,0,0,0,0}}, 0, 3, 6};
   bookcase tmp2 = {0, {{1,0,0},{5,8,0},{1,1,2}}, 0, 3, 3};
   bookcase tmp3 = {0, {{1,0},{2,8},{1,0}}, 0, 3, 2};
   bookcase tmp4 = {0, {{1,1,2,3,0,0,0,0,0},{2,8,5,5,5,5,5,5,5},{1,1,2,3,0,0,0,0,0},{1,1,2,3,0,0,0,0,0},{1,1,2,3,0,0,0,0,0},{2,8,5,5,5,5,5,5,5},{2,8,5,5,5,5,5,5,5},{2,8,5,5,5,5,5,5,5},{1,1,2,3,0,0,0,0,0}},0,9,9};
   bookcase tmp5 = {0, {{1,1,2},{2,8,7},{1,6,6}}, 0, 3, 3};
   bookcase tmp6 = {0, {{1,1,2,3,4},{2,8,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0},{1,1,2,3,4}}, 0, 5, 5};
   bookcase tmp7 = {0, {{1,1,0,0},{2,8,0,0},{0,0,0,0}}, 0, 3, 4};
   bookcase tmp8 = {0, {{1,1,1,1},{2,2,2,0},{0,0,0,0}}, 0, 3, 4};
   bookcase tmp9 = {0, {{1,1,1},{0,0,0},{2,0,0}}, 0, 3, 3};
   /*solution len 3*/
   bookcase tmp10 = {0, {{r,g,e},{r,e,e},{g,e,e}}, FSTPARID, 3, 3};
   bookcase tmp11 = {1, {{r,e,e},{r,e,e},{g,g,e}}, 0, 3, 3};
   bookcase tmp12 = {2, {{r,r,e},{e,e,e},{g,g,e}}, 1, 3, 3};
   /*solution len 2*/
   bookcase tmp13 = {0, {{w,g,e,e,e},{r,e,e,e,e},{g,e,e,e,e}}, FSTPARID, 5, 5};
   bookcase tmp14 = {1, {{w,e,e,e,e},{r,e,e,e,e},{g,g,e,e,e}}, 0, 5, 5};
   /*sol len 1*/
   bookcase tmp15 = {0, {{r,e,e},{g,g,e}}, FSTPARID, 2, 3};
   /*sol len 5, ignore arrays*/
   bookcase tmp16 = {0, {{w,g},{r,e,}}, FSTPARID, 2, 2};
   bookcase tmp17 = {2, {{w,e},{r,e}}, 0, 2, 2};
   bookcase tmp18 = {4, {{w,g},{r,e}}, 2, 2, 2};
   bookcase tmp19 = {8, {{w,e},{r,e}}, 4, 2, 2};
   bookcase tmp20 = {16, {{w,g},{r,e}}, 8, 2, 2};
   /*for checking generateChilds*/
   bookcase testParent1 = {2, {{w,r},{e,e}}, 0, 2, 2};
   bookcase testFirstChild = {2, {{w,e},{r,e}}, 0, 2, 2};
   bookcase testParent2 = {2, {{w,g,e,e},{r,r,r,r},{e,e,e,e}}, 0, 3, 4};
   bookcase testFirstChild2 = {2, {{w,e,e,e},{r,r,r,r},{g,e,e,e}}, 0, 3, 4};
   testBookcase[0] = tmp0;
   testBookcase[1] = tmp1;
   testBookcase[2] = tmp2;
   testBookcase[3] = tmp3;
   testBookcase[4] = tmp4;
   testBookcase[5] = tmp5;
   testBookcase[6] = tmp6;
   testBookcase[7] = tmp7;
   testBookcase[8] = tmp8;
   testBookcase[9] = tmp9;
   /*for solution chain testing*/
   testList = (bookcase*) ncalloc(20, sizeof(bookcase));

   /*******************testing of shelfEmpty*******************/
   for(a = 0; a < MAXLW; a++){
      assert(shelfEmpty(&testBookcase[0], a));
   }
   assert(shelfEmpty(&testBookcase[0], 0));
   assert(shelfEmpty(&testBookcase[6], 2));
   assert(shelfEmpty(&testBookcase[6], 3));
   assert(!shelfEmpty(&testBookcase[6], 0));
   assert(!shelfEmpty(&testBookcase[6], 1));
   assert(!shelfEmpty(&testBookcase[6], 4));
   /*******************testing of shelfHasEmpty*******************/
   assert(shelfHasEmpty(&testBookcase[1], 0));
   assert(shelfHasEmpty(&testBookcase[1], 1));
   assert(shelfHasEmpty(&testBookcase[1], 2));
   assert(!shelfHasEmpty(&testBookcase[5], 0));
   assert(!shelfHasEmpty(&testBookcase[5], 1));
   assert(!shelfHasEmpty(&testBookcase[5], 2));
   /*******************testing shelfFull*******************/
   assert(shelfFull(&testBookcase[5], 0));
   assert(shelfFull(&testBookcase[5], 1));
   assert(shelfFull(&testBookcase[5], 2));
   assert(shelfFull(&testBookcase[6], 0));
   assert(!shelfFull(&testBookcase[6], 2));
   assert(shelfFull(&testBookcase[3], 1));
   assert(!shelfFull(&testBookcase[6], 3));
   /*******************testing rightestBook*******************/
   assert(rightestBook(&testBookcase[1], 0) == 3);
   assert(rightestBook(&testBookcase[1], 1) == 1);
   assert(rightestBook(&testBookcase[1], 2) == 0);
   assert(rightestBook(&testBookcase[9], 0) == 2);
   assert(rightestBook(&testBookcase[4], 1) == 8);
   /*******************testing of copyShelf*******************/
   copyShelf(&testBookcase[10], &testBookcase[1]);
   for(a = 0; a < testBookcase[10].row; a++){
      for(b = 0; b < testBookcase[10].col; b++){
         assert(testBookcase[10].book[a][b] == testBookcase[1].book[a][b]);
      }
   }
   /*******************testing of checkMoves*******************/
   assert(checkMoves(&testBookcase[1]) == 6);
   assert(checkMoves(&testBookcase[2]) == 4);
   assert(checkMoves(&testBookcase[3]) == 4);
   assert(checkMoves(&testBookcase[4]) == 40);
   assert(checkMoves(&testBookcase[5]) == 0);
   assert(checkMoves(&testBookcase[6]) == 8);
   assert(checkMoves(&testBookcase[7]) == 4);
   assert(checkMoves(&testBookcase[0]) == 0);
   /*******************testing of findEmpty*******************/
   assert(findEmpty(&testBookcase[0], 0) == -999999);
   assert(findEmpty(&testBookcase[1], 0) == 4);
   assert(findEmpty(&testBookcase[2], 1) == 2);
   assert(findEmpty(&testBookcase[3], 2) == 1);
   assert(findEmpty(&testBookcase[4], 4) == 4);
   assert(findEmpty(&testBookcase[5], 0) == ERRORHAP);
   assert(findEmpty(&testBookcase[6], 2) == 0);
   assert(findEmpty(&testBookcase[7], 0) == 2);
   /*******************testing of moveBook*******************/
   assert(moveBook(&testBookcase[1],0,0,1,2));
   assert(testBookcase[1].book[0][0] == e && testBookcase[1].book[1][2] != e);
   /*moves book back*/
   moveBook(&testBookcase[1],1,2,0,0);
   assert(moveBook(&testBookcase[1],0,0,1,findEmpty(&testBookcase[1],1)));
   assert(testBookcase[1].book[0][0] == e && testBookcase[1].book[1][2] != e);
   /*move the rightest book of row 0 to row 2 first empty*/
   assert(moveBook(&testBookcase[4],0,rightestBook(&testBookcase[4],0),2,findEmpty(&testBookcase[4],2)));
   assert(testBookcase[4].book[2][4] != e && testBookcase[4].book[0][3] == e);
   assert(!moveBook(&testBookcase[7],2,0,0,3));
   /*******************reset structures*******************/
   testBookcase[1] = tmp1;
   testBookcase[2] = tmp2;
   testBookcase[3] = tmp3;
   testBookcase[4] = tmp4;
   testBookcase[5] = tmp5;
   testBookcase[6] = tmp6;
   testBookcase[7] = tmp7;
   testBookcase[8] = tmp8;
   testBookcase[9] = tmp9;
   /*******************testing of generateChilds*******************/
   generateChilds(&testParent1, 1);
   assert(testCmpArray(&testParent1, &testFirstChild));
   while(!generateChilds(&testParent2, 1));
   assert(testCmpArray(&testParent2, &testFirstChild2));
   testParent2 = tmp14;
   /*testing bool functionality of function, when last board is made returns false*/
   while(generateChilds(&testParent2, 6)){
      movesMade++;
   }
   assert(movesMade == 5);
   /*******************testing of keepInbounds*******************/
   assert(keepInbounds(1,3)==1);
   assert(keepInbounds(2,3)==2);
   assert(keepInbounds(0,3)==0);
   assert(keepInbounds(3,3)==0);
   assert(keepInbounds(4,3)==1);
   assert(keepInbounds(5,3)==2);
   assert(keepInbounds(6,3)==0);
   assert(keepInbounds(1,9)==1);
   assert(keepInbounds(1,5)==1);
   assert(keepInbounds(1,2)==1);
   assert(keepInbounds(1,7)==1);
   assert(keepInbounds(1,100)==1);
   assert(keepInbounds(100,5)==0);
   assert(keepInbounds(101,5)==1);
   assert(keepInbounds(102,5)==2);
   assert(keepInbounds(103,5)==3);
   assert(keepInbounds(104,5)==4);
   assert(keepInbounds(105,5)==0);
   /*******************testing of checkHappyShelf*******************/
   assert(checkHappyShelf(&testBookcase[9], 0));
   assert(checkHappyShelf(&testBookcase[9], 1));
   assert(checkHappyShelf(&testBookcase[9], 2));
   assert(checkHappyShelf(&testBookcase[1], 2));
   assert(checkHappyShelf(&testBookcase[2], 0));
   assert(checkHappyShelf(&testBookcase[7], 0));
   assert(checkHappyShelf(&testBookcase[3], 0));
   assert(checkHappyShelf(&testBookcase[0], 0));
   assert(checkHappyShelf(&testBookcase[7], 2));
   assert(!checkHappyShelf(&testBookcase[2], 1));
   assert(!checkHappyShelf(&testBookcase[1], 1));
   /*******************testing of checkColumnZero*******************/
   assert(checkColumnZero(&testBookcase[9]));
   assert(!checkColumnZero(&testBookcase[1]));
   assert(!checkColumnZero(&testBookcase[2]));
   assert(!checkColumnZero(&testBookcase[3]));
   assert(!checkColumnZero(&testBookcase[4]));
   assert(checkColumnZero(&testBookcase[7]));
   assert(checkColumnZero(&testBookcase[8]));
   /*******************testing of caseHappy*******************/
   assert(caseHappy(&testBookcase[9]));
   assert(caseHappy(&testBookcase[8]));
   assert(!caseHappy(&testBookcase[3]));
   assert(!caseHappy(&testBookcase[7]));
   assert(!caseHappy(&testBookcase[2]));
   /*******************testing of boardValid*******************/
   assert(!boardValid(&testBookcase[5]));
   assert(!boardValid(&testBookcase[4]));
   assert(boardValid(&testBookcase[9]));
   assert(boardValid(&testBookcase[6]));
   assert(boardValid(&testBookcase[3]));
   assert(boardValid(&testBookcase[2]));
   assert(boardValid(&testBookcase[1]));
   /*******************testing of calculateLength*******************/
   testList[0] = tmp16;
   testList[2] = tmp17;
   testList[4] = tmp18;
   testList[8] = tmp19;
   testList[16] = tmp20;
   assert(calculateLength(&testList[16], testList) == 5);
   testList[0] = tmp10;
   testList[1] = tmp11;
   testList[2] = tmp12;
   assert(calculateLength(&testList[2], testList) == 3);
   testList[0] = tmp13;
   testList[1] = tmp14;
   assert(calculateLength(&testList[1], testList) == 2);
   testList[0] = tmp15;
   assert(calculateLength(&testList[0], testList) == 1);
   /*******************testing of bcstateFntCnvtr*******************/
   assert(bcstateFntCnvtr(e) == '.');
   assert(bcstateFntCnvtr(k) == 'K');
   assert(bcstateFntCnvtr(r) == 'R');
   assert(bcstateFntCnvtr(g) == 'G');
   assert(bcstateFntCnvtr(b) != 'Y');
   assert(bcstateFntCnvtr(m) != 'T');
   assert(bcstateFntCnvtr(w) != '!');
   /*******************testing of scanToBcstate*******************/
   assert(scanToBcstate('.') == e);
   assert(scanToBcstate('G') == g);
   assert(scanToBcstate('M') == m);
   assert(scanToBcstate('m') == m);
   assert(scanToBcstate('w') == w);
   /*assert(scanToBcstate('T') == e);*/ /*will cause exit(failure)*/
   /*******************testing of scanBCFile, used your files, will leave comment out but if
   you do have all the files you gave us accessible you can uncomment*******************/
   savedBookcase = tmp0;
   assert(scanBCFile(&savedBookcase, "ccbb-23.bc") == 3);
   savedBookcase = tmp0;
   assert(scanBCFile(&savedBookcase, "mwwbkwmkb-548.bc") == 6);
   savedBookcase = tmp0;
   assert(scanBCFile(&savedBookcase, "rgb-321.bc") == 4);
   savedBookcase = tmp0;
   assert(scanBCFile(&savedBookcase, "rggrkkkk-446.bc") == 5);
   savedBookcase = tmp0;
   /*******************testing of scanToBcstate*******************/
   zeroArr(word);
   for(a = 0; a < (int)strlen(word); a++){
      assert(word[a] == 0);
   }

   free(testList);
}

void* ncalloc(int n, size_t size)
{
   void* v = calloc(n, size);
   if(v == NULL){
      fprintf(stderr, "cannot calloc space\n");
   }
   return v;
}
int findParent(bookcase* bcp)
{
   return bcp->parentID;
}
bool shelfEmpty(bookcase* bcp, int rowNum)
{
   int a;
   for(a = 0; a < bcp->col; a++){
      if(bcp->book[rowNum][a] != e){
         return false;
      }
   }
   return true;
}
bool shelfHasEmpty(bookcase* bcp, int rowNum)
{
   int a;
   for(a = 0; a < bcp->col; a++){
      if(bcp->book[rowNum][a] == e){
         return true;
      }
   }
   return false;
}
bool shelfFull(bookcase* bcp, int rowNum)
{
   int a;
   for(a = 0; a < bcp->col; a++){
      if(bcp->book[rowNum][a] == e){
         return false;
      }
   }
   return true;
}
int rightestBook(bookcase* bcp, int rowNum)
{
   int a;
   if(shelfFull(bcp, rowNum)){
      return (bcp->col - 1);
   }
   for(a = 0; a < bcp->col; a++){
      if(bcp->book[rowNum][a + 1] == e){
         return a;
      }
   }
   /*something wrong has happened if it gets to here*/
   return ERRORHAP;
}
void copyShelf(bookcase* dest, bookcase* src)
{
   int a, b;
   /*original Parent will have ID 0, id set in main*/
   for(a = 0; a < src->row; a++){
      for(b = 0; b < src->col; b++){
         dest->book[a][b] = src->book[a][b];
      }
   }
   dest->row = src->row;
   dest->col = src->col;
   dest->parentID = src->id;
}
int checkMoves(bookcase* bcp)
{
   int a;
   int rowsMoveTo = 0;
   int rowsMoveOut = 0;
   int rowsFull = 0;
   for(a = 0; a < bcp->row; a++){
      if(!shelfEmpty(bcp, a)){
         rowsMoveOut++;
      }
      if(!shelfFull(bcp, a)){
         rowsMoveTo++;
      }
      if(shelfFull(bcp, a)){
         rowsFull++;
      }
   }
   return ((rowsMoveTo * rowsMoveOut) - rowsMoveOut) + rowsFull;
}
int findEmpty(bookcase* bcp, int rowNum)
{
   int i;
   for(i = 0; i < bcp->col; i++){
      if(bcp->book[rowNum][i] == e){
         return i;
      }
   }
   return ERRORHAP;
}
bool moveBook(bookcase* bcp, int bookRow, int bookCol, int newRow, int newCol)
{
   bcstate temp;
   if(bcp->row == 0 || bcp->col == 0 || bcp->book[bookRow][bookCol] == e ||
      bcp->book[newRow][newCol] != e){
      return false;
   }
   temp = bcp->book[bookRow][bookCol];
   bcp->book[bookRow][bookCol] = e;
   bcp->book[newRow][newCol] = temp;
   return true;
}
int keepInbounds(int inbounds, int max)
{
   if(inbounds >= max){
      return (inbounds % max);
   }
   return inbounds;
}
/*condition checks for illegal/invalid boards should be done before calling this*/
bool generateChilds(bookcase* bcp, int moves)
{
   static int boardsMade = 0;
   static int currentRow = 0;
   static int nextRowCount = 0;
   int timeOut = 0;
   int emptyPos;
   int rightBook;
   int nextRow = currentRow + 1 + nextRowCount;

   while(timeOut < TMEOUT){
      /*reset all counters when all boards generated, and signal to main that it should
      now generate children of the next board*/
      if(boardsMade >= moves){
         boardsMade = currentRow = nextRowCount = 0;
         return false;
      }
      /*wraps the row so we when current row is the last row we go up to the first row*/
      nextRow = keepInbounds(nextRow, bcp->row);
      if(nextRowCount >= bcp->row MXINDX){
         currentRow++;
         nextRowCount = 0;
         nextRow = currentRow + 1 + nextRowCount;
         nextRow = keepInbounds(nextRow, bcp->row);
      }
      /*if row is not empty and row below if it has an empty*/
      if(!shelfEmpty(bcp, currentRow) && shelfHasEmpty(bcp, nextRow)){
         /*find where the first empty is on the next row and rightest book on current row*/
         rightBook = rightestBook(bcp, currentRow);
         emptyPos = findEmpty(bcp, nextRow);
         if(moveBook(bcp, currentRow, rightBook, nextRow, emptyPos)){
            boardsMade++;
            nextRowCount++;
            return true;
         }
      }
      /*if current row is empty no moves to make so move to next row*/
      else{
         nextRow++;
         nextRowCount++;
      }
      /*prevent infinite loop if it gets stuck for some reason*/
      timeOut++;
   }
   fprintf(stderr, "error in generatingChild\n");
   exit(EXIT_FAILURE);
   return false;
}
bool checkHappyShelf(bookcase* bcp, int rowNum)
{
   int c;
   bcstate check = bcp->book[rowNum][FIRSTBK];
   /*checks the first book which is redundant but mean pretty for loop...*/
   for(c = 0; c < bcp->col; c++){
      if(bcp->book[rowNum][c] != check && bcp->book[rowNum][c] != e){
         return false;
      }
   }
   return true;
}
bool checkColumnZero(bookcase* bcp)
{
   int c = 0;
   int a, b;
   for(a = 0; a < bcp->row - 1; a++){
      for(b = a + 1; b < bcp->row; b++){
         if(bcp->book[a][c] != e){
            if(bcp->book[a][c] == bcp->book[b][c]){
               return false;
            }
         }
      }
   }
   return true;
}
bool caseHappy(bookcase* bcp)
{
   int r;
   for(r = 0; r < bcp->row; r++){
      if(!checkHappyShelf(bcp, r)){
         return false;
      }
   }
   return checkColumnZero(bcp);
}
bool boardValid(bookcase* bcp)
{
   int r, c;
   int full = 0;
   int empty = 0;
   int colourCnt[MAXLW] = {0};
   /*check not completely full*/
   for(r = 0; r < bcp->row; r++){
      if(shelfFull(bcp, r)){
         full++;
      }
   }
   if(full >= bcp->row){
      return false;
   }
   /*check not completely empty*/
   for(r = 0; r < bcp->row; r++){
      if(shelfEmpty(bcp, r)){
         empty++;
      }
   }
   if(empty >= bcp->row){
      return false;
   }
   /*check there are the right amount of colours for the rows/cols*/
   for(r = 0; r < bcp->row; r++){
      for(c = 0; c < bcp->col; c++){
         colourCnt[bcp->book[r][c]]++;
      }
   }
   /*empty is value 0 so colourCnt[0] will be the amount of empties which
   does not matter, so skip*/
   for(r = EXCEMPY; r < MAXLW; r++){
      if(colourCnt[r] > bcp->col){
         return false;
      }
   }
   return true;
}
int calculateLength(bookcase* solution, bookcase list[PARCILDMX])
{
   /*count starts at one cause of the solution fed in*/
   int count = 0;
   /*dont want a to be 0 otherwise while fails straight away*/
   int a = MAXLW;
   /*if solution is original parent*/
   if (solution->id == ORIGPAR) {
      return ORIGCNT;
   }
   /*count from solution up to parent*/
   while(a != ORIGPAR){
      a = solution->parentID;
      count++;
      solution = &list[a];
   }
   /*then add on the orginal parent*/
   count++;
   return count;
}
void printBookcase(bookcase* bcp)
{
   int r, c;
   for(r = 0; r < bcp->row; r++){
      for(c = 0; c < bcp->col; c++){
         printf("%c", bcstateFntCnvtr(bcp->book[r][c]));
      }
      printf("\n");
   }
   printf("\n");
}
char bcstateFntCnvtr(bcstate letter)
{
   switch(letter){
      case e:
         return '.';
      case k:
         return 'K';
      case r:
         return 'R';
      case g:
         return 'G';
      case y:
         return 'Y';
      case b:
         return 'B';
      case m:
         return 'M';
      case c:
         return 'C';
      case w:
         return 'W';
      default:
         fprintf(stderr, "error in state 2 letter conversion\n");
         exit(EXIT_FAILURE);
         return '!';
   }
}
void verbose(bookcase* solution, bookcase list[PARCILDMX])
{
   printBookcase(solution);
   while(solution->id != 0){
      solution = &list[solution->parentID];
      printBookcase(solution);
   }
}
bcstate scanToBcstate(char letter)
{
   letter = toupper(letter);
   switch(letter){
      case '.':
         return e;
      case 'K':
         return k;
      case 'R':
         return r;
      case 'G':
         return g;
      case 'Y':
         return y;
      case 'B':
         return b;
      case 'M':
         return m;
      case 'C':
         return c;
      case 'W':
         return w;
      default:
         printf("%c\n", letter);
         fprintf(stderr, "error in letter 2 state conversion\n");
         exit(EXIT_FAILURE);
         return e;
   }
}
int scanBCFile(bookcase* bcp, char name[MXNAME])
{
   FILE *fp;
   bool exitFlag = false;
   char line[MAXLW] = {0};
   unsigned char r = 0, c;
   short lineCount = 0;
   int l, w;
   /*open relevant file...*/
   if((fp = fopen(name, "r")) == NULL){
      fprintf(stderr, "not a valid filename\n");
      exit(EXIT_FAILURE);
   }
   /*save one line into a string*/
   do {
      if(fgets(line, MAXLW, fp) != NULL){
         /*first line is dimensions*/
         if(lineCount == 0){
            if((int)strlen(line) < LW){
               fprintf(stderr, "not a valid file\n");
               exit(EXIT_FAILURE);
            }
            sscanf(line, "%d %d", &l, &w);
            bcp->row = l;
            bcp->col = w;
            zeroArr(line);
            /*dimensions check*/
            if(l > MAXLW || w > MAXLW){
               fprintf(stderr, "dimensions not valid\n");
               exit(EXIT_FAILURE);
            }
         }
         /*rest of lines are co-ods*/
         else{
            /*check columns match dimensions*/
            if((int)strlen(line) - NWLNE != w){
               fprintf(stderr, "dimensions do not match given bc\n");
               exit(EXIT_FAILURE);
            }
            for(c = 0; c < bcp->col; c++){
               bcp->book[r][c] = scanToBcstate(line[c]);
            }
            r++;
         }
         lineCount++;
      }
      else{
         /*check scanned lines against given row, -1 takes out top line*/
         if(lineCount - TITLE != l){
            fprintf(stderr, "dimensions do not match given bc\n");
            exit(EXIT_FAILURE);
         }
         exitFlag = true;
      }
   } while(!exitFlag);
   fclose(fp);
   /*return lineCount, really only for testing the right amount of lines*/
   return lineCount;
}
void zeroArr(char arr[MAXLW])
{
   int i;
   for(i = 0; i < MAXLW; i++){
      arr[i] = 0;
   }
}
bool testCmpArray(bookcase* bcp1, bookcase* bcp2)
{
   int a, b;
   for(a = 0; a < bcp1->row; a++){
      for(b = 0; b < bcp1->col; b++){
         if(bcp1->book[a][b] != bcp2->book[a][b]){
            return false;
         }
      }
   }
   return true;
}
