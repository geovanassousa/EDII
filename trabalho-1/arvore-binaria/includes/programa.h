#ifndef PROGRAMA_H
#define PROGRAMA_H

#include "tipos.h"       // tamanhos de texto e enums (TipoCategoria, TipoDemanda)

/* Nó da ABB de Programas (ordenada por nome do programa) */
typedef struct Programa {
    char nome[TXT_GRD];            // chave da ABB
    char periodicidade[TXT_PEQ];   // "Diario", "Semanal", "Mensal"
    int  tempoMin;                 // duração em minutos
    char horarioInicio[6];         // "HH:MM" 
    int  diaSemana;                // 1=Dom .. 7=Sab; 0=qualquer dia (Diario)
    TipoDemanda demanda;           // Ao vivo / Sob Demanda
    char apresentador[TXT_GRD];    // nome do apresentador

    struct Programa *esq;          // nomes menores
    struct Programa *dir;          // nomes maiores
} Programa;

/* Operações na ABB de Programas */
Programa* prog_criar(const char *nome, const char *period, int tempoMin,
                     const char *hhmm, int diaSemana, TipoDemanda demanda, const char *apres);
// cria nó: filhos NULL; campos copiados

Programa* prog_buscar(Programa *raiz, const char *nome);
// busca por nome; retorna nó ou NULL

Programa* prog_inserir(Programa *raiz, const char *nome, const char *period, int tempoMin,
                       const char *hhmm, int diaSemana, TipoDemanda demanda, const char *apres, int *inseriu);
// insere na ABB; sem duplicar; retorna (possível) nova raiz

Programa* prog_remover(Programa *raiz, const char *nome, int *removeu);
// remove nó por nome; retorna (possível) nova raiz

void prog_imprimir_inorder(Programa *raiz);
// percurso em-ordem (listar em ordem alfabética)

#endif
