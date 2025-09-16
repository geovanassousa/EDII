#ifndef APRESENTADOR_H
#define APRESENTADOR_H

#include "tipos.h"   // tamanhos de texto e enums (TipoCategoria, TipoDemanda)

/* estrutura para histórico de streams que o apresentador já trabalhou */
typedef struct StreamHistorico {
    char nomeStream[TXT_GRD];     // nome da stream
    char dataInicio[11];          // data de início "DD/MM/AAAA"
    char dataTermino[11];         // data de término "DD/MM/AAAA"
    struct StreamHistorico *prox; // próximo histórico
} StreamHistorico;

/* lista duplamente encadeada de apresentadores (ordenada por nome) */
typedef struct Apresentador {
    char nome[TXT_GRD];
    char categoria[TXT_GRD];     // categoria que trabalha (texto)
    char streamAtual[TXT_GRD];   // stream atual (deve existir)
    StreamHistorico *historicoStreams; // lista de streams que já trabalhou
    struct Apresentador *ant;
    struct Apresentador *prox;
} Apresentador;

/* operações */
Apresentador* apr_criar(const char *nome, const char *categoria, const char *streamAtual);
// Cria um novo nó de apresentador com os dados fornecidos 

void apr_inserir_ordenado(Apresentador **cabeca, const char *nome,
                          const char *categoria, const char *streamAtual, int *inseriu);
// Insere apresentador na lista em ordem alfabética; evita duplicação de nomes 

int  apr_existe_nome(Apresentador *cabeca, const char *nome);
// Verifica se já existe apresentador com o nome fornecido

int  apr_pode_apresentar(Apresentador *cabeca, const char *nomeApr,
                         const char *categoriaNec, const char *streamNec);
// Verifica compatibilidade: nome existe, mesma categoria, trabalha na stream

void apr_listar_da_stream(Apresentador *cabeca, const char *nomeStream);
// Lista apresentadores que trabalham na stream especificada

void apr_listar_da_categoria(Apresentador *cabeca, const char *nomeCategoria);
// Lista apresentadores que trabalham na categoria especificada


// Enumera apresentadores elegíveis para categoria e stream específicos; retorna quantidade coletada
int  apr_enumerar_elegiveis(Apresentador *cabeca, const char *nomeCategoria,
                            const char *nomeStream, Apresentador **vet, int max);

// Funções para gerenciar histórico de streams
void apr_adicionar_historico(Apresentador *apr, const char *nomeStream, 
                             const char *dataInicio, const char *dataTermino);
void apr_listar_historico(Apresentador *apr);

#endif
