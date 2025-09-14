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
    char categoria[TXT_GRD];     /* categoria que trabalha (texto p/ simplificar) */
    char streamAtual[TXT_GRD];   /* deve existir entre as streams */

    VetHist historico;           /* vetor ordenado por nome da stream (implementamos depois) */

    struct Apresentador *ant;    /* lista duplamente encadeada ordenada por nome */
    struct Apresentador *prox;
} Apresentador;

/* criação simples */
Apresentador* apr_criar(const char *nome, const char *categoria, const char *streamAtual);

/* inserir em ORDEM alfabética por nome; *inseriu = 1 se inseriu, 0 se duplicado/erro */
void apr_inserir_ordenado(Apresentador **cabeca, const char *nome,
                          const char *categoria, const char *streamAtual, int *inseriu);

/* existe por nome? (1=sim, 0=nao) */
int apr_existe_nome(Apresentador *cabeca, const char *nome);

/* para validação do item (iii) */
int apr_pode_apresentar(Apresentador *cabeca, const char *nomeApr,
                        const char *categoriaNec, const char *streamNec);

/* listagens (xii) e (xiii) */
void apr_listar_da_stream(Apresentador *cabeca, const char *nomeStream);
void apr_listar_da_categoria(Apresentador *cabeca, const char *nomeCategoria);

/* util: imprimir todos (debug) */
void apr_listar_todos(Apresentador *cabeca);

#endif
