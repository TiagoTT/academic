#ifndef _MEMORIA_H_
#define _MEMORIA_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <assert.h>

/*
 * Estruturas de dados
 */
#define COLUNAS_MENUS 40

typedef struct MenusStr
{
	char*	texto;
	int	linhas;
        int	colunas;
}Menus;

typedef struct EscolhaStr
{
	int	pid;
	int	menu;
}Escolha;

#define SHM_MENUS   321
#define SHM_PEDIDOS 123

/*
 * cabecalho das funcoes
 */
void* CriaMemoriaPartilhada(int key, int n_bytes);
Escolha* AcedeMemoriaPedidos();
Menus AcedeMemoriaMenus();
void EscreveMemoriaPedido(int pid, int pedido);
int GetIndiceEscolha();
void FechaMemoriaPartilhada(int key);

#endif
