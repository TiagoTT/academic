/*
 * Ficheiro: io.h
 */

#ifndef _io_h
#define _io_h

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

FILE* abreFicheiro(char* nome, char* modo, char* msgErro);
void fechaFicheiro(FILE* fp, char* msgErro);

#endif
