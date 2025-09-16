#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../includes/utils.h"

int str_cmp_i(const char *a, const char *b) {
    unsigned char ca, cb;
    int i = 0;
    int dif = 0;
    /* compara char a char ignorando maiúsculas/minúsculas */
    while (dif == 0 && a[i] != '\0' && b[i] != '\0') {
        ca = (unsigned char) tolower((unsigned char)a[i]);
        cb = (unsigned char) tolower((unsigned char)b[i]);
        if (ca < cb) {
            dif = -1;
        } else if (ca > cb) {
            dif = 1;
        } else {
        }
        i = i + 1;
    }
    if (dif == 0) {
        if (a[i] == '\0' && b[i] == '\0') {
            dif = 0;
        } else if (a[i] == '\0') {
            dif = -1;
        } else {
            dif = 1;
        }
    }
    return dif;
}

int parse_categoria_num(int v) {
    if (v == 1) return TIPO_NOTICIAS;
    if (v == 2) return TIPO_ESPORTE;
    if (v == 3) return TIPO_ENTRETENIMENTO;
    return 0;
}

void dia_semana_para_texto(int dia, char *texto, int tamanho) {
    if (dia == 1) strncpy(texto, "Dom", tamanho-1);
    else if (dia == 2) strncpy(texto, "Seg", tamanho-1);
    else if (dia == 3) strncpy(texto, "Ter", tamanho-1);
    else if (dia == 4) strncpy(texto, "Qua", tamanho-1);
    else if (dia == 5) strncpy(texto, "Qui", tamanho-1);
    else if (dia == 6) strncpy(texto, "Sex", tamanho-1);
    else if (dia == 7) strncpy(texto, "Sab", tamanho-1);
    else strncpy(texto, "Diario", tamanho-1); /* 0 = qualquer dia */
    texto[tamanho-1] = '\0';
}

