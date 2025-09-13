#ifndef UTILS_H
#define UTILS_H

#include "tipos.h"

/* compara strings ignorando maiúsc/minúsc; retorna <0, 0, >0 */
int str_cmp_i(const char *a, const char *b);

/* Lê linha segura (remove \n), tamanho max inclui '\0' */
void ler_linha(char *buf, int max);

/* Converte "1"/"2" para TipoDemanda; 0 = inválido */
int parse_demanda_num(int v);

/* Converte "1"/"2"/"3" para TipoCategoria; 0 = inválido */
int parse_categoria_num(int v);

#endif
