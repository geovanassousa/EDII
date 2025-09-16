#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/programa.h"
#include "../includes/utils.h"


Programa* prog_criar(const char *nome, const char *period, int tempoMin,
                     const char *hhmm, int diaSemana, TipoDemanda demanda, const char *apres) {
    Programa *p = (Programa*) malloc(sizeof(Programa));
    if (p != NULL) {
        strncpy(p->nome, nome, sizeof(p->nome)-1); p->nome[sizeof(p->nome)-1] = '\0';
        strncpy(p->periodicidade, period, sizeof(p->periodicidade)-1);
        p->periodicidade[sizeof(p->periodicidade)-1] = '\0';
        p->tempoMin = tempoMin;
        strncpy(p->horarioInicio, hhmm, sizeof(p->horarioInicio)-1);
        p->horarioInicio[sizeof(p->horarioInicio)-1] = '\0';
        p->diaSemana = diaSemana;
        p->demanda = demanda;
        strncpy(p->apresentador, apres, sizeof(p->apresentador)-1);
        p->apresentador[sizeof(p->apresentador)-1] = '\0';
        p->esq = NULL;
        p->dir = NULL;
        p->altura = 1;  // novo nó tem altura 1
    }
    return p;
}

Programa* prog_buscar(Programa *raiz, const char *nome) {
    int cmp;
    if (raiz == NULL) return NULL;
    cmp = str_cmp_i(nome, raiz->nome);
    if (cmp == 0) return raiz;
    if (cmp < 0)  return prog_buscar(raiz->esq, nome);
    return prog_buscar(raiz->dir, nome);
}

Programa* prog_inserir(Programa *raiz, const char *nome, const char *period, int tempoMin,
                       const char *hhmm, int diaSemana, TipoDemanda demanda, const char *apres, int *inseriu) {
    int cmp;
    if (raiz == NULL) {
        Programa *novo = prog_criar(nome, period, tempoMin, hhmm, diaSemana, demanda, apres);
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
        raiz->esq = prog_inserir(raiz->esq, nome, period, tempoMin, hhmm, diaSemana, demanda, apres, inseriu);
    } else {
        raiz->dir = prog_inserir(raiz->dir, nome, period, tempoMin, hhmm, diaSemana, demanda, apres, inseriu);
    }
    
    // Balanceia a árvore após a inserção
    return prog_balancear(raiz);
}

static void imprimir_programa(const Programa *programa) {
    if (programa != NULL) {
        if (programa->diaSemana == 0) {
            /* programa diario: nao mostra o dia especifico */
            printf("- %s | %s | %d min | %s | %s | apres: %s\n",
                   programa->nome, programa->periodicidade, programa->tempoMin, programa->horarioInicio,
                   (programa->demanda == DEMANDA_AO_VIVO ? "Ao Vivo" : "Sob Demanda"),
                   programa->apresentador);
        } else {
            char dia_texto[16];
            dia_semana_para_texto(programa->diaSemana, dia_texto, sizeof(dia_texto));
            printf("- %s | %s | %d min | %s (%s) | %s | apres: %s\n",
                   programa->nome, programa->periodicidade, programa->tempoMin, programa->horarioInicio, dia_texto,
                   (programa->demanda == DEMANDA_AO_VIVO ? "Ao Vivo" : "Sob Demanda"),
                   programa->apresentador);
        }
    }
}


void prog_imprimir_inorder(Programa *raiz) {
    if (raiz != NULL) {
        prog_imprimir_inorder(raiz->esq);
        imprimir_programa(raiz);
        prog_imprimir_inorder(raiz->dir);
    }
}

static Programa* encontrar_menor_no(Programa *raiz) {
    Programa *atual = raiz;
    if (atual != NULL) {
        while (atual->esq != NULL) {
            atual = atual->esq;
        }
    }
    return atual;
}

/* remove programa da árvore AVL */
Programa* prog_remover(Programa *raiz, const char *nome, int *removeu) {
    int cmp;
    if (removeu != NULL) { *removeu = 0; }
    if (raiz == NULL) { return NULL; }

    cmp = str_cmp_i(nome, raiz->nome);
    if (cmp < 0) {
        raiz->esq = prog_remover(raiz->esq, nome, removeu);
    } else if (cmp > 0) {
        raiz->dir = prog_remover(raiz->dir, nome, removeu);
    } else {
       
        if (raiz->esq == NULL && raiz->dir == NULL) {
            free(raiz);
            if (removeu != NULL) { *removeu = 1; }
            return NULL;
        } else if (raiz->esq == NULL) {
            Programa *tmp = raiz->dir;
            free(raiz);
            if (removeu != NULL) { *removeu = 1; }
            return tmp;
        } else if (raiz->dir == NULL) {
            Programa *tmp = raiz->esq;
            free(raiz);
            if (removeu != NULL) { *removeu = 1; }
            return tmp;
        } else {
            /* dois filhos: substitui pelo sucessor */
            Programa *sucessor = encontrar_menor_no(raiz->dir);

            strncpy(raiz->nome, sucessor->nome, sizeof(raiz->nome)-1);
            raiz->nome[sizeof(raiz->nome)-1] = '\0';

            strncpy(raiz->periodicidade, sucessor->periodicidade, sizeof(raiz->periodicidade)-1);
            raiz->periodicidade[sizeof(raiz->periodicidade)-1] = '\0';

            raiz->tempoMin = sucessor->tempoMin;

            strncpy(raiz->horarioInicio, sucessor->horarioInicio, sizeof(raiz->horarioInicio)-1);
            raiz->horarioInicio[sizeof(raiz->horarioInicio)-1] = '\0';

            raiz->diaSemana = sucessor->diaSemana;
            raiz->demanda = sucessor->demanda;

            strncpy(raiz->apresentador, sucessor->apresentador, sizeof(raiz->apresentador)-1);
            raiz->apresentador[sizeof(raiz->apresentador)-1] = '\0';

            /* remove o sucessor */
            raiz->dir = prog_remover(raiz->dir, sucessor->nome, removeu);
        }
    }
    
    // Balanceia a árvore após a remoção
    return prog_balancear(raiz);
}

/* ========== FUNÇÕES AUXILIARES AVL ========== */

int prog_altura(Programa *no) {
    if (no == NULL) return 0;
    return no->altura;
}

int prog_fator_balanceamento(Programa *no) {
    if (no == NULL) return 0;
    return prog_altura(no->esq) - prog_altura(no->dir);
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

Programa* prog_rotacao_direita(Programa *y) {
    Programa *x = y->esq;
    Programa *T2 = x->dir;
    
    // Realiza a rotação
    x->dir = y;
    y->esq = T2;
    
    // Atualiza alturas
    y->altura = max(prog_altura(y->esq), prog_altura(y->dir)) + 1;
    x->altura = max(prog_altura(x->esq), prog_altura(x->dir)) + 1;
    
    // Retorna nova raiz
    return x;
}

Programa* prog_rotacao_esquerda(Programa *x) {
    Programa *y = x->dir;
    Programa *T2 = y->esq;
    
    // Realiza a rotação
    y->esq = x;
    x->dir = T2;
    
    // Atualiza alturas
    x->altura = max(prog_altura(x->esq), prog_altura(x->dir)) + 1;
    y->altura = max(prog_altura(y->esq), prog_altura(y->dir)) + 1;
    
    // Retorna nova raiz
    return y;
}

Programa* prog_rotacao_dupla_direita(Programa *z) {
    z->esq = prog_rotacao_esquerda(z->esq);
    return prog_rotacao_direita(z);
}

Programa* prog_rotacao_dupla_esquerda(Programa *z) {
    z->dir = prog_rotacao_direita(z->dir);
    return prog_rotacao_esquerda(z);
}

Programa* prog_balancear(Programa *no) {
    if (no == NULL) return no;
    
    // Atualiza altura do nó atual
    no->altura = max(prog_altura(no->esq), prog_altura(no->dir)) + 1;
    
    // Calcula fator de balanceamento
    int fb = prog_fator_balanceamento(no);
    
    // Caso esquerda-esquerda
    if (fb > 1 && prog_fator_balanceamento(no->esq) >= 0) {
        return prog_rotacao_direita(no);
    }
    
    // Caso esquerda-direita
    if (fb > 1 && prog_fator_balanceamento(no->esq) < 0) {
        return prog_rotacao_dupla_direita(no);
    }
    
    // Caso direita-direita
    if (fb < -1 && prog_fator_balanceamento(no->dir) <= 0) {
        return prog_rotacao_esquerda(no);
    }
    
    // Caso direita-esquerda
    if (fb < -1 && prog_fator_balanceamento(no->dir) > 0) {
        return prog_rotacao_dupla_esquerda(no);
    }
    
    return no;
}
