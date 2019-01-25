#include "btree.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "btreenode.h"

struct BTree_t {
  // BTreeNode_t root; // No raiz da arvore
  int root;         // Posição da raíz no arquivo
  // int grau;         // Grau da arvore
  int (*compare)(void *this, void *other); // Funcao para comparar as chaves
  Arquivo_bin tree;
  Arquivo_bin itens;
};

/* =========== FUNCOES ARVORE =========== */

BTree_t bt_create(char *path_and_name, unsigned tamanho_do_item, int (*compare)(void *this, void *other)) {
  struct BTree_t *this = calloc(1, sizeof(*this));

  this->root = -1;
  this->compare = compare;
  
  // path_and_name + "_item" + \0
  char nome[strlen(path_and_name) + 5 + 1];
  strcpy(nome, path_and_name);
  strcat(nome, "_item");

  this->itens = bin_create(nome, tamanho_do_item, NULL, 0);

  this->tree = bin_create(path_and_name, sizeof(struct BTreeNode_t), this, sizeof(struct BTree_t));

  return this;  
}

BTree_t bt_start(char *path_and_name){
  struct BTree_t *this = malloc(sizeof(struct BTree_t));

  Arquivo_bin tree = bin_open(path_and_name, this); // O header é a struct da árvore

  this->tree = tree;

  // path_and_name + "_item" + \0
  char nome[strlen(path_and_name) + 5 + 1];
  strcpy(nome, path_and_name);
  strcat(nome, "_item");

  this->itens = bin_open(path_and_name, NULL);

  return this;
}

void bt_destroy(BTree_t _this, void (*destruir_item)()) {
  struct BTree_t * this = (struct BTree_t *) _this;

  // Se a arvore nao ta vazia
  // if (this->root != -1) {
  //   destroy_node(this->root, destruir_item);
  // }
  
  bin_close(this->tree, this);
  bin_close(this->itens, NULL);
  free(this);
}

// Não implementado com arquivo
/*void bt_traverse(BTree_t _this, void (*callback)(void *item, void *user_data), void *user_data) {
  struct BTree_t * this = (struct BTree_t *) _this;

  if (this->root != NULL) {
    traverse_node(this->root, callback, user_data);
  }
}*/

void *bt_search(BTree_t _this, char *chave) {
  struct BTree_t * this = (struct BTree_t *) _this;

  if (this->root == -1){
    printf("A arvore esta vazia\n");
    return NULL;
  }


  BTreeNode_t root = bin_get_item(this->tree, this->root);
  void *item = search_node(root, chave, this->compare, this->tree, this->itens);
  free(root);

  return item;
}

void *bt_insert(BTree_t _this, char *chave, void *valor) {
  struct BTree_t * this = (struct BTree_t *) _this;

  // Se a arvore estiver vazia
  if (this->root == -1) {
    int item = bin_insert(this->itens, valor, -1);
    BTreeNode_t root = create_node(true);

    strcpy(root->chaves[0].chave, chave);
    root->chaves[0].valor = item;
    root->numero_filhos = 1;

    this->root = bin_get_free_block(this->tree);
    root->posic_arquivo = this->root;
    bin_insert(this->tree, root, this->root);
    free(root);
  }

  // Se a arvore nao esta vazia
  else {

    // Se a raiz ta cheia, a arvore cresce em altura
    BTreeNode_t root = bin_get_item(this->tree, this->root);
    if (is_full_node(root)) {
      
      // Criando espaco pra nova raiz
      BTreeNode_t nova_raiz = create_node(false);

      // Fazer a raiz antiga filha da nova
      nova_raiz->filhos[0] = this->root;

      // Splita a raiz antiga
      split_child_node(nova_raiz, 0, root, this->tree);

      // Nova raiz tem dois filhos
      // Escolher para onde inserir
      int i = 0;
      if (this->compare(nova_raiz->chaves[0].chave, chave) == 1)
        i++;
      
      BTreeNode_t filho = bin_get_item(this->tree, nova_raiz->filhos[i]);
      insert_non_full_node(filho, chave, valor, this->compare, this->tree, this->itens);
      bin_insert(this->tree, filho, filho->posic_arquivo);

      bin_insert(this->tree, root, root->posic_arquivo);
      nova_raiz->posic_arquivo = bin_get_free_block(this->tree);
      
      bin_insert(this->tree, nova_raiz, nova_raiz->posic_arquivo);

      // Mudar a raiz
      this->root = nova_raiz->posic_arquivo;
      free(nova_raiz);
      free(root);
    }

    // Se a raiz nao estiver cheia, so adicionar usando a inser_non_full_node
    else{
      // BTreeNode_t root = bin_get_item(this->tree, this->root);
      insert_non_full_node(root, chave, valor, this->compare, this->tree, this->itens);
      bin_insert(this->tree, root, root->posic_arquivo);
      free(root);
    }

  }
  return NULL;
}
// CONTINUAR A PARTIR DAQUI----
void *bt_remove(BTree_t _this, char *chave) {
  struct BTree_t *this = (struct BTree_t *) _this;

  if (this->root == -1) {
    printf("A arvore esta vazia.\n");
    return NULL;
  }
  BTreeNode_t root = bin_get_item(this->tree, this->root);
  void *retorno = remove_node(root, chave, this->compare, this->tree, this->itens, 1);
  bin_insert(this->tree, root, root->posic_arquivo);

  if (root->numero_filhos == 0) {
    // BTreeNode_t tmp = this->root;

    if (root->folha)
      this->root = -1;
    else
      this->root = root->filhos[0];
    
    bin_remove(this->tree, root->posic_arquivo, 0);

    // destroy_node(tmp, NULL);
  }
  
  return retorno;
}

bool bt_is_empty(BTree_t _this) {
  struct BTree_t * this = (struct BTree_t *) _this;
  return (this->root == -1);
}

/*Lista_t bt_range_search(BTree_t _this, char *chave_min, char *chave_max){
  struct BTree_t* this = _this;
  Lista_t lista = lt_create();
  range_search_recursive(this->root, chave_min, chave_max, lista);
  return lista;
}*/