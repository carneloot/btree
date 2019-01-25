#ifndef __BTREENODE_H__
#define __BTREENODE_H__

#include <stdbool.h>
#include "lista.h"
#include "binfile.h"

/**
 *  Não consegui pensar em uma forma do usuário escolher
 *  o grau da árvore e fosse prático de implementar.
 *  Devido à falta de tempo, o grau será fixo
 */

#define GRAU 20 

/** Par chave / item */
struct BTreePair_t {
  char chave[30];
  int valor;  // Guarda a posição do Item no arquivo
};

typedef struct BTreePair_t *BTreePair_t;

struct BTreeNode_t {
  struct BTreePair_t chaves[2 * GRAU - 1];         // Guarda todas as chaves desse no
  // int grau;                    // Grau minimo do no
  // struct BTreeNode_t **filhos; // Um vetor de filhos
  int filhos[2 * GRAU];
  int numero_filhos;           // Numero atual de filhos
  bool folha;                  // Determina se o no eh uma folha ou nao
  int posic_arquivo;           // Posição em que está no arquivo
};

typedef struct BTreeNode_t *BTreeNode_t;

BTreeNode_t create_node(bool folha);

void destroy_node(BTreeNode_t this, void *_destruir_item);

bool is_full_node(BTreeNode_t this);

void traverse_node(BTreeNode_t this, void (*callback)(void *item, void *user_data), void *user_data);

void *search_node(BTreeNode_t this, char *chave, int (*compare)(void *this, void *other), Arquivo tree, Arquivo item);

/**
 * Funcao que splita o filho do node this.
 * Filho deve estar cheio.
 */
void split_child_node(BTreeNode_t this, int indice, BTreeNode_t child, Arquivo tree);

/**
 * Funcao auxiliar para inserir um novo par no node.
 * Assume-se que o no nao esteja cheio quando essa funcao eh chamada
 */
void insert_non_full_node(BTreeNode_t this, char *chave, void *valor, int (*compare)(void *this, void *other), Arquivo tree, Arquivo itens);

/**
 * Funcao que retorna o indice da primeira chave maior que a chave passada
 */
int find_key_node(BTreeNode_t this, char *chave, int (*compare)(void *this, void *other));

/**
 * Um container para a remocao da chave chave
 */
void *remove_node(BTreeNode_t this, char *chave, int (*compare)(void *this, void *other), Arquivo tree, Arquivo itens, bool return_deleted);

/**
 * Remove a chave presente no indice de um no folha
 */
void *remove_from_leaf_node(BTreeNode_t this, int indice, Arquivo tree, Arquivo itens, bool return_deleted);

/**
 * Remove a chave presente no indice de um no nao folha
 */
void *remove_from_non_leaf_node(BTreeNode_t this, int indice, int (*compare)(void *this, void *other), Arquivo tree, Arquivo itens, bool return_deleted);

/**
 * Pega o predecessor do indice
 */
BTreeNode_t get_predecessor_node(BTreeNode_t this, int indice, Arquivo tree);

/**
 * Pega o sucessor do indice
 */
BTreeNode_t get_successor_node(BTreeNode_t this, int indice, Arquivo tree);

/**
 * Preenche o filho no indice se o filho tem menos que grau - 1 itens
 */
void fill_node(BTreeNode_t this, int indice, Arquivo tree);

/**
 * Empresta a chave do anterior
 */
void borrow_from_prev_node(BTreeNode_t this, int indice, Arquivo tree);

/**
 * Empresta a chave do proximo
 */
void borrow_from_next_node(BTreeNode_t this, int indice, Arquivo tree);

/**
 * Junta os filhos do indice com os do (indice + 1)
 */
void merge_node(BTreeNode_t this, int indice, Arquivo tree);

/**
 * -1 -> chave < min
 *  0 -> min < chave < max
 *  1 -> max < chave
 */
int intervalo_chave(char *chave , char *chave_min, char *chave_max, int (*compare)(void *this, void *other));

void range_search_recursive(BTreeNode_t node, char *chave_min, char *chave_max, Lista_t lista);

#endif /* __BTREENODE_H__ */