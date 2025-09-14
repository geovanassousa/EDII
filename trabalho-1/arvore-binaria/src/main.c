#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../includes/tipos.h"
#include "../includes/utils.h"
#include "../includes/stream.h"
#include "../includes/apresentador.h"

/* Estruturas principais (globais do módulo) */
static Stream *RAIZ = NULL;          /* BST de streams */
static Apresentador *HEAD_APR = NULL;/* lista duplamente encadeada de apresentadores */

static void menu(void) {
    printf("-------------------- MENU Q1 --------------------\n");
    printf(" 1 - Cadastrar Stream\n");
    printf(" 2 - Cadastrar Categoria em uma Stream\n");
    printf(" 3 - Cadastrar Apresentador\n");
    printf(" 5 - Mostrar todas as Streams\n");
    printf(" 6 - Mostrar Categorias de uma Stream\n");
    printf("12 - Mostrar Apresentadores de uma Stream\n");
    printf("13 - Mostrar Apresentadores de uma Categoria\n");
    printf(" 0 - SAIR\n");
    printf("-------------------------------------------------\n");
    printf("Escolha uma opcao: ");
}

/* ------------------- ACOES ------------------- */

static void acao_cadastrar_stream(void) {
    char nome[TXT_GRD];
    char site[TXT_GRD];
    int inseriu = 0;

    printf("\n=== Cadastrar Stream ===\n");
    printf("Nome da stream: ");
    scanf(" %127[^\n]", nome);
    getchar(); /* consome '\n' */

    printf("Site da stream: ");
    scanf(" %127[^\n]", site);
    getchar();

    if (stream_buscar(RAIZ, nome) != NULL) {
        printf("Ja existe uma stream com esse nome.\n\n");
    } else {
        RAIZ = stream_inserir(RAIZ, nome, site, &inseriu);
        if (inseriu == 1) {
            printf("Stream cadastrada.\n\n");
        } else {
            printf("Nao foi possivel cadastrar.\n\n");
        }
    }
}

static void acao_listar_streams(void) {
    printf("\n=== Todas as Streams ===\n");
    if (RAIZ == NULL) {
        printf("(nenhuma stream cadastrada)\n\n");
    } else {
        stream_imprimir_inorder(RAIZ);
        printf("\n");
    }
}

static void acao_cadastrar_categoria(void) {
    char nomeStream[TXT_GRD];
    char nomeCat[TXT_GRD];
    int tipoEscolha;
    int tipoNum;
    int inseriu = 0;

    printf("\n=== Cadastrar Categoria ===\n");
    printf("Stream (nome exato): ");
    scanf(" %127[^\n]", nomeStream);
    getchar();

    if (stream_buscar(RAIZ, nomeStream) == NULL) {
        printf("Stream nao encontrada. Cadastre a stream primeiro.\n\n");
    } else {
        printf("Nome da categoria: ");
        scanf(" %127[^\n]", nomeCat);
        getchar();

        printf("Tipo (1=Noticias, 2=Esporte, 3=Entretenimento): ");
        if (scanf("%d", &tipoEscolha) != 1) {
            /* entrada invalida; limpa stdin */
            int ch = 0;
            while (ch != '\n' && ch != EOF) { ch = getchar(); }
            printf("Tipo invalido.\n\n");
            return;
        }
        getchar(); /* consome '\n' */

        tipoNum = parse_categoria_num(tipoEscolha);
        if (tipoNum == 0) {
            printf("Tipo invalido.\n\n");
        } else {
            stream_adicionar_categoria(RAIZ, nomeStream, nomeCat, (TipoCategoria)tipoNum, &inseriu);
            if (inseriu == 1) {
                printf("Categoria cadastrada.\n\n");
            } else {
                printf("Nao foi possivel cadastrar (duplicada ou erro).\n\n");
            }
        }
    }
}

static void acao_listar_categorias(void) {
    char nomeStream[TXT_GRD];
    printf("\n=== Categorias de uma Stream ===\n");
    printf("Stream (nome exato): ");
    scanf(" %127[^\n]", nomeStream);
    getchar();
    stream_listar_categorias(RAIZ, nomeStream);
    printf("\n");
}

static void acao_cadastrar_apresentador(void) {
    char nome[TXT_GRD];
    char categoria[TXT_GRD];
    char streamAtual[TXT_GRD];
    int inseriu = 0;

    printf("\n=== Cadastrar Apresentador ===\n");
    printf("Nome do apresentador: ");
    scanf(" %127[^\n]", nome);
    getchar();

    if (apr_existe_nome(HEAD_APR, nome) == 1) {
        printf("Ja existe apresentador com esse nome.\n\n");
    } else {
        printf("Categoria (texto exato, ex.: Noticias/Esporte/Entretenimento): ");
        scanf(" %127[^\n]", categoria);
        getchar();

        printf("Stream atual (deve existir): ");
        scanf(" %127[^\n]", streamAtual);
        getchar();

        if (stream_buscar(RAIZ, streamAtual) == NULL) {
            printf("Stream nao encontrada. Cadastre a stream primeiro.\n\n");
        } else {
            apr_inserir_ordenado(&HEAD_APR, nome, categoria, streamAtual, &inseriu);
            if (inseriu == 1) {
                printf("Apresentador cadastrado.\n\n");
            } else {
                printf("Nao foi possivel cadastrar.\n\n");
            }
        }
    }
}

static void acao_listar_apr_stream(void) {
    char s[TXT_GRD];
    printf("\n=== Apresentadores de uma Stream ===\n");
    printf("Stream (nome exato): ");
    scanf(" %127[^\n]", s);
    getchar();
    apr_listar_da_stream(HEAD_APR, s);
    printf("\n");
}

static void acao_listar_apr_categoria(void) {
    char c[TXT_GRD];
    printf("\n=== Apresentadores de uma Categoria ===\n");
    printf("Categoria (texto exato): ");
    scanf(" %127[^\n]", c);
    getchar();
    apr_listar_da_categoria(HEAD_APR, c);
    printf("\n");
}

/* ------------------- MAIN ------------------- */

int main(void) {
    int opcao = -1;

    printf("Trabalho ED2 - Q1 (Streams + Categorias + Apresentadores)\n\n");

    do {
        menu();
        if (scanf("%d", &opcao) != 1) {
            /* limpa stdin em caso de lixo */
            int ch = 0;
            while (ch != '\n' && ch != EOF) { ch = getchar(); }
            opcao = -1;
        }
        getchar(); /* consome '\n' do número */

        switch (opcao) {
            case 1:  acao_cadastrar_stream();       break;
            case 2:  acao_cadastrar_categoria();    break;
            case 3:  acao_cadastrar_apresentador(); break;
            case 5:  acao_listar_streams();         break;
            case 6:  acao_listar_categorias();      break;
            case 12: acao_listar_apr_stream();      break;
            case 13: acao_listar_apr_categoria();   break;
            case 0:
                printf("Saindo do programa...\n");
                /* aqui futuramente liberamos memória (free) */
                break;
            default:
                printf("Opcao invalida, tente novamente!\n\n");
                break;
        }
    } while (opcao != 0);

    return 0;
}
