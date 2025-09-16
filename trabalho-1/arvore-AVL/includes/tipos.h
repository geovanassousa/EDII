#ifndef TIPOS_H
#define TIPOS_H

/* definicao dos tamanhos das strings utilizadas no programa */ 
#define TXT_PEQ   32   // para textos curtos como tipo e horário
#define TXT_MED   64   // para nomes de categoria e programa
#define TXT_GRD  128   // para nomes de stream, apresentador e URLs

/* tipos de categoria disponiveis para selecao */

typedef enum {
    TIPO_NOTICIAS = 1,
    TIPO_ESPORTE  = 2,
    TIPO_ENTRETENIMENTO = 3
} TipoCategoria;

/* tipos de demanda do programa: ao vivo ou sob demanda */ 
typedef enum {
    DEMANDA_AO_VIVO = 1,
    DEMANDA_SOB_DEMANDA = 2
} TipoDemanda;

#endif
