#ifndef _MENU_REF_H_
#define _MENU_REF_H_


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<signal.h>
#include<pthread.h>

#include "stock.h"
#include "registo.h"
#include "sincronizacao.h"
#include "mailbox.h"

/* 
 * Prototipos
 */
void *Menu_Ref(void *argumento);
void Escreve_Menu_Ref(void);
void Abastece(char* produto, char* quantidade); 
void ListaPedidosPendentes(int verb); 
void ListaStock(int verb); 
void TerminaRefeitorio(int arg); 

#endif
