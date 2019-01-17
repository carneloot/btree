#include <stdio.h>
#include <stdlib.h>

#include "btree.h"

void callback(void *valor, void *user_data) {
  printf("%.0f ", * (double *) valor);
}

void inserirNum(BTree_t tree, int num) {
  double *pNum = malloc(sizeof(*pNum));
  *pNum = num;
  bt_insert(tree, *pNum, pNum);
}

int main(int argc, char *argv[]) {

  BTree_t tree = bt_create(3);

  inserirNum(tree, 10);
  inserirNum(tree, 20);
  inserirNum(tree, 5);
  inserirNum(tree, 6);
  inserirNum(tree, 12);
  inserirNum(tree, 30);
  inserirNum(tree, 7);
  inserirNum(tree, 17);

  printf("Traversing tree: ");
  bt_traverse(tree, callback, NULL);
  printf("\n");

  double k = 6;
  (bt_search(tree, k) != NULL) ? printf("Achou\n") : printf("Nao achou\n");

  k = 15;
  (bt_search(tree, k) != NULL) ? printf("Achou\n") : printf("Nao achou\n");

  return 0;
}