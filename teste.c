#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "btree.h"

int compare_letra(void *_this, void *_other){
  char *this = _this;
  char *other = _other;

  if(strcmp(this, other) > 0)
    return 1;

  if(this[0] == other[0])
    return 0;

  // if(strcmp(this, other) < 0)
  return -1;
}

struct Letra{
  char letr;
  int num;
};

struct Letra *create(char letr, int num){
  struct Letra *this = malloc(sizeof(struct Letra));
  this ->letr = letr;
  this->num = num;
  return this;
}
typedef struct Letra *Letra;

int main(){

  BTree_t tree = bt_create("./Teste-arvore", sizeof(struct Letra), compare_letra);
  int i = 1;

  printf("Inserindo l%d\n", i++);
  Letra l1 = create('a', 1);
  bt_insert(tree, &l1->letr, l1);

  printf("Inserindo l%d\n", i++);
  Letra l2 = create('b', 1);
  bt_insert(tree, &l2->letr, l2);

  printf("Inserindo l%d\n", i++);
  Letra l3 = create('c', 3);
  bt_insert(tree, &l3->letr, l3);

  printf("Inserindo l%d\n", i++);
  Letra l4 = create('d', 4);
  bt_insert(tree, &l4->letr, l4);

  printf("Inserindo l%d\n", i++);
  Letra l5 = create('e', 5);
  bt_insert(tree, &l5->letr, l5);

  printf("Inserindo l%d\n", i++);
  Letra l6 = create('f', 6);
  bt_insert(tree, &l6->letr, l6);

  char chave = 'e';

  Letra lteste = bt_search(tree, &chave);
  if(lteste == NULL){
    printf("Não achou\n");
    return 0;
  }

  printf("%d, %c\n", lteste->num, lteste->letr);

  return 0;
}