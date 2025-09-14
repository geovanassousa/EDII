#ifndef STREAM_H
#define STREAM_H

#include "tipos.h"
#include "categoria.h"

typedef struct Stream {
    char nome[TXT_GRD];
    char site[TXT_GRD];
    Categoria *categorias;   /* cabeça da lista circular (pode ser NULL) */
    struct Stream *esq;      /* BST por nome da stream */
    struct Stream *dir;
} Stream;

/* Criação e BST */
Stream* stream_criar(const char *nome, const char *site);
Stream* stream_buscar(Stream *raiz, const char *nome);
Stream* stream_inserir(Stream *raiz, const char *nome, const char *site, int *inseriu);
void    stream_imprimir_inorder(Stream *raiz);

/* Categorias dentro da stream */
void stream_adicionar_categoria(Stream *raiz, const char *nomeStream,
                                const char *nomeCat, TipoCategoria tipo, int *inseriu);
void stream_listar_categorias(Stream *raiz, const char *nomeStream);

#endif
