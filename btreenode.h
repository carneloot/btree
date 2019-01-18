#ifndef __BTREENODE_H__
#define __BTREENODE_H__

#include <stdbool.h>

/** Par chave / item */
struct BTreePair_t {
  double chave;
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


BTreeNode_t create_node(int grau, bool folha);

void destroy_node(BTreeNode_t this, void *_destruir_item);

bool is_full_node(BTreeNode_t this);

void traverse_node(BTreeNode_t this, void (*callback)(void *item, void *user_data), void *user_data);

void *search_node(BTreeNode_t this, double chave);

/**
 * Funcao que splita o filho do node this.
 * Filho deve estar cheio.
 */
void split_child_node(BTreeNode_t this, int indice, BTreeNode_t child);

/**
 * Funcao auxiliar para inserir um novo par no node.
 * Assume-se que o no nao esteja cheio quando essa funcao eh chamada
 */
void insert_non_full_node(BTreeNode_t this, double chave, void *valor);

#endif /* __BTREENODE_H__ */