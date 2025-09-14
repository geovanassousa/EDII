#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/categoria.h"
#include "../includes/utils.h"

Categoria* cat_criar(const char *nome, TipoCategoria tipo) {
    Categoria *c = (Categoria*) malloc(sizeof(Categoria));
    if (c != NULL) {
        strncpy(c->nome, nome, sizeof(c->nome)-1);
        c->nome[sizeof(c->nome)-1] = '\0';
        c->tipo = tipo;
        c->raizProgramas = NULL;
        c->prox = NULL;
    }
    return c;
}

int cat_existe(Categoria *cabeca, const char *nome) {
    Categoria *p;
    int achou = 0;
    if (cabeca != NULL) {
        p = cabeca;
        do {
            if (achou == 0) {
                if (str_cmp_i(p->nome, nome) == 0) {
                    achou = 1;
                }
            }
            p = p->prox;
        } while (p != cabeca && achou == 0);
    }
    return achou;
}

void cat_inserir_ordenado(Categoria **cabeca, const char *nome, TipoCategoria tipo, int *inseriu) {
    Categoria *novo;
    int cmp;
    Categoria *p;
    int acabou;

    if (inseriu != NULL) { *inseriu = 0; }
    if (cabeca == NULL) { return; }

    if (*cabeca == NULL) {
        novo = cat_criar(nome, tipo);
        if (novo != NULL) {
            novo->prox = novo;
            *cabeca = novo;
            if (inseriu != NULL) { *inseriu = 1; }
        }
        return;
    }

    if (cat_existe(*cabeca, nome) == 1) {
        return;
    }

    cmp = str_cmp_i(nome, (*cabeca)->nome);
    if (cmp < 0) {
        novo = cat_criar(nome, tipo);
        if (novo != NULL) {
            p = *cabeca;
            acabou = 0;
            while (acabou == 0) {
                if (p->prox == *cabeca) {
                    acabou = 1;
                } else {
                    p = p->prox;
                }
            }
            novo->prox = *cabeca;
            p->prox = novo;
            *cabeca = novo;
            if (inseriu != NULL) { *inseriu = 1; }
        }
        return;
    }

    p = *cabeca;
    acabou = 0;
    while (acabou == 0) {
        if (p->prox == *cabeca) {
            acabou = 1;
        } else {
            cmp = str_cmp_i(nome, p->prox->nome);
            if (cmp < 0) {
                acabou = 1;
            } else {
                p = p->prox;
            }
        }
    }
    novo = cat_criar(nome, tipo);
    if (novo != NULL) {
        novo->prox = p->prox;
        p->prox = novo;
        if (inseriu != NULL) { *inseriu = 1; }
    }
}

void cat_listar(Categoria *cabeca) {
    Categoria *p;
    if (cabeca == NULL) {
        printf("(sem categorias)\n");
    } else {
        p = cabeca;
        do {
            printf("- %s (tipo %d)\n", p->nome, (int)p->tipo);
            p = p->prox;
        } while (p != cabeca);
    }
}

/* NOVO */
int cat_enumerar(Categoria *cabeca, Categoria **vet, int max) {
    Categoria *p;
    int qtd = 0;
    if (cabeca != NULL && vet != NULL && max > 0) {
        p = cabeca;
        do {
            if (qtd < max) {
                vet[qtd] = p;
                qtd = qtd + 1;
            }
            p = p->prox;
        } while (p != cabeca && qtd < max);
    }
    return qtd;
}
