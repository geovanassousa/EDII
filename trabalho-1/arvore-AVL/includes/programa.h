#ifndef PROGRAMA_H
#define PROGRAMA_H

#include "tipos.h"       // tamanhos de texto e enums (TipoCategoria, TipoDemanda)

/* Nó da árvore de Programas (ordenada por nome do programa) */
typedef struct Programa {
    char nome[TXT_GRD];            // chave da árvore AVL
    char periodicidade[TXT_PEQ];   // "Diario", "Semanal", "Mensal"
    int  tempoMin;                 // duração em minutos
    char horarioInicio[6];         // "HH:MM" 
    int  diaSemana;                // 1=Dom .. 7=Sab; 0=qualquer dia (Diario)
    TipoDemanda demanda;           // Ao vivo / Sob Demanda
    char apresentador[TXT_GRD];    // nome do apresentador

    struct Programa *esq;          // nomes menores
    struct Programa *dir;          // nomes maiores
    int altura;                    // altura do nó para AVL
} Programa;

/* Operações na AVL de Programas */
Programa* prog_criar(const char *nome, const char *period, int tempoMin,
                     const char *hhmm, int diaSemana, TipoDemanda demanda, const char *apres);
// Cria um novo nó de programa com os dados fornecidos

Programa* prog_buscar(Programa *raiz, const char *nome);
// Busca programa por nome

Programa* prog_inserir(Programa *raiz, const char *nome, const char *period, int tempoMin,
                       const char *hhmm, int diaSemana, TipoDemanda demanda, const char *apres, int *inseriu);
// Insere programa na AVL

Programa* prog_remover(Programa *raiz, const char *nome, int *removeu);
// remove programa

void prog_imprimir_inorder(Programa *raiz);
// Percorre a árvore em ordem (listagem alfabética por nome)

/* Funções auxiliares para AVL */
int prog_altura(Programa *no);
// Retorna a altura de um nó (0 para NULL)

int prog_fator_balanceamento(Programa *no);
// Retorna o fator de balanceamento de um nó

Programa* prog_rotacao_direita(Programa *y);
// Rotação simples à direita

Programa* prog_rotacao_esquerda(Programa *x);
// Rotação simples à esquerda

Programa* prog_rotacao_dupla_direita(Programa *z);
// Rotação dupla à direita (esquerda-direita)

Programa* prog_rotacao_dupla_esquerda(Programa *z);
// Rotação dupla à esquerda (direita-esquerda)

Programa* prog_balancear(Programa *no);
// Balanceia um nó aplicando as rotações necessárias

#endif
