#define FORMATSTR "%d"
#define WORD 1000

#define FIXEDSIZE 16
#define SCALEFACTOR 2
#define BITSPERCHAR 8
#define LSB 1
#define MSB 128

typedef unsigned char datatype;
typedef enum bool {false, true} bool;
typedef struct boolarr{
   datatype* arr;
   int capacity;
   int numbits;
} boolarr;
