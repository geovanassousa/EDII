#ifndef STREAM_H
#define STREAM_H

#include "tipos.h"
#include "categoria.h"

typedef struct Stream {
    char nome[TXT_GRD];
    char site[TXT_GRD];

    Categoria *categorias;   /* cabeça da lista circular (pode ser NULL) */

    struct Stream *esq;      /* BST por nome da stream */
    struct Stream *dir;
} Stream;

Stream* stream_criar(const char *nome, const char *site);

#endif
