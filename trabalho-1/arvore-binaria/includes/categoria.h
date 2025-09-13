#ifndef CATEGORIA_H
#define CATEGORIA_H

#include "tipos.h"
#include "programa.h"

typedef struct Categoria {
    char nome[TXT_GRD];
    TipoCategoria tipo;
    Programa *raizProgramas;    /* BST de programas desta categoria */

    struct Categoria *prox;     /* lista circular simples */
} Categoria;

/* Stubs por enquanto; vamos implementar aos poucos */
Categoria* cat_criar(const char *nome, TipoCategoria tipo);

#endif
