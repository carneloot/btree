#include "btree.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "btreenode.h"

struct BTree_t {
  BTreeNode_t root; // No raiz da arvore
  int grau;         // Grau da arvore
};

/* =========== FUNCOES ARVORE =========== */

BTree_t bt_create(int grau) {
  struct BTree_t *this = calloc(1, sizeof(*this));

  this->root = NULL;
  this->grau = grau;

  return this;  
}

void bt_destroy(BTree_t _this, void (*destruir_item)(void *item)) {
  struct BTree_t * this = (struct BTree_t *) _this;

  // Se a arvore nao ta vazia
  if (this->root != NULL) {

    destroy_node(this->root, destruir_item);

  }
  
  free(this);
}

void bt_traverse(BTree_t _this, void (*callback)(void *item, void *user_data), void *user_data) {
  struct BTree_t * this = (struct BTree_t *) _this;

  if (this->root != NULL) {
    traverse_node(this->root, callback, user_data);
  }
}

void *bt_search(BTree_t _this, char *chave) {
  struct BTree_t * this = (struct BTree_t *) _this;

  if (this->root == NULL)
    return NULL;

  return search_node(this->root, chave);
}

void *bt_insert(BTree_t _this, char *chave, void *valor) {
  struct BTree_t * this = (struct BTree_t *) _this;

  // Se a arvore estiver vazia
  if (this->root == NULL) {
    this->root = create_node(this->grau, true);
    this->root->chaves[0] = malloc(sizeof(*this->root->chaves[0]));
    this->root->chaves[0]->chave = chave;
    this->root->chaves[0]->valor = valor;
    this->root->numero_filhos = 1;
  }

  // Se a arvore nao esta vazia
  else {

    // Se a raiz ta cheia, a arvore cresce em altura
    if (is_full_node(this->root)) {
      
      // Criando espaco pra nova raiz
      BTreeNode_t nova_raiz = create_node(this->grau, false);

      // Fazer a raiz antiga filha da nova
      nova_raiz->filhos[0] = this->root;

      // Splita a raiz antiga
      split_child_node(nova_raiz, 0, this->root);

      // Nova raiz tem dois filhos
      // Escolher para onde inserir
      int i = 0;
      if (nova_raiz->chaves[0]->chave < chave)
        i++;
      insert_non_full_node(nova_raiz->filhos[i], chave, valor);

      // Mudar a raiz
      this->root = nova_raiz;

    }

    // Se a raiz nao estiver cheia, so adicionar usando a inser_non_full_node
    else 
      insert_non_full_node(this->root, chave, valor);
  }
}

void *bt_remove(BTree_t _this, char *chave) {
  struct BTree_t *this = (struct BTree_t *) _this;

  if (this->root == NULL) {
    printf("A arvore esta vazia.\n");
    return NULL;
  }

  void *retorno = remove_node(this->root, chave);

  if (this->root->numero_filhos == 0) {
    BTreeNode_t tmp = this->root;

    if (this->root->folha)
      this->root = NULL;
    else
      this->root = this->root->filhos[0];

    destroy_node(tmp, NULL);
  }
  
  return retorno;
}

bool bt_is_empty(BTree_t _this) {
  struct BTree_t * this = (struct BTree_t *) _this;
  return (this->root == NULL);
}