#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "binfile.h"

struct Arquivo{
  FILE *file;
  unsigned item_size;
  unsigned max_index;  // próximo bloco livre
  unsigned total_header_size;
};


// Cria o arquivo e escreve o header dele
Arquivo bin_create(char *file_path, unsigned item_size, void *header, unsigned size_of_header){
  struct Arquivo* arquivo = malloc(sizeof(struct Arquivo));
  arquivo->file = fopen(file_path, "wb+");
  arquivo->item_size = item_size;
  arquivo->max_index = 0;
  arquivo->total_header_size = sizeof(struct Arquivo) + size_of_header;

  fwrite(arquivo, sizeof(struct Arquivo), 1, arquivo->file);
  // Header é opcional
  if(header != NULL){
    fwrite(header, size_of_header, 1, arquivo->file);
  }

  bool ocupado = 0;
  fwrite(&ocupado, sizeof(bool), 1, arquivo->file);

  return arquivo;
}

Arquivo bin_open(char *file_path, void *header){
  struct Arquivo* arquivo = malloc(sizeof(struct Arquivo));

  FILE *fp = fopen(file_path, "rb+");
  

  fread(arquivo, sizeof(struct Arquivo), 1, fp);
  arquivo->file = fp;

  if(header != NULL){
    fread(header, arquivo->total_header_size - sizeof(struct Arquivo), 1, arquivo->file);
  }

  return arquivo;
}

void bin_close(Arquivo _arquivo, void *header){
  struct Arquivo* arquivo = _arquivo;
  fseek(arquivo->file, 0, SEEK_SET);
  fwrite(arquivo, sizeof(struct Arquivo), 1, arquivo->file);
  if(header != NULL){
    fwrite(header, arquivo->total_header_size - sizeof(struct Arquivo), 1, arquivo->file);
  }
  fclose(arquivo->file);
  free(arquivo);
}


Posic bin_insert(Arquivo _arquivo, void* item, int posic){
  struct Arquivo *arquivo = _arquivo;

  unsigned block = 0;
  unsigned block_size = arquivo->item_size + sizeof(bool);

  if(posic != -1){
    block = posic;
    fseek(arquivo->file, arquivo->total_header_size + block_size * block, SEEK_SET );
    bool ocupado = 1;
    fwrite(&ocupado, sizeof(bool), 1, arquivo->file);
    fwrite(item, arquivo->item_size, 1, arquivo->file);

    if(posic >= arquivo->max_index){
      arquivo->max_index = posic + 1;
      ocupado = 0;
      fwrite(&ocupado, sizeof(bool), 1, arquivo->file);
    }

    return posic;
  }

  bool ocupado = 1;
  fseek(arquivo->file, arquivo->total_header_size, SEEK_SET);
  block = 0;
  while(1){
    fread(&ocupado, sizeof(bool), 1, arquivo->file);
    if(ocupado == 0){
      fseek(arquivo->file, -sizeof(bool), SEEK_CUR);
      break;
    }
    fseek(arquivo->file, arquivo->item_size, SEEK_CUR);
    block++;
  }
  
  // achou espaço livre

  ocupado = 1;
  fwrite(&ocupado, sizeof(bool), 1, arquivo->file);
  fwrite(item, arquivo->item_size, 1, arquivo->file);
  if(block >= arquivo->max_index){
    ocupado = 0;
    arquivo->max_index = block + 1;
    fwrite(&ocupado, sizeof(bool), 1, arquivo->file);
  }
  return block;
}

Item bin_remove(Arquivo _arquivo, Posic posic, bool return_item){
  struct Arquivo *arquivo = _arquivo;

  fseek(arquivo->file, arquivo->total_header_size + (arquivo->item_size + sizeof(bool))*posic, SEEK_SET );
  bool ocupado = 0;
  fwrite(&ocupado, sizeof(bool), 1, arquivo->file);

  if(return_item){
    void *item = malloc(arquivo->item_size);
    fread(item, arquivo->item_size, 1, arquivo->file);
    return item;
  }
  return NULL;
}

Item bin_get_item(Arquivo _arquivo, Posic posic ){
  struct Arquivo *arquivo = _arquivo;

  fseek(arquivo->file, arquivo->total_header_size + (arquivo->item_size + sizeof(bool)) * posic , SEEK_SET);
  fseek(arquivo->file, sizeof(bool), SEEK_CUR);
  void *item = malloc(arquivo->item_size);
  fread(item, arquivo->item_size, 1, arquivo->file);
  return item;
}

Item bin_get_first(Arquivo _arquivo){
  struct Arquivo *arquivo = _arquivo;
  fseek(arquivo->file, arquivo->total_header_size + sizeof(bool), SEEK_SET);
  void *item = malloc(arquivo->item_size);
  fread(item, arquivo->item_size, 1, arquivo->file);
  return item;
}

int bin_get_free_block(Arquivo _arquivo){
  struct Arquivo *arquivo = _arquivo;
  int block;

  bool ocupado = 1;
  fseek(arquivo->file, arquivo->total_header_size, SEEK_SET);
  block = 0;
  while(1){
    fread(&ocupado, sizeof(bool), 1, arquivo->file);
    if(ocupado == 0){
      fseek(arquivo->file, -sizeof(bool), SEEK_CUR);
      break;
    }
    fseek(arquivo->file, arquivo->item_size, SEEK_CUR);
    block++;
  }

  return block;
}