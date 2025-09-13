#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/apresentador.h"

Apresentador* apr_criar(const char *nome, const char *categoria, const char *streamAtual) {
    Apresentador *a = (Apresentador*) malloc(sizeof(Apresentador));
    if (a != NULL) {
        strncpy(a->nome, nome, sizeof(a->nome)-1); a->nome[sizeof(a->nome)-1] = '\0';
        strncpy(a->categoria, categoria, sizeof(a->categoria)-1); a->categoria[sizeof(a->categoria)-1] = '\0';
        strncpy(a->streamAtual, streamAtual, sizeof(a->streamAtual)-1); a->streamAtual[sizeof(a->streamAtual)-1] = '\0';

        a->historico.itens = NULL;
        a->historico.qtd = 0;
        a->historico.cap = 0;

        a->ant = NULL;
        a->prox = NULL;
    }
    return a;
}
