#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../includes/utils.h"

int str_cmp_i(const char *a, const char *b) {
    unsigned char ca, cb;
    int i = 0;
    int dif = 0;
    while (dif == 0 && a[i] != '\0' && b[i] != '\0') {
        ca = (unsigned char) tolower((unsigned char)a[i]);
        cb = (unsigned char) tolower((unsigned char)b[i]);
        if (ca < cb) {
            dif = -1;
        } else if (ca > cb) {
            dif = 1;
        } else {
            /* iguais aqui */
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

void ler_linha(char *buf, int max) {
    if (fgets(buf, max, stdin) != NULL) {
        size_t n = strlen(buf);
        if (n > 0 && buf[n-1] == '\n') buf[n-1] = '\0';
    }
}

int parse_demanda_num(int v) {
    if (v == 1) return DEMANDA_AO_VIVO;
    if (v == 2) return DEMANDA_SOB_DEMANDA;
    return 0;
}

int parse_categoria_num(int v) {
    if (v == 1) return TIPO_NOTICIAS;
    if (v == 2) return TIPO_ESPORTE;
    if (v == 3) return TIPO_ENTRETENIMENTO;
    return 0;
}
