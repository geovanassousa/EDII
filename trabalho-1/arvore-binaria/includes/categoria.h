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

/* cria uma categoria isolada (prox = NULL) */
Categoria* cat_criar(const char *nome, TipoCategoria tipo);

/* verifica se existe categoria com esse nome na lista circular (1 = sim, 0 = não) */
int cat_existe(Categoria *cabeca, const char *nome);

/* insere em ORDEM alfabética; mantém lista circular; *inseriu = 1 se inseriu, 0 se duplicado/erro */
void cat_inserir_ordenado(Categoria **cabeca, const char *nome, TipoCategoria tipo, int *inseriu);

/* imprime todas as categorias da lista circular */
void cat_listar(Categoria *cabeca);

#endif
