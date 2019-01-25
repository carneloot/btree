#ifndef __BINFILE_H__
#define __BINFILE_H__
#include <stdio.h>
#include <stdbool.h>

typedef void * Arquivo_bin;

typedef unsigned Posic;

typedef void * Item;

/**
 * file_path: Path and name of file
 * 
 * 
*/
Arquivo_bin bin_create(char *file_path, unsigned item_size, void *header, unsigned size_of_header);

/**
 * Open a file
*/
Arquivo_bin bin_open(char *file_path, void *header);

/**
 * Closes a file
 */
void bin_close(Arquivo_bin _arquivo, void *header);

/**
 * Inserts an item in position posic.
 * If posic == -1, inserts in next avaliable space.
 * Returns position where it was inserted.
 * The function may overwrite the item if posic points
 *  to an occupied position
*/
Posic bin_insert(Arquivo_bin arquivo, Item item, int posic);

/**
 * Removes item from file.
 * Returns item if return_item = 1
*/
Item bin_remove(Arquivo_bin arquivo, Posic posic, bool return_item);

/**
 * Get Item from posic
 */
Item bin_get_item(Arquivo_bin arquivo, Posic posic );

/**
 * Get the first item
 */
Item bin_get_first(Arquivo_bin arquivo);

/**
 * Returns the first free block in
 * file
 */
int bin_get_free_block(Arquivo_bin arquivo);

#endif // __BIN_FILE__