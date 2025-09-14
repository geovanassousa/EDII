#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/apresentador.h"
#include "../includes/utils.h"

Apresentador* apr_criar(const char *nome, const char *categoria, const char *streamAtual) {
    Apresentador *a = (Apresentador*) malloc(sizeof(Apresentador));
    if (a != NULL) {
        strncpy(a->nome, nome, sizeof(a->nome)-1); a->nome[sizeof(a->nome)-1] = '\0';
        strncpy(a->categoria, categoria, sizeof(a->categoria)-1); a->categoria[sizeof(a->categoria)-1] = '\0';
        strncpy(a->streamAtual, streamAtual, sizeof(a->streamAtual)-1); a->streamAtual[sizeof(a->streamAtual)-1] = '\0';
        a->historico.itens = NULL; a->historico.qtd = 0; a->historico.cap = 0;
        a->ant = NULL; a->prox = NULL;
    }
    return a;
}

int apr_existe_nome(Apresentador *cabeca, const char *nome) {
    Apresentador *p = cabeca;
    int achou = 0;
    while (p != NULL && achou == 0) {
        if (str_cmp_i(p->nome, nome) == 0) {
            achou = 1;
        } else {
            p = p->prox;
        }
    }
    return achou;
}

void apr_inserir_ordenado(Apresentador **cabeca, const char *nome,
                          const char *categoria, const char *streamAtual, int *inseriu) {
    Apresentador *novo, *p, *ant;
    int feito, cmp;

    if (inseriu != NULL) { *inseriu = 0; }
    if (cabeca == NULL) { return; }

    if (apr_existe_nome(*cabeca, nome) == 1) {
        return;
    }

    novo = apr_criar(nome, categoria, streamAtual);
    if (novo == NULL) { return; }

    if (*cabeca == NULL) {
        *cabeca = novo;
        if (inseriu != NULL) { *inseriu = 1; }
        return;
    }

    cmp = str_cmp_i(novo->nome, (*cabeca)->nome);
    if (cmp < 0) {
        novo->prox = *cabeca;
        (*cabeca)->ant = novo;
        *cabeca = novo;
        if (inseriu != NULL) { *inseriu = 1; }
        return;
    }

    p = *cabeca;
    ant = NULL;
    feito = 0;
    while (p != NULL && feito == 0) {
        cmp = str_cmp_i(novo->nome, p->nome);
        if (cmp < 0) {
            novo->prox = p;
            novo->ant = p->ant;
            if (p->ant != NULL) { p->ant->prox = novo; }
            p->ant = novo;
            if (p == *cabeca) { *cabeca = novo; }
            feito = 1;
        } else {
            ant = p;
            p = p->prox;
        }
    }
    if (feito == 0) {
        ant->prox = novo;
        novo->ant = ant;
        novo->prox = NULL;
    }
    if (inseriu != NULL) { *inseriu = 1; }
}

int apr_pode_apresentar(Apresentador *cabeca, const char *nomeApr,
                        const char *categoriaNec, const char *streamNec) {
    Apresentador *p = cabeca;
    int ok = 0;
    while (p != NULL && ok == 0) {
        if (str_cmp_i(p->nome, nomeApr) == 0) {
            if (str_cmp_i(p->categoria, categoriaNec) == 0 &&
                str_cmp_i(p->streamAtual, streamNec) == 0) {
                ok = 1;
            }
        } else {
            p = p->prox;
        }
    }
    return ok;
}

void apr_listar_da_stream(Apresentador *cabeca, const char *nomeStream) {
    Apresentador *p = cabeca;
    int vazio = 1;
    while (p != NULL) {
        if (str_cmp_i(p->streamAtual, nomeStream) == 0) {
            printf("- %s (cat: %s, stream atual: %s)\n", p->nome, p->categoria, p->streamAtual);
            vazio = 0;
        }
        p = p->prox;
    }
    if (vazio == 1) {
        printf("(nenhum apresentador nessa stream)\n");
    }
}

void apr_listar_da_categoria(Apresentador *cabeca, const char *nomeCategoria) {
    Apresentador *p = cabeca;
    int vazio = 1;
    while (p != NULL) {
        if (str_cmp_i(p->categoria, nomeCategoria) == 0) {
            printf("- %s (cat: %s, stream atual: %s)\n", p->nome, p->categoria, p->streamAtual);
            vazio = 0;
        }
        p = p->prox;
    }
    if (vazio == 1) {
        printf("(nenhum apresentador nessa categoria)\n");
    }
}

void apr_listar_todos(Apresentador *cabeca) {
    Apresentador *p = cabeca;
    if (p == NULL) {
        printf("(nenhum apresentador)\n");
    } else {
        while (p != NULL) {
            printf("- %s (cat: %s, stream atual: %s)\n", p->nome, p->categoria, p->streamAtual);
            p = p->prox;
        }
    }
}

/* NOVO: enumera elegíveis por (categoria, stream) */
int apr_enumerar_elegiveis(Apresentador *cabeca, const char *nomeCategoria,
                           const char *nomeStream, Apresentador **vet, int max) {
    Apresentador *p = cabeca;
    int qtd = 0;
    while (p != NULL) {
        if (str_cmp_i(p->categoria, nomeCategoria) == 0 &&
            str_cmp_i(p->streamAtual, nomeStream) == 0) {
            if (qtd < max) {
                vet[qtd] = p;
                qtd = qtd + 1;
            }
        }
        p = p->prox;
    }
    return qtd;
}
