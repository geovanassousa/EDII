#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../includes/tipos.h"
#include "../includes/utils.h"
#include "../includes/stream.h"
#include "../includes/categoria.h"
#include "../includes/programa.h"
#include "../includes/apresentador.h"

/* Globais */
static Stream *RAIZ = NULL;           /* BST de streams */
static Apresentador *HEAD_APR = NULL; /* lista duplamente encadeada */

/* ---------- helpers de seleção por número ---------- */

static void tipo_categoria_texto(TipoCategoria t, char *dest, int n) {
    if (t == TIPO_NOTICIAS) {
        strncpy(dest, "Noticias", n-1);
    } else if (t == TIPO_ESPORTE) {
        strncpy(dest, "Esporte", n-1);
    } else {
        strncpy(dest, "Entretenimento", n-1);
    }
    dest[n-1] = '\0';
}

static Stream* selecionar_stream_por_numero(void) {
    Stream *lista[256];
    int qtd, i, ok, escolha;
    Stream *selecionada = NULL;

    qtd = stream_enumerar(RAIZ, lista, 256);
    if (qtd == 0) {
        printf("(nenhuma stream cadastrada)\n");
        return NULL;
    }

    printf("\nStreams cadastradas:\n");
    i = 0;
    while (i < qtd) {
        printf(" %d) %s (%s)\n", i + 1, lista[i]->nome, lista[i]->site);
        i = i + 1;
    }

    ok = 0;
    while (ok == 0) {
        printf("Escolha o numero da stream: ");
        if (scanf("%d", &escolha) != 1) {
            int ch = 0; while (ch != '\n' && ch != EOF) { ch = getchar(); }
        } else {
            if (escolha >= 1 && escolha <= qtd) {
                selecionada = lista[escolha - 1];
                ok = 1;
            } else {
                printf("Valor invalido.\n");
            }
        }
        getchar();
    }
    return selecionada;
}

static Categoria* selecionar_categoria_por_numero(Categoria *cabeca) {
    Categoria *vet[256];
    int qtd, i, ok, escolha, idx;
    Categoria *selecionada = NULL;

    qtd = cat_enumerar(cabeca, vet, 256);
    if (qtd == 0) { printf("(sem categorias)\n"); return NULL; }

    printf("\nCategorias:\n");
    i = 0;
    while (i < qtd) {
        printf(" %d) %s\n", i + 1, vet[i]->nome);
        i = i + 1;
    }

    ok = 0; idx = -1;
    while (ok == 0) {
        printf("Escolha o numero da categoria: ");
        if (scanf("%d", &escolha) != 1) {
            int ch = 0; while (ch != '\n' && ch != EOF) { ch = getchar(); }
        } else {
            if (escolha >= 1 && escolha <= qtd) {
                idx = escolha - 1;
                ok = 1;
            } else {
                printf("Valor invalido.\n");
            }
        }
        getchar();
    }
    if (idx >= 0) { selecionada = vet[idx]; }
    return selecionada;
}


static int selecionar_tipo_categoria(TipoCategoria *outTipo, char *outNome, int outTam) {
    int op, valido, t;
    printf("Tipo de categoria:\n 1) Noticias\n 2) Esporte\n 3) Entretenimento\n");
    valido = 0;
    while (valido == 0) {
        printf("Escolha (1-3): ");
        if (scanf("%d", &op) != 1) {
            int ch = 0; while (ch != '\n' && ch != EOF) { ch = getchar(); }
        } else {
            t = parse_categoria_num(op);
            if (t == 0) {
                printf("Opcao invalida.\n");
            } else {
                *outTipo = (TipoCategoria) t;
                if (t == TIPO_NOTICIAS) strncpy(outNome, "Noticias", outTam-1);
                else if (t == TIPO_ESPORTE) strncpy(outNome, "Esporte", outTam-1);
                else strncpy(outNome, "Entretenimento", outTam-1);
                outNome[outTam-1] = '\0';
                valido = 1;
            }
        }
        getchar();
    }
    return 1;
}

static int selecionar_tipo_demanda(TipoDemanda *out) {
    int op, ok = 0;
    printf("Demanda:\n 1) Ao vivo\n 2) Sob demanda\n");
    while (ok == 0) {
        printf("Escolha (1-2): ");
        if (scanf("%d", &op) != 1) {
            int ch = 0; while (ch != '\n' && ch != EOF) { ch = getchar(); }
        } else {
            if (op == 1) { *out = DEMANDA_AO_VIVO; ok = 1; }
            else if (op == 2) { *out = DEMANDA_SOB_DEMANDA; ok = 1; }
            else { printf("Opcao invalida.\n"); }
        }
        getchar();
    }
    return 1;
}

static Apresentador* selecionar_apresentador_elegivel_por_tipo(TipoCategoria tipoCat, const char *nomeStream) {
    Apresentador *vet[256];
    int qtd, i, escolha, ok, idx;
    Apresentador *selecionado = NULL;
    char tipoTxt[TXT_GRD];

    tipo_categoria_texto(tipoCat, tipoTxt, sizeof(tipoTxt));

    qtd = apr_enumerar_elegiveis(HEAD_APR, tipoTxt, nomeStream, vet, 256);
    if (qtd == 0) {
        printf("(nenhum apresentador elegivel; cadastre primeiro)\n");
        return NULL;
    }

    printf("\nApresentadores elegiveis (cat: %s, stream: %s):\n", tipoTxt, nomeStream);
    i = 0;
    while (i < qtd) {
        printf(" %d) %s\n", i + 1, vet[i]->nome);
        i = i + 1;
    }

    ok = 0; idx = -1;
    while (ok == 0) {
        printf("Escolha o numero do apresentador: ");
        if (scanf("%d", &escolha) != 1) {
            int ch = 0; while (ch != '\n' && ch != EOF) { ch = getchar(); }
        } else {
            if (escolha >= 1 && escolha <= qtd) {
                idx = escolha - 1;
                ok = 1;
            } else {
                printf("Valor invalido.\n");
            }
        }
        getchar();
    }
    if (idx >= 0) { selecionado = vet[idx]; }
    return selecionado;
}

/* coleta nomes distintos de categorias existentes em todas as streams */
static int coletar_categorias_distintas_globais(char nomes[][TXT_GRD], int max) {
    Stream *sVet[256];
    Categoria *cVet[256];
    int nStreams, nCats, i, j, k, tot, existe;

    tot = 0;
    nStreams = stream_enumerar(RAIZ, sVet, 256);

    i = 0;
    while (i < nStreams) {
        nCats = cat_enumerar(sVet[i]->categorias, cVet, 256);
        j = 0;
        while (j < nCats) {
            existe = 0;
            k = 0;
            while (k < tot && existe == 0) {
                if (str_cmp_i(nomes[k], cVet[j]->nome) == 0) {
                    existe = 1;
                } else {
                    k = k + 1;
                }
            }
            if (existe == 0 && tot < max) {
                strncpy(nomes[tot], cVet[j]->nome, TXT_GRD-1);
                nomes[tot][TXT_GRD-1] = '\0';
                tot = tot + 1;
            }
            j = j + 1;
        }
        i = i + 1;
    }
    return tot;
}

static int selecionar_dia_semana(void) {
    int op, ok = 0;
    printf("Dia da semana:\n 1) Dom\n 2) Seg\n 3) Ter\n 4) Qua\n 5) Qui\n 6) Sex\n 7) Sab\n");
    while (ok == 0) {
        printf("Escolha (1-7): ");
        if (scanf("%d", &op) != 1) {
            int ch = 0; while (ch != '\n' && ch != EOF) { ch = getchar(); }
        } else {
            if (op >= 1 && op <= 7) {
                ok = 1;
            } else {
                printf("Valor invalido.\n");
            }
        }
        getchar();
    }
    return op;
}

static void dia_semana_texto(int d, char *t, int n) {
    if (d == 1) strncpy(t, "Dom", n-1);
    else if (d == 2) strncpy(t, "Seg", n-1);
    else if (d == 3) strncpy(t, "Ter", n-1);
    else if (d == 4) strncpy(t, "Qua", n-1);
    else if (d == 5) strncpy(t, "Qui", n-1);
    else if (d == 6) strncpy(t, "Sex", n-1);
    else strncpy(t, "Sab", n-1);
    t[n-1] = '\0';
}

static void rstrip(char *s) {
    int n = (int)strlen(s);
    while (n > 0 && (s[n-1] == ' ' || s[n-1] == '\t' || s[n-1] == '\r')) {
        s[n-1] = '\0';
        n = n - 1;
    }
}

/* percorre a BST e imprime programas do dia (inclui Diarios = diaSemana 0) */
static void _listar_prog_dia(Programa *r, int dia, int *tot) {
    if (r != NULL) {
        _listar_prog_dia(r->esq, dia, tot);

        if (r->diaSemana == dia || r->diaSemana == 0) {
            if (r->diaSemana == 0) {
                /* Diario: nao mostra o dia */
                printf("- %s | %s | %d min | %s | %s | apres: %s\n",
                       r->nome, r->periodicidade, r->tempoMin, r->horarioInicio,
                       (r->demanda == DEMANDA_AO_VIVO ? "Ao Vivo" : "Sob Demanda"),
                       r->apresentador);
            } else {
                char d[16];
                dia_semana_texto(r->diaSemana, d, sizeof(d));
                printf("- %s | %s | %d min | %s (%s) | %s | apres: %s\n",
                       r->nome, r->periodicidade, r->tempoMin, r->horarioInicio, d,
                       (r->demanda == DEMANDA_AO_VIVO ? "Ao Vivo" : "Sob Demanda"),
                       r->apresentador);
            }
            *tot = *tot + 1;
        }

        _listar_prog_dia(r->dir, dia, tot);
    }
}

/* enumera programas da BST em ordem alfabetica (in-order) */
static void _prog_enumerar(Programa *r, Programa **vet, int max, int *qtd) {
    if (r != NULL) {
        _prog_enumerar(r->esq, vet, max, qtd);
        if (*qtd < max) {
            vet[*qtd] = r;
            *qtd = *qtd + 1;
        }
        _prog_enumerar(r->dir, vet, max, qtd);
    }
}

/* imprime uma linha-resumo do programa para a lista numerada */
static void _print_programa_resumo(const Programa *p) {
    if (p != NULL) {
        if (p->diaSemana == 0) {
            printf("   %s | %s | %s | %s\n",
                   p->nome, p->periodicidade, p->horarioInicio,
                   (p->demanda == DEMANDA_AO_VIVO ? "Ao Vivo" : "Sob Demanda"));
        } else {
            char d[16];
            dia_semana_texto(p->diaSemana, d, sizeof(d));
            printf("   %s | %s | %s (%s) | %s\n",
                   p->nome, p->periodicidade, p->horarioInicio, d,
                   (p->demanda == DEMANDA_AO_VIVO ? "Ao Vivo" : "Sob Demanda"));
        }
    }
}


/* ------------------- MENU ------------------- */
static void menu(void) {
    printf("-------------------- MENU Q1 --------------------\n");
    printf(" 1 - Cadastrar Stream\n");
    printf(" 2 - Cadastrar Categorias\n");
    printf(" 3 - Cadastrar Programas\n");
    printf(" 4 - Cadastrar Apresentador\n");
    printf(" 5 - Mostrar todas as streams cadastradas\n");
    printf(" 6 - Mostrar todas as categorias de uma stream\n");
    printf(" 7 - Mostrar todos os programas de uma categoria de uma stream\n");
    printf(" 8 - Mostrar todas as streams que tem uma categoria\n");
    printf(" 9 - Mostrar programas de uma stream em um dia e horario\n");
    printf("10 - Mostrar streams que tem um tipo de categoria\n");
    printf("11 - Mostrar programas de um dia da semana de uma categoria de uma stream\n");
    printf("12 - Mostrar apresentadores de uma stream\n");
    printf("13 - Mostrar apresentadores de uma categoria (independente da stream)\n");
    printf("14 - Mostrar dados de um programa de uma categoria de uma stream\n");
    printf("15 - Remover programa de uma categoria de uma stream\n");
    printf("16 - Remover categoria de uma stream (apenas se vazia)\n");
    printf("17 - Alterar a stream atual de um apresentador (com restricoes)\n");
    printf(" 0 - SAIR\n");
    printf("-------------------------------------------------\n");
    printf("Escolha uma opcao: ");
}

/* ------------------- ACOES IMPLEMENTADAS ------------------- */

/* (i) Cadastrar Stream */
static void acao_cadastrar_stream(void) {
    char nome[TXT_GRD], site[TXT_GRD];
    int inseriu = 0;

    printf("\n=== Cadastrar Stream ===\n");
    printf("Nome da stream: "); scanf(" %127[^\n]", nome); getchar();
    printf("Site da stream: "); scanf(" %127[^\n]", site); getchar();

    if (stream_buscar(RAIZ, nome) != NULL) {
        printf("Ja existe uma stream com esse nome.\n\n");
    } else {
        RAIZ = stream_inserir(RAIZ, nome, site, &inseriu);
        if (inseriu == 1) printf("Stream cadastrada.\n\n");
        else printf("Nao foi possivel cadastrar.\n\n");
    }
}

/* (ii) Cadastrar Categorias */
static void acao_cadastrar_categoria(void) {
    Stream *s; char nomeCat[TXT_GRD]; TipoCategoria tipo; char nomeTipo[TXT_GRD]; int inseriu = 0;

    printf("\n=== Cadastrar Categoria ===\n");
    s = selecionar_stream_por_numero();
    if (s == NULL) { printf("Cadastre uma stream primeiro.\n\n"); }
    else {
        printf("Nome da categoria: "); scanf(" %127[^\n]", nomeCat); getchar();
        if (selecionar_tipo_categoria(&tipo, nomeTipo, sizeof(nomeTipo)) == 1) {
            stream_adicionar_categoria(RAIZ, s->nome, nomeCat, tipo, &inseriu);
            if (inseriu == 1) printf("Categoria cadastrada em \"%s\".\n\n", s->nome);
            else printf("Nao foi possivel cadastrar (duplicada ou erro).\n\n");
        }
    }
}

/* (iii) Cadastrar Programas */

static void acao_cadastrar_programa(void) {
    Stream *s; Categoria *c; Apresentador *apr;
    char nomeProg[TXT_GRD], periodicidade[TXT_PEQ], hhmm[6];
    int tempoMin, inseriu = 0;
    int diaSemSelecionado, diaSemArmazenado;
    TipoDemanda demanda;
    Programa *exist;

    printf("\n=== Cadastrar Programa ===\n");
    s = selecionar_stream_por_numero();
    if (s == NULL) { printf("Cadastre uma stream primeiro.\n\n"); return; }

    c = selecionar_categoria_por_numero(s->categorias);
    if (c == NULL) { printf("Cadastre categorias nessa stream primeiro.\n\n"); return; }

    apr = selecionar_apresentador_elegivel_por_tipo(c->tipo, s->nome);
    if (apr == NULL) { printf("Nenhum apresentador elegivel.\n\n"); return; }

    printf("Nome do programa: "); scanf(" %127[^\n]", nomeProg); getchar();

    exist = prog_buscar(c->raizProgramas, nomeProg);
    if (exist != NULL) { printf("Programa ja existe nesta categoria.\n\n"); return; }

    printf("Periodicidade (ex.: Diario/Semanal/Mensal): ");
    scanf(" %31[^\n]", periodicidade); getchar();
    rstrip(periodicidade); /* evita 'diario ' com espaco no fim */

    /* Se for Diario, nao perguntamos o dia; guardamos 0 (=vale qualquer dia) */
    if (str_cmp_i(periodicidade, "Diario") == 0 || str_cmp_i(periodicidade, "Diário") == 0) {
        diaSemArmazenado = 0;
    } else {
        diaSemSelecionado = selecionar_dia_semana();
        diaSemArmazenado = diaSemSelecionado;
    }

    printf("Tempo (minutos): ");
    if (scanf("%d", &tempoMin) != 1) { int ch=0; while (ch!='\n' && ch!=EOF){ch=getchar();} tempoMin = 0; }
    getchar();

    printf("Horario de inicio (HH:MM): ");
    scanf(" %5[^\n]", hhmm); getchar();

    if (selecionar_tipo_demanda(&demanda) == 1) {
        char tipoTxt[TXT_GRD];
        tipo_categoria_texto(c->tipo, tipoTxt, sizeof(tipoTxt));
        if (apr_pode_apresentar(HEAD_APR, apr->nome, tipoTxt, s->nome) == 1) {
            c->raizProgramas = prog_inserir(c->raizProgramas, nomeProg, periodicidade,
                                            tempoMin, hhmm, diaSemArmazenado, demanda, apr->nome, &inseriu);
            if (inseriu == 1) { printf("Programa cadastrado com sucesso.\n\n"); }
            else { printf("Nao foi possivel cadastrar o programa.\n\n"); }
        } else {
            printf("Apresentador nao atende (categoria/stream).\n\n");
        }
    }
}


/* (iv) Cadastrar Apresentador */
static void acao_cadastrar_apresentador(void) {
    char nome[TXT_GRD], categoriaTxt[TXT_GRD], nomeTipo[TXT_GRD];
    TipoCategoria tipo; Stream *s; int inseriu = 0;

    printf("\n=== Cadastrar Apresentador ===\n");
    printf("Nome do apresentador: "); scanf(" %127[^\n]", nome); getchar();

    if (apr_existe_nome(HEAD_APR, nome) == 1) {
        printf("Ja existe apresentador com esse nome.\n\n");
    } else {
        if (selecionar_tipo_categoria(&tipo, nomeTipo, sizeof(nomeTipo)) == 1) {
            strncpy(categoriaTxt, nomeTipo, sizeof(categoriaTxt)-1);
            categoriaTxt[sizeof(categoriaTxt)-1] = '\0';
        }
        s = selecionar_stream_por_numero();
        if (s == NULL) { printf("Cadastre uma stream primeiro.\n\n"); }
        else {
            apr_inserir_ordenado(&HEAD_APR, nome, categoriaTxt, s->nome, &inseriu);
            if (inseriu == 1) printf("Apresentador cadastrado (cat: %s, stream: %s).\n\n", categoriaTxt, s->nome);
            else printf("Nao foi possivel cadastrar.\n\n");
        }
    }
}

/* (v) Mostrar todas as streams */
static void acao_listar_streams(void) {
    printf("\n=== Todas as Streams ===\n");
    if (RAIZ == NULL) printf("(nenhuma stream cadastrada)\n\n");
    else { stream_imprimir_inorder(RAIZ); printf("\n"); }
}

/* (vi) Mostrar categorias de uma stream */
static void acao_listar_categorias_stream(void) {
    Stream *s; printf("\n=== Categorias de uma Stream ===\n");
    s = selecionar_stream_por_numero();
    if (s == NULL) { printf("Cadastre uma stream primeiro.\n\n"); }
    else { stream_listar_categorias(RAIZ, s->nome); printf("\n"); }
}

/* (vii) Mostrar todos os programas de uma categoria de uma stream */
static void acao_listar_programas_cat_stream(void) {
    Stream *s;
    Categoria *c;
    char tipoTxt[TXT_GRD];

    printf("\n=== Programas de uma Categoria de uma Stream ===\n");

    /* escolher a stream por número */
    s = selecionar_stream_por_numero();
    if (s == NULL) {
        printf("Cadastre uma stream primeiro.\n\n");
    } else {
        /* escolher a categoria por número, dentre as da stream */
        c = selecionar_categoria_por_numero(s->categorias);
        if (c == NULL) {
            printf("Cadastre categorias nessa stream primeiro.\n\n");
        } else {
            /* cabeçalho bonito com tipo + nome da categoria */
            tipo_categoria_texto(c->tipo, tipoTxt, sizeof(tipoTxt));
            printf("Stream: %s | Categoria: %s (tipo: %s)\n", s->nome, c->nome, tipoTxt);

            if (c->raizProgramas == NULL) {
                printf("(nenhum programa cadastrado nesta categoria)\n\n");
            } else {
                /* imprime BST em ordem alfabética pelo nome do programa */
                prog_imprimir_inorder(c->raizProgramas);
                printf("\n");
            }
        }
    }
}

/* (viii) Mostrar todas as streams que têm uma determinada categoria (por NOME da categoria) */
static void acao_listar_streams_com_categoria(void) {
    char cats[256][TXT_GRD];
    int qtd, i, escolha, ok, idx;
    char alvo[TXT_GRD];
    Stream *sVet[256];
    int nStreams, alguma;

    printf("\n=== Streams que possuem uma determinada categoria ===\n");

    qtd = coletar_categorias_distintas_globais(cats, 256);
    if (qtd == 0) {
        printf("(nao ha categorias cadastradas em nenhuma stream)\n\n");
    } else {
        /* lista categorias disponíveis para evitar erro de digitação */
        printf("Categorias existentes (por nome):\n");
        i = 0;
        while (i < qtd) {
            printf(" %d) %s\n", i + 1, cats[i]);
            i = i + 1;
        }

        ok = 0; idx = -1;
        while (ok == 0) {
            printf("Escolha o numero da categoria: ");
            if (scanf("%d", &escolha) != 1) {
                int ch = 0; while (ch != '\n' && ch != EOF) { ch = getchar(); }
            } else {
                if (escolha >= 1 && escolha <= qtd) {
                    idx = escolha - 1;
                    ok = 1;
                } else {
                    printf("Valor invalido.\n");
                }
            }
            getchar();
        }

        if (idx >= 0) {
            strncpy(alvo, cats[idx], sizeof(alvo)-1);
            alvo[sizeof(alvo)-1] = '\0';

            /* varre streams e imprime as que possuem a categoria 'alvo' */
            nStreams = stream_enumerar(RAIZ, sVet, 256);
            alguma = 0;
            i = 0;
            while (i < nStreams) {
                /* checa se a stream tem a categoria pelo nome */
                if (cat_existe(sVet[i]->categorias, alvo) == 1) {
                    printf("- %s (%s)\n", sVet[i]->nome, sVet[i]->site);
                    alguma = 1;
                }
                i = i + 1;
            }
            if (alguma == 0) {
                printf("(nenhuma stream possui a categoria \"%s\")\n", alvo);
            }
            printf("\n");
        }
    }
}
/* (ix) Mostrar programas de uma stream em um dia e horário */
static void _listar_match_prog(Programa *r, int dia, const char *hhmm, const char *nomeCat, int *tot) {
    if (r != NULL) {
        _listar_match_prog(r->esq, dia, hhmm, nomeCat, tot);
        if ( (r->diaSemana == dia || r->diaSemana == 0) && strcmp(r->horarioInicio, hhmm) == 0 ) {
            printf("[%s] - %s | %s | %d min | %s | %s | apres: %s\n",
                   nomeCat, r->nome, r->periodicidade, r->tempoMin, r->horarioInicio,
                   (r->demanda == DEMANDA_AO_VIVO ? "Ao Vivo" : "Sob Demanda"),
                   r->apresentador);
            *tot = *tot + 1;
        }
        _listar_match_prog(r->dir, dia, hhmm, nomeCat, tot);
    }
}

static void acao_listar_programas_por_dia_horario(void) {
    Stream *s;
    Categoria *cVet[256];
    int nCats, i, total;
    int dia;
    char hhmm[6], diaTxt[16];

    printf("\n=== Programas de uma Stream por Dia e Horario ===\n");

    s = selecionar_stream_por_numero();
    if (s == NULL) {
        printf("Cadastre uma stream primeiro.\n\n");
    } else {
        dia = selecionar_dia_semana();
        dia_semana_texto(dia, diaTxt, sizeof(diaTxt));

        printf("Horario (HH:MM): ");
        scanf(" %5[^\n]", hhmm); getchar();

        printf("Stream: %s | Dia: %s | Horario: %s\n", s->nome, diaTxt, hhmm);

        nCats = cat_enumerar(s->categorias, cVet, 256);
        total = 0;
        i = 0;
        while (i < nCats) {
            _listar_match_prog(cVet[i]->raizProgramas, dia, hhmm, cVet[i]->nome, &total);
            i = i + 1;
        }
        if (total == 0) {
            printf("(nenhum programa encontrado nesse dia/horario na stream)\n");
        }
        printf("\n");
    }
}


/* (x) Mostrar todas as streams que tem um determinado TIPO de categoria */
static void acao_listar_streams_por_tipo_categoria(void) {
    TipoCategoria tipo;
    char tipoTxt[TXT_GRD];
    Stream *sVet[256];
    Categoria *cVet[256];
    int nStreams, nCats, i, j, impressa, totalImp;

    printf("\n=== Streams por Tipo de Categoria ===\n");

    if (selecionar_tipo_categoria(&tipo, tipoTxt, sizeof(tipoTxt)) == 1) {
        nStreams = stream_enumerar(RAIZ, sVet, 256);
        totalImp = 0;

        i = 0;
        while (i < nStreams) {
            impressa = 0;
            nCats = cat_enumerar(sVet[i]->categorias, cVet, 256);

            j = 0;
            while (j < nCats) {
                if (cVet[j]->tipo == tipo && impressa == 0) {
                    printf("- %s (%s)\n", sVet[i]->nome, sVet[i]->site);
                    impressa = 1;            /* imprime cada stream no máximo 1 vez */
                    totalImp = totalImp + 1;
                }
                j = j + 1;
            }
            i = i + 1;
        }

        if (totalImp == 0) {
            printf("(nenhuma stream possui categoria do tipo: %s)\n", tipoTxt);
        }
        printf("\n");
    }
}

/* (xi) Mostrar todos os programas de um dia da semana de uma categoria de uma stream */
static void acao_listar_programas_por_dia_semana_cat_stream(void) {
    Stream *s;
    Categoria *c;
    int dia, total;
    char diaTxt[16], tipoTxt[TXT_GRD];

    printf("\n=== Programas por Dia da Semana de uma Categoria de uma Stream ===\n");

    s = selecionar_stream_por_numero();
    if (s == NULL) {
        printf("Cadastre uma stream primeiro.\n\n");
    } else {
        c = selecionar_categoria_por_numero(s->categorias);
        if (c == NULL) {
            printf("Cadastre categorias nessa stream primeiro.\n\n");
        } else {
            dia = selecionar_dia_semana();
            dia_semana_texto(dia, diaTxt, sizeof(diaTxt));
            tipo_categoria_texto(c->tipo, tipoTxt, sizeof(tipoTxt));

            printf("Stream: %s | Categoria: %s (tipo: %s) | Dia: %s\n",
                   s->nome, c->nome, tipoTxt, diaTxt);

            if (c->raizProgramas == NULL) {
                printf("(nenhum programa nesta categoria)\n\n");
            } else {
                total = 0;
                _listar_prog_dia(c->raizProgramas, dia, &total);
                if (total == 0) {
                    printf("(nenhum programa encontrado para esse dia)\n");
                }
                printf("\n");
            }
        }
    }
}

/* (xii) */
static void acao_listar_apr_stream(void) {
    Stream *s; printf("\n=== Apresentadores de uma Stream ===\n");
    s = selecionar_stream_por_numero();
    if (s == NULL) { printf("Cadastre uma stream primeiro.\n\n"); }
    else { apr_listar_da_stream(HEAD_APR, s->nome); printf("\n"); }
}

/* (xiii) */
static void acao_listar_apr_categoria(void) {
    TipoCategoria tipo; char nomeTipo[TXT_GRD];
    printf("\n=== Apresentadores de uma Categoria ===\n");
    if (selecionar_tipo_categoria(&tipo, nomeTipo, sizeof(nomeTipo)) == 1) {
        apr_listar_da_categoria(HEAD_APR, nomeTipo); printf("\n");
    }
}

/* (xiv) Mostrar os dados de um determinado programa de uma categoria de uma stream */
static void acao_mostrar_dados_programa(void) {
    Stream *s;
    Categoria *c;
    Programa *vet[256];
    int qtd, i, ok, escolha, idx;

    printf("\n=== Dados de um Programa (Categoria de uma Stream) ===\n");

    /* escolher stream */
    s = selecionar_stream_por_numero();
    if (s == NULL) {
        printf("Cadastre uma stream primeiro.\n\n");
    } else {
        /* escolher categoria */
        c = selecionar_categoria_por_numero(s->categorias);
        if (c == NULL) {
            printf("Cadastre categorias nessa stream primeiro.\n\n");
        } else {
            /* listar programas da categoria (in-order) para escolher por numero */
            qtd = 0;
            _prog_enumerar(c->raizProgramas, vet, 256, &qtd);

            if (qtd == 0) {
                printf("(nenhum programa nesta categoria)\n\n");
            } else {
                char tipoTxt[TXT_GRD];
                tipo_categoria_texto(c->tipo, tipoTxt, sizeof(tipoTxt));

                printf("Stream: %s | Categoria: %s (tipo: %s)\n", s->nome, c->nome, tipoTxt);
                printf("Escolha o programa:\n");

                i = 0;
                while (i < qtd) {
                    printf(" %d) ", i + 1);
                    _print_programa_resumo(vet[i]);
                    i = i + 1;
                }

                ok = 0; idx = -1;
                while (ok == 0) {
                    printf("Numero do programa: ");
                    if (scanf("%d", &escolha) != 1) {
                        int ch = 0; while (ch != '\n' && ch != EOF) { ch = getchar(); }
                    } else {
                        if (escolha >= 1 && escolha <= qtd) {
                            idx = escolha - 1;
                            ok = 1;
                        } else {
                            printf("Valor invalido.\n");
                        }
                    }
                    getchar();
                }

                if (idx >= 0) {
                    Programa *p = vet[idx];
                    char d[16]; char tipoTxt2[TXT_GRD];
                    dia_semana_texto(p->diaSemana == 0 ? 0 : p->diaSemana, d, sizeof(d));
                    tipo_categoria_texto(c->tipo, tipoTxt2, sizeof(tipoTxt2));

                    printf("\n--- DADOS DO PROGRAMA ---\n");
                    printf("Stream        : %s (%s)\n", s->nome, s->site);
                    printf("Categoria     : %s (tipo: %s)\n", c->nome, tipoTxt2);
                    printf("Nome          : %s\n", p->nome);
                    printf("Periodicidade : %s\n", p->periodicidade);
                    if (p->diaSemana == 0) {
                        printf("Dia           : Diario\n");
                    } else {
                        printf("Dia           : %s\n", d);
                    }
                    printf("Horario inicio: %s\n", p->horarioInicio);
                    printf("Duracao       : %d min\n", p->tempoMin);
                    printf("Exibicao      : %s\n", (p->demanda == DEMANDA_AO_VIVO ? "Ao Vivo" : "Sob Demanda"));
                    printf("Apresentador  : %s\n", p->apresentador);
                    printf("-------------------------\n\n");
                }
            }
        }
    }
}

/* (xv) Remover programa de uma categoria de uma stream */
static void acao_remover_programa(void) {
    Stream *s;
    Categoria *c;
    Programa *vet[256];
    int qtd, i, ok, escolha, idx, removeu;

    printf("\n=== Remover Programa de uma Categoria de uma Stream ===\n");

    s = selecionar_stream_por_numero();
    if (s == NULL) {
        printf("Cadastre uma stream primeiro.\n\n");
    } else {
        c = selecionar_categoria_por_numero(s->categorias);
        if (c == NULL) {
            printf("Cadastre categorias nessa stream primeiro.\n\n");
        } else {
            qtd = 0;
            _prog_enumerar(c->raizProgramas, vet, 256, &qtd);

            if (qtd == 0) {
                printf("(nenhum programa nesta categoria)\n\n");
            } else {
                printf("Escolha o programa para remover:\n");
                i = 0;
                while (i < qtd) {
                    printf(" %d) ", i + 1);
                    _print_programa_resumo(vet[i]);
                    i = i + 1;
                }

                ok = 0; idx = -1;
                while (ok == 0) {
                    printf("Numero do programa: ");
                    if (scanf("%d", &escolha) != 1) {
                        int ch = 0; while (ch != '\n' && ch != EOF) { ch = getchar(); }
                    } else {
                        if (escolha >= 1 && escolha <= qtd) {
                            idx = escolha - 1;
                            ok = 1;
                        } else {
                            printf("Valor invalido.\n");
                        }
                    }
                    getchar();
                }

                removeu = 0;
                c->raizProgramas = prog_remover(c->raizProgramas, vet[idx]->nome, &removeu);
                if (removeu == 1) {
                    printf("Programa removido.\n\n");
                } else {
                    printf("Nao foi possivel remover.\n\n");
                }
            }
        }
    }
}




/* ------------------- STUBS (vamos implementar depois) ------------------- */


/* (xvi) */
static void acao_remover_categoria_se_vazia(void) {
    printf("\n(em desenvolvimento: remover categoria se nao tiver programas)\n\n");
}

/* (xvii) */
static void acao_alterar_stream_atual_apresentador(void) {
    printf("\n(em desenvolvimento: alterar stream atual do apresentador com as restricoes)\n\n");
}

/* ------------------- MAIN ------------------- */
int main(void) {
    int opcao = -1;

    printf("Trabalho ED2 - Q1 (menu alinhado ao enunciado)\n\n");

    do {
        menu();
        if (scanf("%d", &opcao) != 1) {
            int ch = 0; while (ch != '\n' && ch != EOF) { ch = getchar(); }
            opcao = -1;
        }
        getchar();

        switch (opcao) {
            case 1:  acao_cadastrar_stream();                      break; /* i */
            case 2:  acao_cadastrar_categoria();                   break; /* ii */
            case 3:  acao_cadastrar_programa();                    break; /* iii */
            case 4:  acao_cadastrar_apresentador();                break; /* iv */
            case 5:  acao_listar_streams();                        break; /* v */
            case 6:  acao_listar_categorias_stream();              break; /* vi */
            case 7:  acao_listar_programas_cat_stream();           break; /* vii */
            case 8:  acao_listar_streams_com_categoria();          break; /* viii */
            case 9:  acao_listar_programas_por_dia_horario();      break; /* ix */
            case 10: acao_listar_streams_por_tipo_categoria();     break; /* x */
            case 11: acao_listar_programas_por_dia_semana_cat_stream(); break; /* xi */
            case 12: acao_listar_apr_stream();                     break; /* xii */
            case 13: acao_listar_apr_categoria();                  break; /* xiii */
            case 14: acao_mostrar_dados_programa();                break; /* xiv */
            case 15: acao_remover_programa();                      break; /* xv */
            case 16: acao_remover_categoria_se_vazia();            break; /* xvi */
            case 17: acao_alterar_stream_atual_apresentador();     break; /* xvii */
            case 0:
                printf("Saindo do programa...\n");
                break;
            default:
                printf("Opcao invalida, tente novamente!\n\n");
                break;
        }
    } while (opcao != 0);

    return 0;
}
