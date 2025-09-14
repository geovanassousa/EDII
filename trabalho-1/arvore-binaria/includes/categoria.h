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

Categoria* cat_criar(const char *nome, TipoCategoria tipo);
int  cat_existe(Categoria *cabeca, const char *nome);
void cat_inserir_ordenado(Categoria **cabeca, const char *nome, TipoCategoria tipo, int *inseriu);
void cat_listar(Categoria *cabeca);

/*enumera nós da lista circular (em ordem) para vetor; retorna quantidade */
int  cat_enumerar(Categoria *cabeca, Categoria **vet, int max);

#endif
