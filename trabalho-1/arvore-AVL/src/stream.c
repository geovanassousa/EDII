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
        s->altura = 1;  // novo nó tem altura 1
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
        return raiz;  // elemento já existe
    } else if (cmp < 0) {
        raiz->esq = stream_inserir(raiz->esq, nome, site, inseriu);
    } else {
        raiz->dir = stream_inserir(raiz->dir, nome, site, inseriu);
    }
    
    // Balanceia a árvore após a inserção
    return stream_balancear(raiz);
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

/* ======== categorias dentro da stream ======== */

void stream_adicionar_categoria(Stream *raiz, const char *nomeStream,
                                const char *nomeCat, TipoCategoria tipo, int *inseriu) {
    Stream *s;
    if (inseriu != NULL) { *inseriu = 0; }
    s = stream_buscar(raiz, nomeStream);
    if (s != NULL) {
        /* chama a função de inserir categoria */
        cat_inserir_ordenado(&(s->categorias), nomeCat, tipo, inseriu);
    }
}

/* pega as streams em ordem */
static void enumerar_em_ordem(Stream *raiz, Stream **vetor, int maximo, int *quantidade) {
    if (raiz != NULL && *quantidade < maximo) {
        enumerar_em_ordem(raiz->esq, vetor, maximo, quantidade);
        if (*quantidade < maximo) {
            vetor[*quantidade] = raiz;
            *quantidade = *quantidade + 1;
        }
        if (*quantidade < maximo) {
            enumerar_em_ordem(raiz->dir, vetor, maximo, quantidade);
        }
    }
}

int stream_enumerar(Stream *raiz, Stream **vet, int max) {
    int qtd = 0;
    enumerar_em_ordem(raiz, vet, max, &qtd);
    return qtd;
}

/* ========== FUNÇÕES AUXILIARES AVL ========== */

int stream_altura(Stream *no) {
    if (no == NULL) return 0;
    return no->altura;
}

int stream_fator_balanceamento(Stream *no) {
    if (no == NULL) return 0;
    return stream_altura(no->esq) - stream_altura(no->dir);
}

int max_stream(int a, int b) {
    return (a > b) ? a : b;
}

Stream* stream_rotacao_direita(Stream *y) {
    Stream *x = y->esq;
    Stream *T2 = x->dir;
    
    // Realiza a rotação
    x->dir = y;
    y->esq = T2;
    
    // Atualiza alturas
    y->altura = max_stream(stream_altura(y->esq), stream_altura(y->dir)) + 1;
    x->altura = max_stream(stream_altura(x->esq), stream_altura(x->dir)) + 1;
    
    // Retorna nova raiz
    return x;
}

Stream* stream_rotacao_esquerda(Stream *x) {
    Stream *y = x->dir;
    Stream *T2 = y->esq;
    
    // faz a rotação
    y->esq = x;
    x->dir = T2;
    
    // Atualiza alturas
    x->altura = max_stream(stream_altura(x->esq), stream_altura(x->dir)) + 1;
    y->altura = max_stream(stream_altura(y->esq), stream_altura(y->dir)) + 1;
    
    // Retorna nova raiz
    return y;
}

Stream* stream_rotacao_dupla_direita(Stream *z) {
    z->esq = stream_rotacao_esquerda(z->esq);
    return stream_rotacao_direita(z);
}

Stream* stream_rotacao_dupla_esquerda(Stream *z) {
    z->dir = stream_rotacao_direita(z->dir);
    return stream_rotacao_esquerda(z);
}

Stream* stream_balancear(Stream *no) {
    if (no == NULL) return no;
    
    // Atualiza altura do nó atual
    no->altura = max_stream(stream_altura(no->esq), stream_altura(no->dir)) + 1;
    
    // Calcula fator de balanceamento
    int fb = stream_fator_balanceamento(no);
    
    // Caso esquerda-esquerda
    if (fb > 1 && stream_fator_balanceamento(no->esq) >= 0) {
        return stream_rotacao_direita(no);
    }
    
    // Caso esquerda-direita
    if (fb > 1 && stream_fator_balanceamento(no->esq) < 0) {
        return stream_rotacao_dupla_direita(no);
    }
    
    // Caso direita-direita
    if (fb < -1 && stream_fator_balanceamento(no->dir) <= 0) {
        return stream_rotacao_esquerda(no);
    }
    
    // Caso direita-esquerda
    if (fb < -1 && stream_fator_balanceamento(no->dir) > 0) {
        return stream_rotacao_dupla_esquerda(no);
    }
    
    return no;
}
