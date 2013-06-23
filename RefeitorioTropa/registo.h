#ifndef _REGISTO_H_
#define _REGISTO_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>

#include "sock_dgram.h"
#include "stock.h"
/*
 * Estruturas de dados
 */
#define	SOLDADO	1
#define OFICIAL	2
#define GENERAL	3
/*
 * Passou para stock.h
 * 
typedef struct PedidoStr
{
	Produto* prod;
	int	 n_prod;
}Pedido;
*/
typedef struct MilitarStr
{	
	unsigned id; 	/* PID do processo do Militar. Fica 0 quando este sai. */
	unsigned tipo;	/* Categoria do Militar (ver acima) */
	Pedido* pedido;	/* Pedido pendente do Militar. 
			 * (NULL se ja foi atendido) */
	pthread_t tid;	/* TID da thread que o atende */
}Militar;

typedef struct ClientesStr
{
	Militar* vector;
	int	 n_cli;
	int	 n_max;	
}Clientes;

/*
 * cabecalho das funcoes
 */
#define	N_CLI_INIT 20
#define N_CLI_ADD  10

int IdentificaSoldado(char* msg);
int IdentificaOficial(char *msg);

int GetIndiceMilitar(int pid);

void RegistaEntrada(int pid, int tipo);
void RegistaSaida(int pid);
void RegistaPedido(int pid, Pedido* pedido);

void IniciaClientes();
void ReiniciaClientes();
void MandaTodosSair();

#endif
