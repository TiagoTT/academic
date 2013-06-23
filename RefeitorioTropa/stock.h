#ifndef _STOCK_H
#define _STOCK_H

/*
 * Includes necessarios
 */
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<assert.h>

/*
 * Definicao de constantes
 * (identificadores dos mecanismos de sincronizacao)
 */
#define PG_SZ sysconf(_SC_PAGESIZE)
#define	STOCK	123

/*
 * Definicao de estruturas com mecanismos 
 * de sincronizacao integrados
 */
#define	MAX_NOME	20
#define MAX_QUANT	5
typedef struct ProdutoStr
{
	/*
	 * estrutura que nos facilita na manipulaçao dos
	 * semaforos no acesso zona de memoria onde se 
	 * encontra o stock
	 * */
	char	nome[MAX_NOME];
	char	quantidade[MAX_QUANT];	
}Produto;

typedef struct StockStr
{
  int 	 fd;	/* Descritor do ficheiro de stock */
  Produto* inicio;/* Posicao inicial do mapeamento */
  Produto* fim;	/* Primeira posicao livre do mapeamento */
  size_t 	 dim;	/* Dimensao do mapeamento em bytes */
  int	 n_prod;/* Numero de Protudos no mapeamento */
  int      n_sem; /* Numero de produtos representados em semaforos */
}Stock;

typedef struct PedidoStr
{
	Produto* prod;
	int	 n_prod;
}Pedido;
/*
 * Prototipos de funcoes
 */

void CriaStock();
void AssinalaStock(Produto* produtos, int n_prod);
void EsperaStock(Produto* produtos, int n_prod);
void DestroiStock();

int DevolveValorStock(int indice);
void AdicionaProdMemMap(char* nome, char* quantidade);

short GetIndiceStock(char* nome);
void MapeiaFicheiroStock(char* nomefiche);
void DesMapeiaFicheiroStock();

#endif
