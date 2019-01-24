#include "btreenode.h"
#include "lista.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

BTreeNode_t create_node( bool folha) {
  BTreeNode_t this = (BTreeNode_t) calloc(1, sizeof(*this));

  this->folha = folha;
  this->numero_filhos = 0;

  return this;
}

void destroy_node(BTreeNode_t this, void *_destruir_item) {
  return;
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

void *search_node(BTreeNode_t this, char *chave, int (*compare)(void *this, void *other), Arquivo tree, Arquivo item) {
  int i = 0;

  // Percorre ate achar a chave "maior" que a passada
  while (i < this->numero_filhos && compare(this->chaves[i].chave, chave) > 0)
    i++;

  // Se a chave for igual, retorna o valor atual
  if (compare(this->chaves[i].chave, chave) == 0)
    return bin_get_item(item, this->filhos[i]);
  
  
  // Se chegou numa folha, quer dizer que nao achou o item
  if (this->folha == true)
    return NULL;

  // Procura no filho
  BTreeNode_t filho = bin_get_item(tree, this->filhos[i]);
  void *item = search_node(filho, chave, compare, tree, item);
  free(filho);

  return item;
}

void split_child_node(BTreeNode_t this, int indice, BTreeNode_t child) {

  // Cria um novo no que vai guardar this->grau - 1 valores
  BTreeNode_t novo_node = create_node(child->folha);
  novo_node->numero_filhos = GRAU - 1;

  // Copia os ultimos this->grau - 1 valores de child para novo_node
  for (int j = 0; j < GRAU - 1; j++)
    novo_node->chaves[j] = child->chaves[j + GRAU];

  // Copia os ultimos GRAU filhos de child para novo_node
  if (child->folha == false) {
    for (int j = 0; j < GRAU; j++)
      novo_node->filhos[j] = child->filhos[j + GRAU];
  }

  // Reduzir o numero de valores em child
  child->numero_filhos = GRAU - 1;

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
  this->chaves[indice] = child->chaves[GRAU - 1];

  // Incrementar o numero de filhos em this
  this->numero_filhos++;

}

void insert_non_full_node(BTreeNode_t this, char *chave, void *valor) {

  // Inicializa o indice como elemento mais a direita
  int i = this->numero_filhos - 1;

  // Se for uma folha
  if (this->folha == true) {

    // Achar o lugar a ser inserido o novo item
    // E mover todos os de valor maior pra frente
    while (i >= 0 && this->compare(this->chaves[i]->chave, chave) > 0) {
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
    while (i >= 0 && this->compare(this->chaves[i]->chave, chave) > 0)
      i--;
    
    // Se o filho estiver cheio
    if (is_full_node(this->filhos[i + 1])) {
      split_child_node(this, i + 1, this->filhos[i + 1]);

      // Depois de mudar, o no vai ser separado em dois
      // Tem que achar em qual dos dois vai o novo valor
      if (this->compare(this->chaves[i + 1]->chave, chave) < 0)
        i++;
    }
    insert_non_full_node(this->filhos[i + 1], chave, valor);
  }
}

int find_key_node(BTreeNode_t this, char *chave) {
  int indice = 0;
  while (indice < this->numero_filhos &&
    this->compare(this->chaves[indice]->chave, chave) < 0)
    indice++;

  return indice;
}

void *remove_node(BTreeNode_t this, char *chave) {
  int indice = find_key_node(this, chave);

  // Se a chave estiver nesse no
  if (indice < this->numero_filhos &&
    this->compare(this->chaves[indice]->chave, chave) == 0) {
    
    if (this->folha)
      return remove_from_leaf_node(this, indice);
    else
      return remove_from_non_leaf_node(this, indice);
  }

  // Se nao estiver nesse no, procurar nos filhos
  else {

    // Se for uma folha, entao o item nao existe
    if (this->folha) {
      printf("A chave \"%s\" nao existe na arvore.\n", chave);
      return NULL;
    }

    // A flag indice se o item esta no ultimo filho desse no
    bool flag = ( indice == this->numero_filhos );

    // Se o filho que o valor supostamente esta tem menos que o grau minimo, tem que preencher ele
    if (this->filhos[indice]->numero_filhos < this->grau)
      fill_node(this, indice);

    if (flag && indice > this->numero_filhos)
      return remove_node(this->filhos[indice - 1], chave);
    else
      return remove_node(this->filhos[indice],     chave);
    
  }

}

void *remove_from_leaf_node(BTreeNode_t this, int indice) {
  void *retorno = this->chaves[indice]->valor;

  free(this->chaves[indice]);

  for (int i = indice + 1; i < this->numero_filhos; i++) {
    this->chaves[i - 1] = this->chaves[i];
  }

  this->numero_filhos--;

  return retorno;
}

void *remove_from_non_leaf_node(BTreeNode_t this, int indice) {

  if (this->filhos[indice]->numero_filhos >= this->grau) {
    BTreePair_t pred = get_predecessor_node(this, indice);
    free(this->chaves[indice]);
    this->chaves[indice] = pred;
    return remove_node(this->filhos[indice], pred->chave);
  }

  else if (this->filhos[indice + 1]->numero_filhos >= this->grau) {
    BTreePair_t succ = get_successor_node(this, indice);
    free(this->chaves[indice]);
    this->chaves[indice] = succ;
    return remove_node(this->filhos[indice + 1], succ->chave);
  }

  else {
    char *chave = this->chaves[indice]->chave;
    merge_node(this, indice);
    return remove_node(this->filhos[indice], chave);
  }
}

BTreePair_t get_predecessor_node(BTreeNode_t this, int indice) {
  // Sempre pega o ultimo dos filhos até chegar numa folha
  BTreeNode_t cur = this->filhos[indice];
  while (cur->folha == false)
    cur = cur->filhos[cur->numero_filhos];

  // Retorna o ultimo da folha
  return cur->chaves[cur->numero_filhos - 1];
}

BTreePair_t get_successor_node(BTreeNode_t this, int indice) {
  // Sempre pega o primeiro dos filhos até chegar numa folha
  BTreeNode_t cur = this->filhos[indice + 1];
  while (cur->folha == false)
    cur = cur->filhos[0];

  // Retorna o primeiro da folha
  return cur->chaves[0];
}

void fill_node(BTreeNode_t this, int indice) {

  if (indice != 0 && this->filhos[indice - 1]->numero_filhos >= this->grau)
    borrow_from_prev_node(this, indice);

  else if (indice != this->numero_filhos && this->filhos[indice + 1]->numero_filhos >= this->grau)
    borrow_from_next_node(this, indice);

  else {
    if (indice != this->numero_filhos)
      merge_node(this, indice);
    else
      merge_node(this, indice - 1);
  }
}

void borrow_from_prev_node(BTreeNode_t this, int indice) {
  BTreeNode_t filho = this->filhos[indice];
  BTreeNode_t irmao = this->filhos[indice - 1];

  // Move todos em this->filhos para a frente
  for (int i = filho->numero_filhos - 1; i>= 0; i--)
    filho->chaves[i + 1] = filho->chaves[i];
  
  // Se o filho nao for uma folha, move todos os filhos dele pra frente
  if (filho->folha == false)
    for (int i = filho->numero_filhos; i >= 0; i--)
      filho->filhos[i + 1] = filho->filhos[i];

  filho->chaves[0] = this->chaves[indice - 1];

  if (filho->folha == false)
    filho->filhos[0] = irmao->filhos[irmao->numero_filhos];

  this->chaves[indice - 1] = irmao->chaves[irmao->numero_filhos - 1];

  filho->numero_filhos += 1;
  irmao->numero_filhos -= 1;
}

void borrow_from_next_node(BTreeNode_t this, int indice) {
  BTreeNode_t filho = this->filhos[indice];
  BTreeNode_t irmao = this->filhos[indice + 1];

  filho->chaves[filho->numero_filhos] = this->chaves[indice];

  if (filho->folha == false)
    filho->filhos[filho->numero_filhos + 1] = irmao->filhos[0];

  this->chaves[indice] = irmao->chaves[0];

  for (int i = 1; i < irmao->numero_filhos; i++)
    irmao->chaves[i - 1] = irmao->chaves[i];

  if (irmao->folha == false)
    for (int i = 1; i <= irmao->numero_filhos; i++)
      irmao->filhos[i - 1] = irmao->filhos[i];

  filho->numero_filhos += 1;
  irmao->numero_filhos -= 1;
}

void merge_node(BTreeNode_t this, int indice) {
  BTreeNode_t filho = this->filhos[indice];
  BTreeNode_t irmao = this->filhos[indice + 1];

  filho->chaves[this->grau - 1] = this->chaves[indice];

  for (int i = 0; i < irmao->numero_filhos; i++)
    filho->chaves[i + this->grau] = irmao->chaves[i];

  if (filho->folha == false)
    for (int i = 0; i <= irmao->numero_filhos; i++)
      filho->filhos[i + this->grau] = irmao->filhos[i];

  for (int i = indice + 1; i < this->numero_filhos; i++)
    this->chaves[i - 1] = this->chaves[i];

  for (int i = indice + 2; i <= this->numero_filhos; i++)
    this->filhos[i - 1] = this->filhos[i];
  
  filho->numero_filhos += irmao->numero_filhos + 1;
  this->numero_filhos--;

  destroy_node(irmao, NULL);
}

void range_search_recursive(BTreeNode_t node, char *chave_min, char *chave_max, Lista_t lista) {
  
  if (node->folha) {
    int sair = 0;
    for (int i = 0; i < node->numero_filhos; i++) {

      int result = intervalo_chave(node->chaves[i]->chave, chave_min, chave_max, node->compare);
      switch (result) {
        case -1:
          continue;

        case 0:
          lt_insert(lista, node->chaves[i]->valor);
          break;

        case 1:
          sair = 1;
          break;
      }

      if (sair) break;

    }

  } else {
    int sair = 0;
    for (int i = 0; i < node->numero_filhos; i++) {

      int result = intervalo_chave(node->chaves[i]->chave, chave_min, chave_max, node->compare);
      switch (result) {
        case -1:
          continue;

        case 0:
          range_search_recursive(node->filhos[i], chave_min, chave_max, lista);
          lt_insert(lista, node->chaves[i]->valor);
          break;

        case 1:
          sair = 1;
          range_search_recursive(node->filhos[i], chave_min, chave_max, lista);
          break;
      }
      if (sair) break;

    }
    // Se sair é 0, quer dizer que o último item era 0 e o for chegou no final
    // Então, procurar na chave mais a direita
    if (sair == 0) {
      range_search_recursive(node->filhos[node->numero_filhos], chave_min, chave_max, lista);
    }

  }
    
}

int intervalo_chave(char *chave , char *chave_min, char *chave_max, int (*compare)(void *this, void *other)) {
  if (compare(chave, chave_min) < 0) {
    // Chave é menor que mínimo
    return -1;
  }

  if ( compare(chave, chave_max) > 0) {
    // Chave é maior que max
    return 1;
  }
  
  // Chave está entre os dois
  return 0;
}