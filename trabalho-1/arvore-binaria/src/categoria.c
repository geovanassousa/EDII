#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/categoria.h"

Categoria* cat_criar(const char *nome, TipoCategoria tipo) {
    Categoria *c = (Categoria*) malloc(sizeof(Categoria));
    if (c != NULL) {
        strncpy(c->nome, nome, sizeof(c->nome)-1);
        c->nome[sizeof(c->nome)-1] = '\0';
        c->tipo = tipo;
        c->raizProgramas = NULL;
        c->prox = NULL; /* ao inserir, vamos fechar o ciclo (lista circular) */
    }
    return c;
}
