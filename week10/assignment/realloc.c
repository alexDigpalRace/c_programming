#include "assoc.h"
#include <limits.h>

/*returns a 'unique' index for use with hashtable*/
unsigned int _hash1(assoc* ap, void* key);
/*creates space for a dataframe and intialises where key and data point to*/
dataframe* _dfInit();
/*frees dataframe*/
void _dfFree(dataframe* dfp);
/*returns the next closest prime to the inputted number*/
unsigned int _findNextPrime(unsigned int n);
/*returns a pointer to a new bigger structure with all the old dataframes rehashed*/
assoc* _resize(assoc** app);
/*checks if inputted keys are the same*/
bool _isDup(void* storedKey, void* passedKey, int keysize);
/*returns a probe length,*/
unsigned int _hash2(assoc* ap, int hash1);
/*keeps data inbounds*/
unsigned int _inbounds(unsigned int index, unsigned int size);

/*originally had done linear probing which gets the string processing part of the program
  done near instantly, but the numbers were taking longer so i tried double hashing,
  it seems the way ive implemented it actually made it slower then just probing linearly,
  so I am defaulting back to linear probing but will leave my hash2 func in so you can,
  see maybe where i went wrong... thanks!*/

/*my understanding of why we needed to assoc** was wrong for 90% of this project,
  only after i had got most of the program coded did i realise, why it was useful,
  i think this might be why its a bit slow (~50s on my sys)*/

assoc* assoc_init(int keysize)
{
   assoc* ap = (assoc*)ncalloc(SPACENEEDED, sizeof(assoc));
   ap->table = (dataframe**)ncalloc(FIXEDSIZE, sizeof(dataframe*));
   /*ap->currSize auto set to 0*/
   ap->capacity = FIXEDSIZE;
   ap->keysize = keysize;
   return ap;
}
void assoc_insert(assoc** a, void* key, void* data)
{
   bool duplicate = false;
   unsigned int index, hash;
   assoc* newap;
   dataframe* dfp;
   if(a == NULL || (*a) == NULL){
      fprintf(stderr, "no assoc structure exists/being pointed to\n");
      exit(EXIT_FAILURE);
   }
   /*resize when 2/3 full*/
   if((*a)->currSize * MULTIPLIER1 >= (*a)->capacity * MULTIPLIER2){
      newap = _resize(a);
      *a = newap;
   }
   /*generate hashcode from key*/
   hash = _hash1(*a, key);
   index = (hash % (*a)->capacity);
   /*intialise space for a dataframe and point it to key data pair*/
   dfp = _dfInit(key, data);
   /*check table at hash index is empty, if it is point it to dataframe*/
   if((*a)->table[index] == NULL){
      (*a)->table[index] = dfp;
      (*a)->currSize++;
   }
   /*COLLISON search for any empty whilst checking for duplicate entries*/
   else{
      while((*a)->table[index] != NULL && duplicate == false){
         if(_isDup((*a)->table[index]->key, key, (*a)->keysize)){
            duplicate = true;
            _dfFree(dfp);
         }
         /*linear probe*/
         index++;
         index = _inbounds(index, (*a)->capacity);
      }
      if(duplicate == false){
         (*a)->table[index] = dfp;
         (*a)->currSize++;
      }
   }
   return;
}
unsigned int assoc_count(assoc* a)
{
   if(a == NULL){
      return 0;
   }
   return a->currSize;
}
void* assoc_lookup(assoc* a, void* key)
{
   bool emptyFound = false;
   unsigned int index, hash;
   if(a == NULL){
      fprintf(stderr, "no assoc structure to lookup\n");
      exit(EXIT_FAILURE);
   }
   hash = _hash1(a, key);
   index = (hash % a->capacity);
   do{
      /*check table position points to a dataframe*/
      if(a->table[index] != NULL){
         if(_isDup(a->table[index]->key, key, a->keysize)){
            return a->table[index]->data;
         }
      }
      else{
         emptyFound = true;
      }
      index++;
      index = _inbounds(index, a->capacity);
   }
   while(emptyFound == false);
   return NULL;
}
void assoc_free(assoc* a)
{
   unsigned int i;
   if(a == NULL){
      return;
   }
   for(i = 0; i < a->capacity; i++){
      if(a->table[i] != NULL){
         _dfFree(a->table[i]);
      }
   }
   free(a->table);
   free(a);
   return;
}
/*magic numbers*/
/*http://www.cse.yorku.ca/~oz/hash.html slightly edited*/
unsigned int _hash1(assoc* ap, void* key)
{
   int c, count = ap->keysize;
   int i = 0;
   unsigned long hash = BERNSTEINNUM1;
   char *keyStr;
   if(ap == NULL){
      fprintf(stderr, "error in hash1 func, no assoc struct being pointed to, exiting\n");
      exit(EXIT_FAILURE);
   }
   if(ap->capacity == 0){
      fprintf(stderr, "table size can't be zero (error in hash func)\n");
      exit(EXIT_FAILURE);
      return 0;
   }
   if(ap->keysize == STRING){
      count = WORD;
   }
   keyStr = (char*)key;
   while(i < count && (c = keyStr[i])){
      hash = ((hash << BERNSTEINNUM3) + hash) + c;
      i++;
   }
   return (unsigned int)hash;
}
unsigned int _hash2(assoc* ap, int hash1)
{
   int smallPrime;
   if(ap == NULL){
      fprintf(stderr, "no assoc structure linked for generate probe, exiting\n");
      exit(EXIT_FAILURE);
      return 0;
   }
   /*assumming ap capacity already checked for 0 if getting to this stage*/
   /*needed som arbitarily big number to get a small probe size, BERNSTEINNUM1
     seemed appropriate...*/
   smallPrime = _findNextPrime((ap->capacity) / BERNSTEINNUM1);
   return (unsigned int)(smallPrime - (hash1 % smallPrime));
}
dataframe* _dfInit(void* key, void* data)
{
   dataframe* dfp = (dataframe*)ncalloc(SPACENEEDED, sizeof(dataframe));
   dfp->data = data;
   dfp->key = key;
   return dfp;
}
unsigned int _findNextPrime(unsigned int n)
{
   unsigned int i, j;
   bool skip = false;
   if(n < FSTPRIME){
      return FSTPRIME;
   }
   i = n + 1;
   while(TRUE){
      for(j = i-1; j > 1; j--){
         if(i % j == 0){
            skip = true;
         }
      }
      if(!skip){
         return i;
      }
      skip = false;
      i++;
   }
   return 0;
}
assoc* _resize(assoc** app)
{
   unsigned int i;
   assoc* ap = (assoc*)ncalloc(SPACENEEDED, sizeof(assoc));
   /*null check*/
   if(app == NULL || *app == NULL){
      fprintf(stderr, "no assoc structure to resize?\n");
      exit(EXIT_FAILURE);
      return NULL;
   }
   ap->capacity = _findNextPrime((*app)->capacity * SCALE);
   ap->table = (dataframe**)ncalloc(ap->capacity, sizeof(dataframe*));
   ap->keysize = (*app)->keysize;
   /*currSize to stay at 0 as insert will count back up to correct size*/
   /*rehash old data to new ap*/
   for(i = 0; i < (*app)->capacity; i++){
      if((*app)->table[i] != NULL){
         assoc_insert(&ap, (*app)->table[i]->key, (*app)->table[i]->data);
      }
   }
   assoc_free(*app);
   /**app = ap;*/
   return ap;
}
void _dfFree(dataframe* dfp)
{
   free(dfp);
}
bool _isDup(void* storedKey, void* passedKey, int keysize)
{
   if(storedKey == NULL || passedKey == NULL){
      fprintf(stderr, "key pointer is null? exiting...\n");
      exit(EXIT_FAILURE);
   }
   if(keysize == STRING){
      if(strcmp((char*)storedKey, (char*)passedKey) == 0){
         return true;
      }
      else{
         return false;
      }
   }
   else{
      if(memcmp(storedKey, passedKey, keysize) == 0){
         return true;
      }
      else{
         return false;
      }
   }
   fprintf(stderr, "error in duplication check\n");
   exit(EXIT_FAILURE);
   return false;
}
unsigned int _inbounds(unsigned int index, unsigned int size)
{
   if(size == 0){
      fprintf(stderr, "size cannot be zero (inbounds func), exiting...\n");
      exit(EXIT_FAILURE);
      return 0;
   }
   if(index >= size){
      return (index % size);
   }
   return index;
}
/*prototyped in specific.h so should be able to just add in to main somewhere*/
void _reallocTest()
{
   unsigned int a;
   void* analysis;
   assoc* apstr = assoc_init(0);
   assoc* apnum = assoc_init(sizeof(short));
   assoc** appnum = &apnum;
   /*assoc** appstr = NULL;*/
   /*assoc* apnull = NULL;*/
   short data2 = 0xfff; void* vpData2 = &data2;
   char hashtest[WORD] = "hashtest"; void* vpKey = hashtest;
   short hashtest2 = 32001; void* vpKey2 = &hashtest2;
   short key1 = 32700; void* vpkey1 = &key1;
   short key3 = 1; void* vpkey3 = &key3;
   short key4 = 10; void* vpkey4 = &key4;
   short key5 = 100; void* vpkey5 = &key5;
   short key6 = 400; void* vpkey6 = &key6;
   short key7 = 512; void* vpkey7 = &key7;
   short key8 = 789; void* vpkey8 = &key8;
   short key9 = 1234; void* vpkey9 = &key9;
   short key10 = 32700; void* vpkey10 = &key10;
   short key11 = 6009; void* vpkey11 = &key11;
   short key12 = 11000; void* vpkey12 = &key12;
   short key13 = 22700; void* vpkey13 = &key13;
   short key14 = 12700; void* vpkey14 = &key14;
   short key15 = 24700; void* vpkey15 = &key15;
   short key16 = 26700; void* vpkey16 = &key16;
   /*rest of the array that I've not filled will have junk in but not really
   concerned about that*/
   char data1[WORD] = "cheese"; void* vpData1 = data1;
   char data4[WORD] = "planet"; void* vpData4 = data4;
   char data5[WORD] = "gfsdffff"; void* vpData5 = data5;
   char data6[WORD] = "bob"; void* vpData6 = data6;
   char data7[WORD] = "dusseldorf"; void* vpData7 = data7;
   char data8[WORD] = "home"; void* vpData8 = data8;
   char data9[WORD] = "elephant"; void* vpData9 = data9;
   char data10[WORD] = "fleventyfive"; void* vpData10 = data10;
   char data11[WORD] = "cheese"; void* vpData11 = data11;
   char data12[WORD] = "crypto"; void* vpData12 = data12;
   char data13[WORD] = "1234556"; void* vpData13 = data13;
   char data14[WORD] = "noob"; void* vpData14 = data14;
   char data15[WORD] = "boon"; void* vpData15 = data15;
   char data16[WORD] = "0000000"; void* vpData16 = data16;
   char data17[WORD] = "test2"; void* vpData17 = data17;
   dataframe* dfpTest1 = (dataframe*)_dfInit(vpKey, vpData1);
   dataframe* dfpTest2 = (dataframe*)_dfInit(vpKey2, vpData2);
   dataframe* dfpNULL = (dataframe*)_dfInit(NULL,NULL);
   /************************tesing assoc_init************************/
   assert(apstr->currSize == 0);
   assert(apstr->capacity == 17);
   assert(apstr->keysize == 0);
   for(a = 0; a < 17; a++){
      assert(apstr->table[a] == NULL);
   }
   assert(apnum->currSize == 0);
   assert(apnum->capacity == 17);
   assert(apnum->keysize == sizeof(short));
   for(a = 0; a < 17; a++){
      assert(apnum->table[a] == NULL);
   }
   /************************tesing _hash1************************/
   for(a = 1; a < 300000; a+=100){
      apstr->capacity = a;
      assert((_hash1(apstr, vpKey) % apstr->capacity) < a);
      /*printf("hash index: %d / %d\n", _hash1(apstr, vpKey), a);*/
   }
   for(a = 1; a < 300000; a+=55){
      apnum->capacity = a;
      assert((_hash1(apnum, vpKey2) % apnum->capacity) < a);
      /*printf("hash index: %d / %d\n", _hash1(apnum, vpKey), a);*/
   }
   /*Following tests will cause EXIT_FAILURE*/
   /*apstr->capacity = 0;
   assert(_hash1(apstr, vpKey) == 0);*/
   /************************tesing _findNextPrime************************/
   assert(_findNextPrime(0) == 2);
   assert(_findNextPrime(1) == 2);
   assert(_findNextPrime(2) == 3);
   assert(_findNextPrime(34) == 37);
   assert(_findNextPrime(113) == 127);
   assert(_findNextPrime(200000) == 200003);
   /************************tesing _dfInit************************/
   assert(dfpTest1->key == vpKey);
   assert(dfpTest1->data == vpData1);
   assert(dfpTest2->key == vpKey2);
   assert(dfpTest2->data == vpData2);
   assert(dfpNULL->key == NULL && dfpNULL->data == NULL);
   /************************tesing assoc_insert************************/
   apstr->capacity = FIXEDSIZE;
   apnum->capacity = FIXEDSIZE;
   /*insert creates a dataframe which needs freeing*/
   assoc_insert(appnum, vpkey1, vpData1);
   assert(assoc_count(apnum) == 1);
   /*insert duplicate*/
   assoc_insert(appnum, vpkey1, vpData1);
   assert(assoc_count(apnum) == 1);
   /*insert nonconflicting data*/
   assoc_insert(appnum, vpkey3, vpData4);
   assert(assoc_count(apnum) == 2);
   assoc_insert(appnum, vpkey4, vpData5);
   assert(assoc_count(apnum) == 3);
   assoc_insert(appnum, vpkey5, vpData6);
   assert(assoc_count(apnum) == 4);
   assoc_insert(appnum, vpkey6, vpData7);
   assert(assoc_count(apnum) == 5);
   assoc_insert(appnum, vpkey7, vpData8);
   assert(assoc_count(apnum) == 6);
   assoc_insert(appnum, vpkey8, vpData9);
   assert(assoc_count(apnum) == 7);
   assoc_insert(appnum, vpkey9, vpData10);
   assert(assoc_count(apnum) == 8);
   /*another duplicate*/
   assoc_insert(appnum, vpkey10, vpData11);
   assert(assoc_count(apnum) == 8);
   assoc_insert(appnum, vpkey11, vpData12);
   assert(assoc_count(apnum) == 9);
   assoc_insert(appnum, vpkey12, vpData13);
   assert(assoc_count(apnum) == 10);
   assoc_insert(appnum, vpkey13, vpData14);
   assert(assoc_count(apnum) == 11);
   assoc_insert(appnum, vpkey14, vpData15);
   assert(assoc_count(*appnum) == 12);
   assoc_insert(appnum, vpkey15, vpData16);
   assert(assoc_count(*appnum) == 13);
   /*resize should happen*/
   assoc_insert(appnum, vpkey16, vpData17);
   assert(assoc_count(*appnum) == 14);
   assert((*appnum)->capacity == 37); /*double is 34 then closest prime is 37*/
   /************************tesing _resize************************/
   apstr = _resize(&apstr);
   assert(apstr->capacity == 37);
   apstr = _resize(&apstr);
   assert(apstr->capacity == 79);
   /*will cause exit(failure)*/
   /*assert(_resize(appstr) == NULL);*/
   /************************tesing assoc_lookup************************/
   analysis = assoc_lookup(apnum, vpkey1);
   assert(strcmp((char*)analysis, (char*)vpData1) == 0);
   /*will cause exit failure*/
   /*analysis = assoc_lookup(apnum, vpData2);*/
   /*analysis = assoc_lookup(apnull, vpData2);*/
   analysis = assoc_lookup(apnum, vpkey5);
   assert(strcmp((char*)analysis, (char*)vpData6) == 0);
   analysis = assoc_lookup(apnum, vpkey10);
   assert(strcmp((char*)analysis, (char*)vpData11) == 0);
   analysis = assoc_lookup(apnum, vpkey11);
   assert(strcmp((char*)analysis, (char*)vpData12) == 0);

   /*would have done tests for key as short but it'll be tested by your testassoc
   with ints which is/should be enough otherwise this testfunc would be nearly twice
   the size*/

   /************************tesing assoc_lookup************************/
   assert(_inbounds(5, 10) == 5);
   assert(_inbounds(10, 10) == 0);
   assert(_inbounds(20, 5) == 0);
   assert(_inbounds(21, 5) == 1);
   assert(_inbounds(1, 10) == 1);
   assert(_inbounds(2, 10) == 2);
   assert(_inbounds(5, 100) == 5);

   _dfFree(dfpTest1);
   _dfFree(dfpTest2);
   _dfFree(dfpNULL);
   assoc_free(apstr);
   assoc_free(apnum);
   return;
}
