#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>

#define FILENAME 40
#define MAXWORDS 2000
#define MAXLETTERS 50
#define STARTROW 0
#define STARTSRCHLETR 0

typedef enum bool {false, true} bool;
typedef struct coll Coll;
typedef struct dataframe Dataframe;

void test(void);
void scanWord(const char *fileName, char word[MAXLETTERS]);
void addWordLL(Coll* c, char word[MAXLETTERS]);
Coll* collIntialise(void);
int collSize(Coll* c);
bool findWord(Coll* c, char word[MAXLETTERS]);
void* ncalloc(int n, size_t size);
void collFree(Coll* c);

struct dataframe{
   char word[MAXLETTERS];
   Dataframe* next;
};

struct coll{
   Dataframe* start;
   Dataframe* end;
   int size;
};
