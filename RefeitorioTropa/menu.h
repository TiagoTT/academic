#ifndef _REF_MENU_H_
#define _REF_MENU_H_

#include <string.h>
#include "stock.h"

/* 
 * Prototipos
 */
void *Menu(void *arg);

void Abastece(char* produto, int quantidade);
void ListaPedidosPendentesVerboso(); 
void ListaPedidosPendentes(); 
void ListaStockVerboso();
void ListaStock(); 
void TerminaRefeitorio(); 

#endif
