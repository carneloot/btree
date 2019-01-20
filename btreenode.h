#ifndef __BTREENODE_H__
#define __BTREENODE_H__

#include <stdbool.h>

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


BTreeNode_t create_node(int grau, bool folha);

void destroy_node(BTreeNode_t this, void *_destruir_item);

bool is_full_node(BTreeNode_t this);

void traverse_node(BTreeNode_t this, void (*callback)(void *item, void *user_data), void *user_data);

void *search_node(BTreeNode_t this, char *chave);

/**
 * Funcao que splita o filho do node this.
 * Filho deve estar cheio.
 */
void split_child_node(BTreeNode_t this, int indice, BTreeNode_t child);

/**
 * Funcao auxiliar para inserir um novo par no node.
 * Assume-se que o no nao esteja cheio quando essa funcao eh chamada
 */
void insert_non_full_node(BTreeNode_t this, char *chave, void *valor);

/**
 * Funcao que retorna o indice da primeira chave maior que a chave passada
 */
int find_key_node(BTreeNode_t this, char *chave);

/**
 * Um container para a remocao da chave chave
 */
void *remove_node(BTreeNode_t this, char *chave);

/**
 * Remove a chave presente no indice de um no folha
 */
void *remove_from_leaf_node(BTreeNode_t this, int indice);

/**
 * Remove a chave presente no indice de um no nao folha
 */
void *remove_from_non_leaf_node(BTreeNode_t this, int indice);

/**
 * Pega o predecessor do indice
 */
BTreePair_t get_predecessor_node(BTreeNode_t this, int indice);

/**
 * Pega o sucessor do indice
 */
BTreePair_t get_successor_node(BTreeNode_t this, int indice);

/**
 * Preenche o filho no indice se o filho tem menos que grau - 1 itens
 */
void fill_node(BTreeNode_t this, int indice);

/**
 * Empreta a chave do anterior
 */
void borrow_from_prev_node(BTreeNode_t this, int indice);

/**
 * Empreta a chave do proximo
 */
void borrow_from_next_node(BTreeNode_t this, int indice);

/**
 * Junta os filhos do indice com os do (indice + 1)
 */
void merge_node(BTreeNode_t this, int indice);

#endif /* __BTREENODE_H__ */