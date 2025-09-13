#ifndef APRESENTADOR_H
#define APRESENTADOR_H

#include "tipos.h"

typedef struct HistItem {
    char stream[TXT_GRD];
    char dataIni[11];  /* "DD/MM/AAAA" */
    char dataFim[11];
} HistItem;

typedef struct VetHist {
    HistItem *itens;
    int qtd;
    int cap;
} VetHist;

typedef struct Apresentador {
    char nome[TXT_GRD];
    char categoria[TXT_GRD];     /* categoria que trabalha */
    char streamAtual[TXT_GRD];   /* precisa estar cadastrada */

    VetHist historico;           /* vetor ordenado por nome da stream */

    struct Apresentador *ant;    /* lista duplamente encadeada ordenada */
    struct Apresentador *prox;
} Apresentador;

/* Criação simples; lógica de ordenar na inserção virá depois */
Apresentador* apr_criar(const char *nome, const char *categoria, const char *streamAtual);

#endif
