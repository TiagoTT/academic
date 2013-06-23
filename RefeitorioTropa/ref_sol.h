/*
 * Processo Refietorio
 * 
 * Ficheiro: ref_sol.h
 */

#ifndef _REF_SOL_H
#define _REF_SOL_H


#include <pthread.h>
#include "stock.h"
/*
 * Estruturas de dados
 */

/*
 * Prototipos de funcoes
 */
void * AtendeSoldados(void *argumento);
void * ThreadSoldado(void *argumento);
void MenusFicheiroParaMemoriaPartilhada(char* ficheiro, int key);

int RetiraPedido(int pid);
Pedido* ProcessaMenuSoldado( int menu );
char* CriaResposta(int menu);

#endif
