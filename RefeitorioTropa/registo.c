/* 
 * Processo Refeitorio
 * Ficheiro: registo.c
 */
#include "sock_dgram.h"

#include "registo.h"

extern Clientes clientes;

int IdentificaSoldado(char* msg)
{
	int pid;

	sscanf( msg, "Apresenta-se o Soldado %d", &pid);
	
	return pid;
}

int IdentificaOficial(char* msg)
{
	int pid;
	
	sscanf( msg, "Apresenta-se o Oficial %d", &pid);

	return pid;
}

int GetIndiceMilitar(int pid)
{
	int i;

	for( i = 0 ; i < clientes.n_cli ; i++ )
	{
		if( clientes.vector[i].id == pid )
		{
			return i;
		}
	}

	return -1;
}

void RegistaEntrada(int pid, int tipo)
{
	Militar novo;

	novo.id = pid;
	novo.tipo = tipo;
	novo.pedido = NULL;
	
	/* Se nao houver espaco, adiciona 10 posicoes ao vector. */
	if( clientes.n_cli >= clientes.n_max )
	{	
		clientes.n_max += N_CLI_ADD;
		clientes.vector = (Militar*)realloc( clientes.vector, sizeof(Militar)*(clientes.n_max) );
	}

	/* Introduz o novo cliente no vector. */
	clientes.vector[ clientes.n_cli ] = novo;
	clientes.n_cli ++;
	
	return;
}

void RegistaSaida(int pid)
{
	int i;

	for( i = 0 ; i < clientes.n_cli ; i++ )
	{
		if( clientes.vector[i].id == pid )
		{
			clientes.vector[i].id = 0;
			return;
		}
	}
	
	fprintf(stderr, "O militar %d nao esta no Refeitorio.\n", pid);
	
	return;
}

void MandaTodosSair()
{
	int i;
	
	for( i = 0 ; i < clientes.n_cli ; i++ )
	{
		if( clientes.vector[i].id )
		{
			kill( clientes.vector[i].id, SIGINT );
			clientes.vector[i].id = 0;
		}
	}

	return;
}

void IniciaClientes()
{
	clientes.vector = ( Militar* )malloc( sizeof( Militar )*N_CLI_INIT );
	clientes.n_cli = 0;
	clientes.n_max = N_CLI_INIT;
	
	return;
}

void ReiniciaClientes()
{
	int i, n_ofi;
	Militar* tmp;
	
	tmp = ( Militar* )malloc( sizeof( Militar )*clientes.n_cli );
	
	/* Guarda a informacao sobre os Oficiais que ficaram. */
	for( i = 0, n_ofi = 0 ; i < clientes.n_cli ; i++ )
	{
		if( clientes.vector[i].id )
		{
			tmp[ n_ofi ] = clientes.vector[i];
			n_ofi++;
		}
	}	
	
	/* Reajusta os valores e o Vector. */
	clientes.n_max = (n_ofi > N_CLI_INIT) ?
		(1+n_ofi/N_CLI_ADD)*N_CLI_ADD:
		N_CLI_INIT;
	clientes.vector = (Militar*)realloc( clientes.vector, sizeof(Militar)*(clientes.n_max) );
	clientes.n_cli = n_ofi;
	
	/* Repoe os Oficiais no Vector. */
	for( i = 0 ; i < clientes.n_cli ; i++ )
	{
		clientes.vector[i] = tmp[i];
	}
	
	return;
}
