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
        c->prox = NULL; /* ao inserir, vamos fechar o ciclo */
    }
    return c;
}

/* percorre a lista circular procurando por nome (case-insensitive) */
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

/* insere mantendo ordem alfabética por nome, sem duplicar */
void cat_inserir_ordenado(Categoria **cabeca, const char *nome, TipoCategoria tipo, int *inseriu) {
    Categoria *novo;
    int cmp;
    Categoria *p;
    int acabou;

    if (inseriu != NULL) { *inseriu = 0; }

    if (cabeca == NULL) {
        return;
    }

    if (*cabeca == NULL) {
        novo = cat_criar(nome, tipo);
        if (novo != NULL) {
            novo->prox = novo;      /* primeiro elemento fecha o ciclo */
            *cabeca = novo;
            if (inseriu != NULL) { *inseriu = 1; }
        }
        return;
    }

    /* se já existe, não insere */
    if (cat_existe(*cabeca, nome) == 1) {
        return;
    }

    /* decidir posição: antes da cabeça ou no meio/fim */
    cmp = str_cmp_i(nome, (*cabeca)->nome);
    if (cmp < 0) {
        /* inserir antes da cabeça: achar o último (que aponta para a cabeça) */
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
            *cabeca = novo; /* nova cabeça */
            if (inseriu != NULL) { *inseriu = 1; }
        }
        return;
    }

    /* inserir depois de algum nó: anda até achar p tal que p->prox é cabeça OU nome < p->prox->nome */
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
