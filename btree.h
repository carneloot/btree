#ifndef __BTREE_H__
#define __BTREE_H__

/**
 * Tipo que representa uma Arvore B
 */
typedef void *BTree_t;

/**
 * Retorna uma nova BTree vazia de grau grau
 */
BTree_t bt_create(int grau);

/**
 * Destroi a BTree.
 * Caso a funcao destroy_item seja passada, os itens sao destruidos tambem 
 * Se nao quiser destruir os itens, passar destroy_item como NULL
 */
void bt_destroy(BTree_t this, void (*destroy_item)(void *item));

/**
 * Percorre a BTree aplicando uma funcao de callback em todo item
 */
void bt_traverse(BTree_t this, void (*callback)(void *item, void *user_data), void *user_data);

/**
 * Procura por um item com chave chave.
 * Se encontrado, retorna esse item
 */
void *bt_search(BTree_t this, double chave);


#endif /* __BTREE_H__ */