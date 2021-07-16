#include "bookcase.h"

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
