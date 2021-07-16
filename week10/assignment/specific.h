#define FIXEDSIZE 17
#define WORD 50
#define SCALE 2
#define SPACENEEDED 1
#define BERNSTEINNUM1 5381
#define BERNSTEINNUM2 33
#define BERNSTEINNUM3 5
#define TOBIT 8
#define STRING 0
#define TRUE 1
#define FOUND 0
#define LNRPROBE 3
#define MULTIPLIER1 3
#define MULTIPLIER2 2
#define FSTPRIME 2

typedef enum bool {false, true} bool;

/*strcpy or memcpy into pointers of dataframe*/
/**/
typedef struct dataframe{
   void* data;
   void* key;
} dataframe;

typedef struct assoc{
   dataframe** table;          /*hashtable of dataframe pointers*/
   unsigned int currSize;      /*amount of filled slots*/
   unsigned int capacity;      /*max amount of slots*/
   int keysize;
} assoc;

void _reallocTest();
