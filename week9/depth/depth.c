#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<time.h>
#include<string.h>
#define STRSIZE 5000

typedef struct node{
   char c;
   struct node* left;
   struct node* right;
} Node;
typedef enum bool {false, true} bool;

Node* MakeNode(char c);
void InsertRandom(Node* t, Node* n);
char* PrintTree(Node* t);
void freeTree(Node* t);

int main(void)
{
   char c;
   Node* head = MakeNode('A');
   Node* n;
   char* str;
   /*srand(time(NULL));*/
   for(c = 'B'; c < 'G'; c++){
      n = MakeNode(c);
      InsertRandom(head, n);
   }
   str = PrintTree(head);
   printf("%s\n", str);
   freeTree(head);
   free(str);
   return 0;
}
Node* MakeNode(char c)
{
   Node* n = (Node*)calloc(1, sizeof(Node));
   assert(n != NULL);
   n->c = c;
   return n;
}
void InsertRandom(Node* t, Node* n)
{
   if((rand() % 2) == 0){
      /*Left*/
      if(t->left == NULL){
         t->left = n;
      }
      else{
         InsertRandom(t->left, n);
      }
   }else{
      /*Right*/
      if(t->right == NULL){
         t->right = n;
      }
      else{
         InsertRandom(t->right, n);
      }
   }
}
char* PrintTree(Node* t)
{
   char* str;
   assert((str = calloc(STRSIZE,sizeof(char))) != NULL);
   if(t == NULL){
      strcpy(str, "*");
      return str;
   }
   sprintf(str, "%c (%s) (%s)", t->c, PrintTree(t->left), PrintTree(t->right));
   return str;
}
void freeTree(Node* t)
{
   /*go left and right until bottom found*/
   if(t->left != NULL){
      freeTree(t->left);
      t->left = NULL;
   }
   if(t->right != NULL){
      freeTree(t->right);
      t->right = NULL;
   }
   /*check if its the bottom node*/
   if(t->left == NULL && t->right == NULL){
      free(t);
   }
}
