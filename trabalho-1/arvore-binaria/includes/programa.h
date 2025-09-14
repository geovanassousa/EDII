#ifndef PROGRAMA_H
#define PROGRAMA_H

#include "tipos.h"

typedef struct Programa {
    char nome[TXT_GRD];
    char periodicidade[TXT_PEQ];   /* diário, semanal, mensal, ... */
    int  tempoMin;                 /* minutos */
    char horarioInicio[6];         /* "HH:MM" */
    TipoDemanda demanda;           /* ao vivo/sob demanda */
    char apresentador[TXT_GRD];    /* nome do apresentador */

    struct Programa *esq;
    struct Programa *dir;
} Programa;

Programa* prog_criar(const char *nome, const char *period, int tempoMin,
                     const char *hhmm, TipoDemanda demanda, const char *apres);

/* NOVO: operações BST por nome do programa */
Programa* prog_buscar(Programa *raiz, const char *nome);
Programa* prog_inserir(Programa *raiz, const char *nome, const char *period, int tempoMin,
                       const char *hhmm, TipoDemanda demanda, const char *apres, int *inseriu);
void      prog_imprimir_inorder(Programa *raiz);

#endif
