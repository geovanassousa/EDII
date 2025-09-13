#ifndef TIPOS_H
#define TIPOS_H

#define TXT_PEQ   32
#define TXT_MED   64
#define TXT_GRD  128

typedef enum {
    TIPO_NOTICIAS = 1,
    TIPO_ESPORTE  = 2,
    TIPO_ENTRETENIMENTO = 3
} TipoCategoria;

typedef enum {
    DEMANDA_AO_VIVO = 1,
    DEMANDA_SOB_DEMANDA = 2
} TipoDemanda;

#endif
