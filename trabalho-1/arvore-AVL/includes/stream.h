#ifndef STREAM_H
#define STREAM_H

#include "tipos.h"        // tamanhos de texto e enums (TipoCategoria, TipoDemanda)
#include "categoria.h"    // definição de Categoria (lista circular de categorias)

/* Estrutura de uma Stream na árvore AVL */
typedef struct Stream {
    char nome[TXT_GRD];       // nome da stream (usado para ordenar a árvore)
    char site[TXT_GRD];       // URL da stream
    Categoria *categorias;    // lista circular com as categorias desta stream
    struct Stream *esq;       // filho esquerdo: streams com nome menor
    struct Stream *dir;       // filho direito: streams com nome maior
    int altura;               // altura do nó para AVL
} Stream;

// cria stream
Stream* stream_criar(const char *nome, const char *site); 

// busca stream
Stream* stream_buscar(Stream *raiz, const char *nome);

// Adiciona uma nova stream na árvore AVL 
Stream* stream_inserir(Stream *raiz, const char *nome, const char *site, int *inseriu);

/* Funções para gerenciar categorias */
void stream_adicionar_categoria(Stream *raiz, const char *nomeStream,
                                const char *nomeCat, TipoCategoria tipo, int *inseriu);
// Adiciona uma categoria à lista circular de uma stream específica

/* Funções auxiliares */
int stream_enumerar(Stream *raiz, Stream **vet, int max);
// Lista todas as streams em ordem alfabética no vetor

void stream_imprimir_inorder(Stream *raiz);

/* Funções auxiliares para AVL */
int stream_altura(Stream *no);
// Retorna a altura de um nó (0 para NULL)

int stream_fator_balanceamento(Stream *no);
// Retorna o fator de balanceamento de um nó

Stream* stream_rotacao_direita(Stream *y);
// Rotação simples à direita

Stream* stream_rotacao_esquerda(Stream *x);
// Rotação simples à esquerda

Stream* stream_rotacao_dupla_direita(Stream *z);
// Rotação dupla à direita (esquerda-direita)

Stream* stream_rotacao_dupla_esquerda(Stream *z);
// Rotação dupla à esquerda (direita-esquerda)

Stream* stream_balancear(Stream *no);
// Balanceia um nó aplicando as rotações necessárias

#endif
