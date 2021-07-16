#include "spellcheck.h"

 /*scan a word from a file and load it in to an alphabeticalised list*/
int main(int argc, char *argv[])
{
   Coll* linkedWordList = collIntialise();
   Dataframe* f;
   char word[MAXLETTERS] = {0};
   int i = 0;
   int count = 0;

   /*check only one cmd line argument has been given*/
   assert(argc == 3);

   while(i < MAXWORDS){
      scanWord(argv[1], word);
      addWordLL(linkedWordList, word);
      i++;
   }
   
   f = linkedWordList->start;
   do{
      printf("%3d %s\n", count++, f->word);
      f = f->next;
   } while(f != NULL);


   collFree(linkedWordList);
   return 1;
}
/*for testing....*/
void test(void)
{

}
/*linked list add word*/
void addWordLL(Coll* c, char word[MAXLETTERS])
{
   Dataframe* f;
   Dataframe* t;
   Dataframe* temp;
   if(c){
      f = (Dataframe*) ncalloc(sizeof(Dataframe), 1);
      strcpy(f->word, word);
      /*for first word added*/
      if(c->size == 0){
         c->start = f;
         c->size++;
      }
      else{
         t = c->start;
         /*if new word is smaller*/
         if(strcmp(word, t->word) < 0){
           f->next = c->start;
           c->start = f;
           c->size++;
         }
         else{
            while(t->next != NULL && strcmp(word, t->next->word) > 0){
               t = t->next;
            }
            /*add new word onto end of list*/
            if(t == NULL){
              t = f;
              c->size++;
            }
            /*if words exist that are bigger than the new word, insert word in correct place*/
            else{
              temp = t->next;
              t->next = f;
              f->next = temp;
            }
         }
      }
   }
}

void collFree(Coll* c)
{
   if(c){
      Dataframe* tmp;
      Dataframe* p = c->start;
      while(p != NULL){
         tmp = p->next;
         free(p);
         p = tmp;
      }
      free(c);
   }
}

Coll* collIntialise(void)
{
   Coll* c = (Coll*) ncalloc(sizeof(Coll), 1);
   return c;
}

int collSize(Coll* c)
{
   if(c == NULL){
      return 0;
   }
   return c -> size;
}

bool findWord(Coll* c, char word[MAXLETTERS])
{
   Dataframe* f;
   if(c == NULL || c->start == NULL){
      return false;
   }
   f = c->start;
   do{
      if(!strcmp(f->word, word)){
         return true;
      }
      f = f->next;
   }
   while(f != NULL);
   return false;
}

/*scan one word from a file*/
void scanWord(const char *fileName, char word[MAXLETTERS])
{
   static int lineCount = 0;
   FILE* fp;
   int i;
   char temp[MAXLETTERS] = {0};

   /*open file*/
   if((fp = fopen(fileName, "r")) == NULL){
      printf("not scanning \n");
      fprintf(stderr, "could not open file %s\n", fileName);
      exit(EXIT_FAILURE);
   }
   /*words to ignore*/
   for(i = 0; i < lineCount; i++){
      fgets(temp, MAXLETTERS, fp);
   }
   /*word to scan*/
   if(fgets(temp, MAXLETTERS, fp) != NULL){
      sscanf(temp, "%s", word);
      lineCount++;
   }
   /*scan successful so close file*/
   fclose(fp);
}

void* ncalloc(int n, size_t size)
{
   void* v = calloc(n, size);
   if(v == NULL){
      fprintf(stderr, "cannot calloc space\n");
   }
   return v;
}
