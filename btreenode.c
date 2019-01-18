#include "btreenode.h"

#include <stdlib.h>

BTreeNode_t create_node(int grau, bool folha) {
  BTreeNode_t this = (BTreeNode_t) calloc(1, sizeof(*this));

  this->grau  = grau;
  this->folha = folha;

  this->chaves = (BTreePair_t *) calloc(2 * grau - 1, sizeof(*this->chaves));
  this->filhos = (BTreeNode_t *) calloc(2 * grau,     sizeof(*this->filhos));

  this->numero_filhos = 0;

  return this;
}

void destroy_node(BTreeNode_t this, void *_destruir_item) {
  void (*destruir_item)(void *item) = (void (*)(void *)) _destruir_item;

  if (this->folha == false)
    for (int i = 0; i < this->numero_filhos; i++)
      destroy_node(this->filhos[i], destruir_item);

  free(this->filhos);

  int tam = 2 * this->grau - 1;
  for (int i = 0; i < tam; i++) {
    if (this->chaves[i] == NULL) break;

    if (destruir_item)
      destruir_item(this->chaves[i]->valor);

    free(this->chaves[i]);
  }

  free(this->chaves);

  free(this);
}

bool is_full_node(BTreeNode_t this) {
  return (this->numero_filhos == 2 * this->grau - 1);
}

void traverse_node(BTreeNode_t this, void (*callback)(void *item, void *user_data), void *user_data) {
  int i;

  for (i = 0; i < this->numero_filhos; i++) {
    if (this->folha == false)
      traverse_node(this->filhos[i], callback, user_data);

    callback(this->chaves[i]->valor, user_data);
  }

  if (this->folha == false)
    traverse_node(this->filhos[i], callback, user_data);

}

void *search_node(BTreeNode_t this, double chave) {
  int i = 0;

  // Percorre ate achar a chave "maior" que a passada
  while (i < this->numero_filhos && chave > this->chaves[i]->chave)
    i++;

  // Se a chave for igual, retorna o valor atual
  if (this->chaves[i] && this->chaves[i]->chave == chave)
    return this->chaves[i]->valor;
  
  // Se chegou numa folha, quer dizer que nao achou o item
  if (this->folha == true)
    return NULL;

  // Procura no filho
  return search_node(this->filhos[i], chave);
}

/**
 * Funcao que splita o filho do node this.
 * Filho deve estar cheio.
 */
void split_child_node(BTreeNode_t this, int indice, BTreeNode_t child) {

  // Cria um novo no que vai guardar this->grau - 1 valores
  BTreeNode_t novo_node = create_node(child->grau, child->folha);
  novo_node->numero_filhos = this->grau - 1;

  // Copia os ultimos this->grau - 1 valores de child para novo_node
  for (int j = 0; j < this->grau - 1; j++)
    novo_node->chaves[j] = child->chaves[j + this->grau];

  // Copia os ultimos this->grau filhos de child para novo_node
  if (child->folha == false) {
    for (int j = 0; j < this->grau; j++)
      novo_node->filhos[j] = child->filhos[j + this->grau];
  }

  // Reduzir o numero de valores em child
  child->numero_filhos = this->grau - 1;

  // Abrir espaco para o novo filho
  for (int j = this->numero_filhos; j >= indice + 1; j--) {
    this->filhos[j + 1] = this->filhos[j];
  }

  // Colocar o novo filho em this
  this->filhos[indice + 1] = novo_node;

  // Achar a posicao e mover os valores da chave de child
  for (int j = this->numero_filhos - 1; j >= indice; j--)
    this->chaves[j + 1] = this->chaves[j];

  // Copia a chave do meio de child para this
  this->chaves[indice] = child->chaves[this->grau - 1];

  // Incrementar o numero de filhos em this
  this->numero_filhos++;

}

/**
 * Funcao auxiliar para inserir um novo par no node.
 * Assume-se que o no nao esteja cheio quando essa funcao eh chamada
 */
void insert_non_full_node(BTreeNode_t this, double chave, void *valor) {

  // Inicializa o indice como elemento mais a direita
  int i = this->numero_filhos - 1;

  // Se for uma folha
  if (this->folha == true) {

    // Achar o lugar a ser inserido o novo item
    // E mover todos os de valor maior pra frente
    while (i >= 0 && this->chaves[i]->chave > chave) {
      this->chaves[i + 1] = this->chaves[i];
      i--;
    }

    // Inserir o valor no local encontrado
    this->chaves[i + 1] = malloc(sizeof(*this->chaves[i + 1]));
    this->chaves[i + 1]->chave = chave;
    this->chaves[i + 1]->valor = valor;
    this->numero_filhos++;
  }

  // Se nao for uma folha
  else {
    // Achar o filho que tem que ter o novo valor
    while (i >= 0 && this->chaves[i]->chave > chave)
      i--;
    
    // Se o filho estiver cheio
    if (is_full_node(this->filhos[i + 1])) {
      split_child_node(this, i + 1, this->filhos[i + 1]);

      // Depois de mudar, o no vai ser separado em dois
      // Tem que achar em qual dos dois vai o novo valor
      if (this->chaves[i + 1]->chave < chave)
        i++;
    }
    insert_non_full_node(this->filhos[i + 1], chave, valor);
  }
}