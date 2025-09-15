#ifndef APRESENTADOR_H
#define APRESENTADOR_H

#include "tipos.h"   // tamanhos de texto e enums (TipoCategoria, TipoDemanda)

/* item de histórico: onde trabalhou e período */
typedef struct HistItem {
    char stream[TXT_GRD];
    char dataIni[11];  // "DD/MM/AAAA"
    char dataFim[11];  // "DD/MM/AAAA"
} HistItem;

/* vetor dinâmico de histórico (ordenado por nome da stream) */
typedef struct VetHist {
    HistItem *itens;
    int qtd;
    int cap;
} VetHist;

/* lista duplamente encadeada de apresentadores (ordenada por nome) */
typedef struct Apresentador {
    char nome[TXT_GRD];
    char categoria[TXT_GRD];     // categoria que trabalha (texto)
    char streamAtual[TXT_GRD];   // stream atual (deve existir)
    VetHist historico;           // streams já trabalhadas (vetor ordenado)
    struct Apresentador *ant;
    struct Apresentador *prox;
} Apresentador;

/* operações */
Apresentador* apr_criar(const char *nome, const char *categoria, const char *streamAtual);
// cria nó; 

void apr_inserir_ordenado(Apresentador **cabeca, const char *nome,
                          const char *categoria, const char *streamAtual, int *inseriu);
// insere na lista em ordem alfabética; sem duplicar nome 

int  apr_existe_nome(Apresentador *cabeca, const char *nome);
// verifica se já existe apresentador com esse nome

int  apr_pode_apresentar(Apresentador *cabeca, const char *nomeApr,
                         const char *categoriaNec, const char *streamNec);
// checa compatibilidade: nome existe, mesma categoria, trabalha na stream

void apr_listar_da_stream(Apresentador *cabeca, const char *nomeStream);
// lista apresentadores cuja streamAtual == nomeStream

void apr_listar_da_categoria(Apresentador *cabeca, const char *nomeCategoria);
// lista apresentadores cuja categoria == nomeCategoria

void apr_listar_todos(Apresentador *cabeca);
// lista toda a lista em ordem

/* elegíveis para (categoria, stream); retorna quantidade coletada */
int  apr_enumerar_elegiveis(Apresentador *cabeca, const char *nomeCategoria,
                            const char *nomeStream, Apresentador **vet, int max);

#endif
