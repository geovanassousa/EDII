#ifndef APRESENTADOR_H
#define APRESENTADOR_H

#include "tipos.h"

typedef struct HistItem {
    char stream[TXT_GRD];
    char dataIni[11];
    char dataFim[11];
} HistItem;

typedef struct VetHist {
    HistItem *itens;
    int qtd;
    int cap;
} VetHist;

typedef struct Apresentador {
    char nome[TXT_GRD];
    char categoria[TXT_GRD];
    char streamAtual[TXT_GRD];
    VetHist historico;
    struct Apresentador *ant;
    struct Apresentador *prox;
} Apresentador;

Apresentador* apr_criar(const char *nome, const char *categoria, const char *streamAtual);
void apr_inserir_ordenado(Apresentador **cabeca, const char *nome,
                          const char *categoria, const char *streamAtual, int *inseriu);
int  apr_existe_nome(Apresentador *cabeca, const char *nome);
int  apr_pode_apresentar(Apresentador *cabeca, const char *nomeApr,
                         const char *categoriaNec, const char *streamNec);
void apr_listar_da_stream(Apresentador *cabeca, const char *nomeStream);
void apr_listar_da_categoria(Apresentador *cabeca, const char *nomeCategoria);
void apr_listar_todos(Apresentador *cabeca);

/* NOVO: lista apenas os elegíveis p/ (stream,categoria); retorna quantidade */
int  apr_enumerar_elegiveis(Apresentador *cabeca, const char *nomeCategoria,
                             const char *nomeStream, Apresentador **vet, int max);

#endif
