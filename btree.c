#include "btree.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/** Par chave / item */
struct BTreePair_t {
  char *chave;
  void *valor;
};

typedef struct BTreePair_t *BTreePair_t;

struct BTreeNode_t {
  BTreePair_t *chaves;         // Guarda todas as chaves desse no
  int grau;                    // Grau minimo do no
  struct BTreeNode_t **filhos; // Um vetor de filhos
  int numero_filhos;           // Numero atual de filhos
  bool folha;                  // Determina se o no eh uma folha ou nao
};

typedef struct BTreeNode_t *BTreeNode_t;

struct BTree_t {
  BTreeNode_t root; // No raiz da arvore
  int grau;         // Grau da arvore
};

/* =========== FUNCOES NODE =========== */

static BTreeNode_t __create_node(int grau, bool folha) {
  BTreeNode_t this = (BTreeNode_t) calloc(1, sizeof(*this));

  this->grau  = grau;
  this->folha = folha;

  this->chaves = (BTreePair_t *) calloc(2 * grau - 1, sizeof(*this->chaves));
  this->filhos = (BTreeNode_t *) calloc(2 * grau,     sizeof(*this->filhos));

  this->numero_filhos = 0;

  return this;
}

static void __destroy_node(BTreeNode_t this, void *_destruir_item) {
  void (*destruir_item)(void *item) = (void (*)(void *)) _destruir_item;

  for (int i = 0; i < this->numero_filhos; i++)
    __destroy_node(this->filhos[i], destruir_item);

  free(this->filhos);

  int tam = 2 * this->grau - 1;
  for (int i = 0; i < tam; i++) {
    if (this->chaves == NULL) break;

    free(this->chaves[i]->chave);
    if (destruir_item)
      free(this->chaves[i]->valor);

    free(this->chaves[i]);
  }

  free(this->chaves);

  free(this);
}

static void __traverse_node(BTreeNode_t this, void (*callback)(void *item, void *user_data), void *user_data) {
  int i;

  for (i = 0; i < this->numero_filhos; i++) {
    if (this->folha == false)
      __traverse_node(this->filhos[i], callback, user_data);

    callback(this->chaves[i]->valor, user_data);
  }

  if (this->folha == false)
    __traverse_node(this->filhos[i], callback, user_data);

}

static void *__search_node(BTreeNode_t this, char *chave) {
  int i = 0;

  // Percorre ate achar a chave "maior" que a passada
  while (i < this->numero_filhos && strcmp(this->chaves[i]->chave, chave) > 0)
    i++;

  // Se a chave for igual, retorna o valor atual
  if (strcmp(this->chaves[i]->chave, chave) == 0)
    return this->chaves[i]->valor;
  
  // Se chegou numa folha, quer dizer que nao achou o item
  if (this->folha == true)
    return NULL;

  // Procura no filho
  return __search_node(this->filhos[i], chave);
}

/* =========== FUNCOES ARVORE =========== */

BTree_t bt_create(int grau) {
  struct BTree_t *this = calloc(1, sizeof(*this));

  this->root = NULL;
  this->grau = grau;

  return this;  
}

// TODO: Passar por tudo destruindo
void bt_destroy(BTree_t _this, void (*destruir_item)(void *item)) {
  struct BTree_t * this = (struct BTree_t *) _this;

  if (this->root != NULL) {

  }
  
  free(this);
}

void bt_traverse(BTree_t _this, void (*callback)(void *item, void *user_data), void *user_data) {
  struct BTree_t * this = (struct BTree_t *) _this;

  if (this->root != NULL) {
    __traverse_node(this->root, callback, user_data);
  }
}

void *bt_search(BTree_t _this, char *chave) {
  struct BTree_t * this = (struct BTree_t *) _this;

  if (this->root == NULL)
    return NULL;

  return __search_node(this->root, chave);
}