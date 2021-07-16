#include "bookcase.h"

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

   while(timeOut < 100){
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
         fprintf(stderr, "error in state 2 letter conversion");
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
            if((int)strlen(line) < LW || (int)strlen(line) > LWH){
               fprintf(stderr, "not a valid file\n");
               exit(EXIT_FAILURE);
            }
            sscanf(line, "%d %d", &l, &w);
            bcp->row = l;
            bcp->col = w;
            /*prevent against bad files*/
            zeroArr(line);
         }
         /*rest of lines are co-ods*/
         else{
            for(c = 0; c < bcp->col; c++){
               bcp->book[r][c] = scanToBcstate(line[c]);
            }
            r++;
         }
         lineCount++;
      }
      else{
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
