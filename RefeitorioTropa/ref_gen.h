/*
 * Processo Refietorio
 * 
 * Ficheiro: ref_gen.h
 */

#ifndef _REF_GEN_H
#define _REF_GEN_H

/*
 * Includes necessarios
 */
#include <pthread.h>
#include <string.h>
/*
 * Estruturas de dados
 * 
 */


/*
 * Prototipos de funcoes
 */
void * AtendeGenerais(void *argumento);
void *ThreadGeneral(void * arg);
Pedido* ProcessaMsgGeneral( char* msg );

#endif
