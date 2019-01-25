#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "btree.h"

int compare_item(void *this, void *other) {
  // return strcmp(this, other);
  return strtol(this, NULL, 10) - strtol(other, NULL, 10);
}

void callback(void *valor, void *user_data) {
  printf("%d ", * (int *) valor);
}

void inserirNum(BTree_t tree, char *num) {
  int *pNum = malloc(sizeof(int));
  *pNum = strtol(num, NULL, 10);
  bt_insert(tree, num, (void *) pNum);
}

void removeTeste(BTree_t tree, char *num) {
  bt_remove(tree, num);
  printf("Traversing tree remove \"%s\":\n", num);
  bt_traverse(tree, callback, NULL);
  printf("\n");
}

int main(int argc, char *argv[]) {

  BTree_t tree = bt_create(3, compare_item);

  inserirNum(tree, "1"); 
  inserirNum(tree, "3"); 
  inserirNum(tree, "7"); 
  inserirNum(tree, "10"); 
  inserirNum(tree, "11"); 
  inserirNum(tree, "13"); 
  inserirNum(tree, "14"); 
  inserirNum(tree, "15"); 
  inserirNum(tree, "18"); 
  inserirNum(tree, "16"); 
  inserirNum(tree, "19"); 
  inserirNum(tree, "24"); 
  inserirNum(tree, "25"); 
  inserirNum(tree, "26"); 
  inserirNum(tree, "21"); 
  inserirNum(tree, "4"); 
  inserirNum(tree, "5"); 
  inserirNum(tree, "20"); 
  inserirNum(tree, "22"); 
  inserirNum(tree, "2"); 
  inserirNum(tree, "17"); 
  inserirNum(tree, "12"); 
  inserirNum(tree, "6");
  
  printf("Traversing tree:\n");
  bt_traverse(tree, callback, NULL);
  printf("\n");

  // removeTeste(tree, "6");
  // removeTeste(tree, "13");
  // removeTeste(tree, "7");
  // removeTeste(tree, "4");
  // removeTeste(tree, "2");
  // removeTeste(tree, "16");

  printf("\n\nRange search entre 10 e 20:\n");
  Lista_t intervalo = bt_range_search(tree, "10", "20");

  Posic_t it = lt_get_first(intervalo);
  while (it) {
    int *item = lt_get(intervalo, it);

    printf("%d ", *item);

    it = lt_get_next(intervalo, it);
  }
  lt_destroy(intervalo, NULL);

  printf("\n");

  bt_destroy(tree, free);

  return 0;
}