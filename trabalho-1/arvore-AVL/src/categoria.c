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
    int encontrado = 0; 
    if (cabeca != NULL) {
        p = cabeca;
        do {
            if (encontrado == 0) {
                if (str_cmp_i(p->nome, nome) == 0) {
                    encontrado = 1;
                }
            }
            p = p->prox;
        } while (p != cabeca && encontrado == 0);
    }
    return encontrado;
}

/* adiciona categoria na lista circular em ordem alfabetica */
void cat_inserir_ordenado(Categoria **cabeca, const char *nome, TipoCategoria tipo, int *inseriu) {
    Categoria *novo;
    int cmp;
    Categoria *p;
    int chegouNoFim;

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
            chegouNoFim = 0;
            while (chegouNoFim == 0) {
                if (p->prox == *cabeca) {
                    chegouNoFim = 1;
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
    chegouNoFim = 0;
    while (chegouNoFim == 0) {
        if (p->prox == *cabeca) {
            chegouNoFim = 1;
        } else {
            cmp = str_cmp_i(nome, p->prox->nome);
            if (cmp < 0) {
                chegouNoFim = 1;
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


/* remove categoria apenas se vazia */
void cat_remover_se_vazia(Categoria **cabeca, const char *nome, int *removeu) {
    Categoria *head, *cur, *prev, *tail;
    int done, found, podeRemover, la;

    if (removeu != NULL) { *removeu = 0; }
    if (cabeca == NULL) { return; }

    head = *cabeca;
    if (head == NULL) { return; }

    prev = NULL;
    cur = head;
    done = 0;
    found = 0;

    /* percorre ate voltar no head */
    while (done == 0) {
        if (str_cmp_i(cur->nome, nome) == 0) {
            found = 1;
            done = 1;
        } else {
            prev = cur;
            cur = cur->prox;  /* campo prox: lista circular simples */
            if (cur == head) {
                done = 1;
            }
        }
    }

    if (found == 1) {
        if (cur->raizProgramas == NULL) {
            podeRemover = 1;
        } else {
            podeRemover = 0;
        }

        if (podeRemover == 1) {
            /* caso 1: lista com 1 no */
            if (cur->prox == cur) {
                *cabeca = NULL;
            } else if (prev == NULL) {
                /* removendo o head; achar o tail e religar */
                tail = cur;
                la = 0;
                while (la == 0) {
                    if (tail->prox == cur) {
                        la = 1;
                    } else {
                        tail = tail->prox;
                    }
                }
                *cabeca = cur->prox;
                tail->prox = *cabeca;
            } else {
                /* removendo no do meio/final */
                prev->prox = cur->prox;
                if (cur == *cabeca) {
                    *cabeca = cur->prox;
                }
            }
            free(cur);
            if (removeu != NULL) { *removeu = 1; }
        } else {
            if (removeu != NULL) { *removeu = 0; }
        }
    }
}

