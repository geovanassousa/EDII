#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include "../includes/tipos.h"
#include "../includes/stream.h"
#include "../includes/utils.h"

/* raiz da BST de streams */
static Stream *RAIZ = NULL;

static void menu_mostrar(void) {
    printf("\n=== MENU Q1 (Streams) ===\n");
    printf("1) Cadastrar Stream\n");
    printf("2) Cadastrar Categoria em uma Stream\n");
    printf("5) Mostrar todas as streams cadastradas\n");
    printf("6) Mostrar categorias de uma Stream\n");
    printf("0) Sair\n");
    printf("Opcao: ");
}

static void acao_cadastrar_stream(void) {
    char nome[TXT_GRD];
    char site[TXT_GRD];
    int inseriu = 0;

    printf("Nome da stream: ");
    ler_linha(nome, sizeof(nome));
    printf("Site da stream: ");
    ler_linha(site, sizeof(site));

    if (stream_buscar(RAIZ, nome) != NULL) {
        printf("Ja existe uma stream com esse nome.\n");
    } else {
        RAIZ = stream_inserir(RAIZ, nome, site, &inseriu);
        if (inseriu == 1) {
            printf("Stream cadastrada.\n");
        } else {
            printf("Nao foi possivel cadastrar.\n");
        }
    }
}

static void acao_listar_streams(void) {
    if (RAIZ == NULL) {
        printf("(nenhuma stream cadastrada)\n");
    } else {
        stream_imprimir_inorder(RAIZ);
    }
}

static void acao_cadastrar_categoria(void) {
    char nomeStream[TXT_GRD];
    char nomeCat[TXT_GRD];
    char linha[16];
    int tipoNum;
    int inseriu = 0;

    printf("Stream (nome exato): ");
    ler_linha(nomeStream, sizeof(nomeStream));
    if (stream_buscar(RAIZ, nomeStream) == NULL) {
        printf("Stream nao encontrada. Cadastre a stream primeiro.\n");
    } else {
        printf("Nome da categoria: ");
        ler_linha(nomeCat, sizeof(nomeCat));

        printf("Tipo (1=Noticias, 2=Esporte, 3=Entretenimento): ");
        ler_linha(linha, sizeof(linha));
        tipoNum = parse_categoria_num(atoi(linha));
        if (tipoNum == 0) {
            printf("Tipo invalido.\n");
        } else {
            stream_adicionar_categoria(RAIZ, nomeStream, nomeCat, (TipoCategoria)tipoNum, &inseriu);
            if (inseriu == 1) {
                printf("Categoria cadastrada.\n");
            } else {
                printf("Nao foi possivel cadastrar (duplicada ou erro).\n");
            }
        }
    }
}

static void acao_listar_categorias(void) {
    char nomeStream[TXT_GRD];
    printf("Stream (nome exato): ");
    ler_linha(nomeStream, sizeof(nomeStream));
    stream_listar_categorias(RAIZ, nomeStream);
}

int main(void) {
    int sair = 0;
    char linha[16];

    printf("Trabalho ED2 - Questao 1 (Streams + Categorias)\n");

    while (sair == 0) {
        menu_mostrar();
        ler_linha(linha, sizeof(linha));
        if (strcmp(linha, "1") == 0) {
            acao_cadastrar_stream();
        } else if (strcmp(linha, "2") == 0) {
            acao_cadastrar_categoria();
        } else if (strcmp(linha, "5") == 0) {
            acao_listar_streams();
        } else if (strcmp(linha, "6") == 0) {
            acao_listar_categorias();
        } else if (strcmp(linha, "0") == 0) {
            sair = 1;
        } else {
            printf("Opcao invalida.\n");
        }
    }
    return 0;
}
