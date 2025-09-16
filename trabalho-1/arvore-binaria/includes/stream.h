#ifndef STREAM_H
#define STREAM_H

#include "tipos.h"        // tamanhos de texto e enums (TipoCategoria, TipoDemanda)
#include "categoria.h"    // definição de Categoria (lista circular de categorias)

/* Estrutura de uma Stream na árvore binária */
typedef struct Stream {
    char nome[TXT_GRD];       // nome da stream (usado para ordenar a árvore)
    char site[TXT_GRD];       // URL da stream
    Categoria *categorias;    // lista circular com as categorias desta stream
    struct Stream *esq;       // filho esquerdo: streams com nome menor
    struct Stream *dir;       // filho direito: streams com nome maior
} Stream;

// Cria uma nova stream com os dados fornecidos
Stream* stream_criar(const char *nome, const char *site); 

// Procura uma stream pelo nome
Stream* stream_buscar(Stream *raiz, const char *nome);

// Adiciona uma nova stream na árvore (evita duplicação de nomes)
Stream* stream_inserir(Stream *raiz, const char *nome, const char *site, int *inseriu);

/* Funções para gerenciar categorias */
void stream_adicionar_categoria(Stream *raiz, const char *nomeStream,
                                const char *nomeCat, TipoCategoria tipo, int *inseriu);
// Adiciona uma categoria à lista circular de uma stream específica

/* Funções auxiliares */
int stream_enumerar(Stream *raiz, Stream **vet, int max);
// Lista todas as streams em ordem alfabética no vetor

void stream_imprimir_inorder(Stream *raiz);

#endif
