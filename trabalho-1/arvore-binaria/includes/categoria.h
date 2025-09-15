#ifndef CATEGORIA_H
#define CATEGORIA_H

#include "tipos.h"     // tamanhos de texto e enums (TipoCategoria, TipoDemanda)
#include "programa.h"  // definição de Programa (ABB por nome)

/* Nó da lista circular de Categorias (ordenada por nome) */
typedef struct Categoria {
    char nome[TXT_GRD];          // nome da categoria 
    TipoCategoria tipo;          // Notícias, Esporte ou Entretenimento
    Programa *raizProgramas;     // ABB de programas desta categoria
    struct Categoria *prox;      // próximo nó (lista circular simples)
} Categoria;

/* Operações na lista circular de categorias */
Categoria* cat_criar(const char *nome, TipoCategoria tipo); 
// cria nó de categoria (raizProgramas=NULL; prox aponta para si ao inserir primeira)

int  cat_existe(Categoria *cabeca, const char *nome);
// verifica se já existe categoria com esse nome na lista circular

void cat_inserir_ordenado(Categoria **cabeca, const char *nome, TipoCategoria tipo, int *inseriu);
// insere em ordem alfabética; sem duplicar 

void cat_listar(Categoria *cabeca);
// percorre uma volta da lista circular e mostra as categorias

void cat_remover_se_vazia(Categoria **cabeca, const char *nome, int *removeu);
// remove apenas se 'raizProgramas == NULL'

/* Utilitário */
int  cat_enumerar(Categoria *cabeca, Categoria **vet, int max);
// coleta ponteiros em ordem (uma volta) até 'max'; retorna quantidade

#endif
