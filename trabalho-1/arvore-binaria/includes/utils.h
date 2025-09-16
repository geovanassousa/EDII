#ifndef UTILS_H
#define UTILS_H

#include "tipos.h"

/* Funções utilitárias para o programa */

// Compara duas strings ignorando diferenças entre maiúsculas e minúsculas
int str_cmp_i(const char *a, const char *b);

// Converte número inteiro para enum TipoCategoria correspondente
int parse_categoria_num(int v);

// Converte número do dia da semana (1-7) para texto abreviado
void dia_semana_para_texto(int dia, char *texto, int tamanho);


#endif
