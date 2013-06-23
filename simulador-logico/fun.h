/*
 * Ficheiro: fun.h
 */

#ifndef _fun_h
#define _fun_h

#include <stdio.h>

#include "cir.h"

#define N_TIPOS   6  /* Numero de tipos de funcoes logicas. */
#define N_VALORES 5  /* Numero de valores logicos. */

/* Valores que representam os valores logicos. */
#define ZERO 0
#define ONE  1
#define X    2
#define D    3
#define DBAR 4

void calculaValor(Circuito* circuito, int node);

#endif
