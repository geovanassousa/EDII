#ifndef PROGRAMA_H
#define PROGRAMA_H

#include "tipos.h"

typedef struct Programa {
    char nome[TXT_GRD];
    char periodicidade[TXT_PEQ];   /* diário, semanal, mensal, ... */
    int  tempoMin;                 /* duração em minutos */
    char horarioInicio[6];         /* "HH:MM" */
    TipoDemanda demanda;           /* ao vivo ou sob demanda */
    char apresentador[TXT_GRD];    /* nome do apresentador */

    struct Programa *esq;
    struct Programa *dir;
} Programa;

/* (Implementações virão aos poucos, conforme os itens do enunciado) */
Programa* prog_criar(const char *nome, const char *period, int tempoMin,
                     const char *hhmm, TipoDemanda demanda, const char *apres);

#endif
