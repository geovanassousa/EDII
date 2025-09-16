#ifndef CATEGORIA_H
#define CATEGORIA_H

#include "tipos.h"     // tamanhos de texto e enums (TipoCategoria, TipoDemanda)
#include "programa.h"  // definição de Programa (AVL por nome)

/* Estrutura de uma Categoria na lista circular */
typedef struct Categoria {
    char nome[TXT_GRD];          // nome da categoria 
    TipoCategoria tipo;          // Notícias, Esporte ou Entretenimento
    Programa *raizProgramas;     // árvore AVL com os programas desta categoria
    struct Categoria *prox;      // próximo nó da lista circular
} Categoria;

/* Funções para gerenciar categorias */
Categoria* cat_criar(const char *nome, TipoCategoria tipo); 
// cria categoria

int  cat_existe(Categoria *cabeca, const char *nome);
// checa se existe

void cat_inserir_ordenado(Categoria **cabeca, const char *nome, TipoCategoria tipo, int *inseriu);
// insere ordenado


void cat_remover_se_vazia(Categoria **cabeca, const char *nome, int *removeu);
// Remove categoria apenas se não possuir programas cadastrados

/* Funções auxiliares */
int  cat_enumerar(Categoria *cabeca, Categoria **vet, int max);
// Lista todas as categorias em ordem no vetor

#endif
