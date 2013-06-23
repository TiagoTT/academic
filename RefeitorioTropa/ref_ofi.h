/*
 * Processo Refietorio
 * 
 * Ficheiro: ref_ofi.h
 */

#ifndef _REF_OFI_H
#define _REF_OFI_H


/*
 * Estruturas de dados
 */


/*
 * Prototipos de funcoes
 */
void * AtendeOficiais(void *argumento);
void* ThreadOficiais(void* argumento);
void CancelaThreadOfic(void);
#endif
