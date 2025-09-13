#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/stream.h"

Stream* stream_criar(const char *nome, const char *site) {
    Stream *s = (Stream*) malloc(sizeof(Stream));
    if (s != NULL) {
        strncpy(s->nome, nome, sizeof(s->nome)-1); s->nome[sizeof(s->nome)-1] = '\0';
        strncpy(s->site, site, sizeof(s->site)-1); s->site[sizeof(s->site)-1] = '\0';
        s->categorias = NULL; /* nenhuma categoria cadastrada ainda */
        s->esq = NULL;
        s->dir = NULL;
    }
    return s;
}
