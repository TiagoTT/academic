/*
 * ajustes.h
 *
 * Ficheiro com constantes de ajuste.
 */

/* Constantes multiplicativas que afectam o tamanho dos pacotes
 * para obter variacoes nos debitos requeridos por cada sessao.
 */
#define AJUSTE 0

#if AJUSTE

#define AJUSTE_PACOTE_S1 1.3
#define AJUSTE_PACOTE_S2 1.8
#define AJUSTE_PACOTE_S3 1

#else

#define AJUSTE_PACOTE_S1 1
#define AJUSTE_PACOTE_S2 1
#define AJUSTE_PACOTE_S3 1

#endif

