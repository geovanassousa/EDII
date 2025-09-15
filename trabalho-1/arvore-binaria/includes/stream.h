#ifndef STREAM_H
#define STREAM_H

#include "tipos.h"        // tamanhos de texto e enums (TipoCategoria, TipoDemanda)
#include "categoria.h"    // definição de Categoria (lista circular de categorias)

/* Nó da ABB de Streams (ordenada por nome) */
typedef struct Stream {
    char nome[TXT_GRD];       // nome da stream (chave da ABB)
    char site[TXT_GRD];       // URL da stream
    Categoria *categorias;    //  lista circular de categorias dessa stream
    struct Stream *esq;       // filho esquerdo: nomes menores
    struct Stream *dir;       // filho direito: nomes maiores
} Stream;

// cria um nó da ABB de streams
Stream* stream_criar(const char *nome, const char *site); 

// busca pela chave 'nome'
Stream* stream_buscar(Stream *raiz, const char *nome);

// insere uma nova stream (sem duplicar); retorna a nova raiz
Stream* stream_inserir(Stream *raiz, const char *nome, const char *site, int *inseriu);

/* Categorias de uma stream */
void stream_adicionar_categoria(Stream *raiz, const char *nomeStream,
                                const char *nomeCat, TipoCategoria tipo, int *inseriu);
// procura a stream na ABB e insere categoria na lista circular (sem repetir)

void stream_listar_categorias(Stream *raiz, const char *nomeStream);
// percorre e mostra a lista circular de categorias da stream

/* Utilitário */
int stream_enumerar(Stream *raiz, Stream **vet, int max);
// percorre em-ordem; coloca as streams no vetor até 'max'; retorna quantidade

#endif
