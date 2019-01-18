#include <stdio.h>
#include <stdlib.h>

#include "btree.h"

void callback(void *valor, void *user_data) {
  printf("%.0f ", * (double *) valor);
}

void inserirNum(BTree_t tree, double num) {
  double *pNum = malloc(sizeof(double));
  *pNum = num;
  bt_insert(tree, num, (void *) pNum);
}

int main(int argc, char *argv[]) {

  BTree_t tree = bt_create(3);

  inserirNum(tree, 10.0);
  inserirNum(tree, 20.0);
  inserirNum(tree,  5.0);
  inserirNum(tree,  6.0);
  inserirNum(tree, 12.0);
  inserirNum(tree, 30.0);
  inserirNum(tree,  7.0);
  inserirNum(tree, 17.0);

  printf("Traversing tree: ");
  bt_traverse(tree, callback, NULL);
  printf("\n");

  double k = 6;
  (bt_search(tree, k) != NULL) ? printf("Achou\n") : printf("Nao achou\n");

  k = 15;
  (bt_search(tree, k) != NULL) ? printf("Achou\n") : printf("Nao achou\n");

  bt_destroy(tree, free);

  return 0;
}