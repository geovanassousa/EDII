#ifndef TIPOS_H
#define TIPOS_H

// tamanhos padrão pra strings (nome, site, etc.);
#define TXT_PEQ   32   // ex.: rótulos curtos (tipo, hora "HH:MM")
#define TXT_MED   64   // ex.: nomes de categoria/programa menores
#define TXT_GRD  128   // ex.: nomes de stream/apresentador, URLs

// tipo de categoria usada (notícias, esporte, entretenimento)
// vai imprimir esses valores em um menu
typedef enum {
    TIPO_NOTICIAS = 1,
    TIPO_ESPORTE  = 2,
    TIPO_ENTRETENIMENTO = 3
} TipoCategoria;

// demanda do programa: ao vivo ou sob demanda 
typedef enum {
    DEMANDA_AO_VIVO = 1,
    DEMANDA_SOB_DEMANDA = 2
} TipoDemanda;

#endif
