#ifndef CATEGORIA_H
#define CATEGORIA_H

#include "tipos.h"     // tamanhos de texto e enums (TipoCategoria, TipoDemanda)
#include "programa.h"  // definição de Programa (ABB por nome)

/* Estrutura de uma Categoria na lista circular */
typedef struct Categoria {
    char nome[TXT_GRD];          // nome da categoria 
    TipoCategoria tipo;          // Notícias, Esporte ou Entretenimento
    Programa *raizProgramas;     // árvore binária com os programas desta categoria
    struct Categoria *prox;      // próximo nó da lista circular
} Categoria;

/* Funções para gerenciar categorias */
Categoria* cat_criar(const char *nome, TipoCategoria tipo); 
// Cria uma nova categoria

int  cat_existe(Categoria *cabeca, const char *nome);
// Verifica se já existe uma categoria com esse nome

void cat_inserir_ordenado(Categoria **cabeca, const char *nome, TipoCategoria tipo, int *inseriu);
// Adiciona categoria em ordem alfabética (evita duplicação de nomes)


void cat_remover_se_vazia(Categoria **cabeca, const char *nome, int *removeu);
// Remove categoria apenas se não possuir programas cadastrados

/* Funções auxiliares */
int  cat_enumerar(Categoria *cabeca, Categoria **vet, int max);
// Lista todas as categorias em ordem no vetor

#endif
