#include <stdio.h>
#include <stdlib.h>

#include "btree.h"

void callback(void *valor, void *user_data) {
  printf("%.0f ", * (double *) valor);
}

void inserirNum(BTree_t tree, char *num) {
  double *pNum = malloc(sizeof(double));
  *pNum = strtod(num, NULL);
  bt_insert(tree, num, (void *) pNum);
}

void removeTeste(BTree_t tree, char *num) {
  bt_remove(tree, num);
  printf("Traversing tree remove \"%s\":\n", num);
  bt_traverse(tree, callback, NULL);
  printf("\n");
}

int main(int argc, char *argv[]) {

  BTree_t tree = bt_create(3);

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

  removeTeste(tree, "6");
  removeTeste(tree, "13");
  removeTeste(tree, "7");
  removeTeste(tree, "4");
  removeTeste(tree, "2");
  removeTeste(tree, "16");

  bt_destroy(tree, free);

  return 0;
}