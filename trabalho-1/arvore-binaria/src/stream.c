#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/stream.h"
#include "../includes/utils.h"

Stream* stream_criar(const char *nome, const char *site) {
    Stream *s = (Stream*) malloc(sizeof(Stream));
    if (s != NULL) {
        strncpy(s->nome, nome, sizeof(s->nome)-1); s->nome[sizeof(s->nome)-1] = '\0';
        strncpy(s->site, site, sizeof(s->site)-1); s->site[sizeof(s->site)-1] = '\0';
        s->categorias = NULL;
        s->esq = NULL;
        s->dir = NULL;
    }
    return s;
}

Stream* stream_buscar(Stream *raiz, const char *nome) {
    int cmp;
    if (raiz == NULL) {
        return NULL;
    }
    cmp = str_cmp_i(nome, raiz->nome);
    if (cmp == 0) {
        return raiz;
    }
    if (cmp < 0) {
        return stream_buscar(raiz->esq, nome);
    }
    return stream_buscar(raiz->dir, nome);
}

Stream* stream_inserir(Stream *raiz, const char *nome, const char *site, int *inseriu) {
    int cmp;
    if (inseriu != NULL && *inseriu != 0) {
        /* nada */
    }
    if (raiz == NULL) {
        Stream *novo = stream_criar(nome, site);
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
        raiz->esq = stream_inserir(raiz->esq, nome, site, inseriu);
    } else {
        raiz->dir = stream_inserir(raiz->dir, nome, site, inseriu);
    }
    return raiz;
}

static void stream_imprimir(Stream *s) {
    if (s != NULL) {
        printf("- %s (%s)\n", s->nome, s->site);
    }
}

void stream_imprimir_inorder(Stream *raiz) {
    if (raiz != NULL) {
        stream_imprimir_inorder(raiz->esq);
        stream_imprimir(raiz);
        stream_imprimir_inorder(raiz->dir);
    }
}

/* ======== NOVO: categorias dentro da stream ======== */

void stream_adicionar_categoria(Stream *raiz, const char *nomeStream,
                                const char *nomeCat, TipoCategoria tipo, int *inseriu) {
    Stream *s;
    if (inseriu != NULL) { *inseriu = 0; }
    s = stream_buscar(raiz, nomeStream);
    if (s != NULL) {
        cat_inserir_ordenado(&(s->categorias), nomeCat, tipo, inseriu);
    }
}

void stream_listar_categorias(Stream *raiz, const char *nomeStream) {
    Stream *s = stream_buscar(raiz, nomeStream);
    if (s == NULL) {
        printf("Stream nao encontrada.\n");
    } else {
        cat_listar(s->categorias);
    }
}
