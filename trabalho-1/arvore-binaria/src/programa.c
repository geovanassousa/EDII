#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/programa.h"
#include "../includes/utils.h"

static void _dia_txt(int d, char *t, int n) {
    if (d == 1) strncpy(t, "Dom", n-1);
    else if (d == 2) strncpy(t, "Seg", n-1);
    else if (d == 3) strncpy(t, "Ter", n-1);
    else if (d == 4) strncpy(t, "Qua", n-1);
    else if (d == 5) strncpy(t, "Qui", n-1);
    else if (d == 6) strncpy(t, "Sex", n-1);
    else if (d == 7) strncpy(t, "Sab", n-1);
    else strncpy(t, "Diario", n-1); /* 0 = qualquer dia */
    t[n-1] = '\0';
}

Programa* prog_criar(const char *nome, const char *period, int tempoMin,
                     const char *hhmm, int diaSemana, TipoDemanda demanda, const char *apres) {
    Programa *p = (Programa*) malloc(sizeof(Programa));
    if (p != NULL) {
        strncpy(p->nome, nome, sizeof(p->nome)-1); p->nome[sizeof(p->nome)-1] = '\0';
        strncpy(p->periodicidade, period, sizeof(p->periodicidade)-1);
        p->periodicidade[sizeof(p->periodicidade)-1] = '\0';
        p->tempoMin = tempoMin;
        strncpy(p->horarioInicio, hhmm, sizeof(p->horarioInicio)-1);
        p->horarioInicio[sizeof(p->horarioInicio)-1] = '\0';
        p->diaSemana = diaSemana;
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
                       const char *hhmm, int diaSemana, TipoDemanda demanda, const char *apres, int *inseriu) {
    int cmp;
    if (inseriu != NULL && *inseriu != 0) { /* nada */ }
    if (raiz == NULL) {
        Programa *novo = prog_criar(nome, period, tempoMin, hhmm, diaSemana, demanda, apres);
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
        raiz->esq = prog_inserir(raiz->esq, nome, period, tempoMin, hhmm, diaSemana, demanda, apres, inseriu);
    } else {
        raiz->dir = prog_inserir(raiz->dir, nome, period, tempoMin, hhmm, diaSemana, demanda, apres, inseriu);
    }
    return raiz;
}

static void _prog_print(const Programa *p) {
    if (p != NULL) {
        if (p->diaSemana == 0) {
            /* Diario: nao mostra '(Dia)' */
            printf("- %s | %s | %d min | %s | %s | apres: %s\n",
                   p->nome, p->periodicidade, p->tempoMin, p->horarioInicio,
                   (p->demanda == DEMANDA_AO_VIVO ? "Ao Vivo" : "Sob Demanda"),
                   p->apresentador);
        } else {
            char d[16];
            _dia_txt(p->diaSemana, d, sizeof(d));
            printf("- %s | %s | %d min | %s (%s) | %s | apres: %s\n",
                   p->nome, p->periodicidade, p->tempoMin, p->horarioInicio, d,
                   (p->demanda == DEMANDA_AO_VIVO ? "Ao Vivo" : "Sob Demanda"),
                   p->apresentador);
        }
    }
}


void prog_imprimir_inorder(Programa *raiz) {
    if (raiz != NULL) {
        prog_imprimir_inorder(raiz->esq);
        _prog_print(raiz);
        prog_imprimir_inorder(raiz->dir);
    }
}
