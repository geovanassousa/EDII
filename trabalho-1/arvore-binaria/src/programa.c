#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/programa.h"
#include "../includes/utils.h"

Programa* prog_criar(const char *nome, const char *period, int tempoMin,
                     const char *hhmm, TipoDemanda demanda, const char *apres) {
    Programa *p = (Programa*) malloc(sizeof(Programa));
    if (p != NULL) {
        strncpy(p->nome, nome, sizeof(p->nome)-1); p->nome[sizeof(p->nome)-1] = '\0';
        strncpy(p->periodicidade, period, sizeof(p->periodicidade)-1);
        p->periodicidade[sizeof(p->periodicidade)-1] = '\0';
        p->tempoMin = tempoMin;
        strncpy(p->horarioInicio, hhmm, sizeof(p->horarioInicio)-1);
        p->horarioInicio[sizeof(p->horarioInicio)-1] = '\0';
        p->demanda = demanda;
        strncpy(p->apresentador, apres, sizeof(p->apresentador)-1);
        p->apresentador[sizeof(p->apresentador)-1] = '\0';
        p->esq = NULL;
        p->dir = NULL;
    }
    return p;
}

Programa* prog_buscar(Programa *raiz, const char *nome) {
    int cmp;
    if (raiz == NULL) return NULL;
    cmp = str_cmp_i(nome, raiz->nome);
    if (cmp == 0) return raiz;
    if (cmp < 0)  return prog_buscar(raiz->esq, nome);
    return prog_buscar(raiz->dir, nome);
}

Programa* prog_inserir(Programa *raiz, const char *nome, const char *period, int tempoMin,
                       const char *hhmm, TipoDemanda demanda, const char *apres, int *inseriu) {
    int cmp;
    if (inseriu != NULL && *inseriu != 0) { /* nada */ }
    if (raiz == NULL) {
        Programa *novo = prog_criar(nome, period, tempoMin, hhmm, demanda, apres);
        if (novo != NULL) {
            if (inseriu != NULL) { *inseriu = 1; }
            return novo;
        } else {
            if (inseriu != NULL) { *inseriu = 0; }
            return NULL;
        }
    }
    cmp = str_cmp_i(nome, raiz->nome);
    if (cmp == 0) {
        if (inseriu != NULL) { *inseriu = 0; }
    } else if (cmp < 0) {
        raiz->esq = prog_inserir(raiz->esq, nome, period, tempoMin, hhmm, demanda, apres, inseriu);
    } else {
        raiz->dir = prog_inserir(raiz->dir, nome, period, tempoMin, hhmm, demanda, apres, inseriu);
    }
    return raiz;
}

static void _prog_print(const Programa *p) {
    if (p != NULL) {
        printf("- %s | %s | %d min | %s | %s | apres: %s\n",
               p->nome, p->periodicidade, p->tempoMin, p->horarioInicio,
               (p->demanda == DEMANDA_AO_VIVO ? "Ao Vivo" : "Sob Demanda"),
               p->apresentador);
    }
}

void prog_imprimir_inorder(Programa *raiz) {
    if (raiz != NULL) {
        prog_imprimir_inorder(raiz->esq);
        _prog_print(raiz);
        prog_imprimir_inorder(raiz->dir);
    }
}
