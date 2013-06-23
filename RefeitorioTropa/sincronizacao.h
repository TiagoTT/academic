#ifndef _SINCRONIZACAO_H
#define _SINCRONIZACAO_H

/*
 * Includes necessarios
 */
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<errno.h>

/*
 * Definicao de constantes
 * (identificadores dos mecanismos de sincronizacao)
 */

#define ENTRADA		1
#define SAIDA		2
#define PRESENTES	3
#define SERVIR_JANTAR	4
#define	CONTAGEM	5
#define SENTIDO		6
#define	PEDIDOS		7
#define MUTEX_INDICE_ESCOLHA 8
#define INDICE_ESCOLHA  9
#define SEM_LINHAS     10
#define SEM_COLUNAS    11
#define BARREIRA_LE    12
#define BARREIRA_SERVE 13
#define SOLDADOS       14
#define PEDIDOS_PRONTOS 15
/*
 * Definicao de estruturas com mecanismos 
 * de sincronizacao integrados
 */



/*
 * Prototipos de funcoes
 */

void CriaSemaforo(int key);
void DefineSemaforo(int key, int valor);
int  DevolveSemaforo(int key);
void AssinalaSemaforo(int key);
void EsperaSemaforo(int key);
void EsperaSemaforoZero(int key);
void DestroiSemaforo(int key);

#endif
