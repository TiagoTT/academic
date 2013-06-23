/*
 * defrr.c
 *
 * DRR- Deficit Round Robin
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "simulador.h"

int JaPodeServirSessao( int i, Router* r );
int AindaPodeServirSessao( int i, Router* r );
int AlgumaFilaNaoVazia( Router* r );

/* Processa chegada */
void DRRPC( Evento* ev, pq fila ) {
	double servico;

#ifdef DEBUG	
	/* printf( "Disciplina DRR, processando pacote:\n" ); */
	PrintEvento( NULL, ev );
#endif

	/* Se o router estiver livre, o pacote pode ser enviado ja.*/
	if( ev -> r -> livre ) {
		/* O router passa estar ocupado. */
		ev -> r -> livre = 0;

		servico = /* Tempo de servico. */
			(double)(ev -> s -> TamanhoPacote) / 
			(double)(ev -> r -> capacidade) ;
		/* Tempo de transmissao.*/
		ev -> tempo += servico;
		/* Acumulacao do tempo de actividade do Router. */
		ev -> r -> actividade += servico;

		ev -> tipo = PARTIDA;

		/* Reinsere o envento na fila. */
		ColocaNaFila( fila, ev  );
	}
	else { /* O pacote vai ser colocado na fila de espera da sua sessao. */
		if( 0 > ColocaNoFim( ev -> r -> filas[ ev -> s -> id ], ev ) ) {
			fprintf( stdout, "Evento perdido!\n" );
			ApagaEvento( ev );
		}
		else {
			/* Actualiza estatisticas. */
			ActualizaOcupacaoFilas( ev -> tempo, ev -> s, ev -> r, +1 );
		}
	}

}

void DRRPP( Evento* ev, pq fila ){
	Evento* prox;
	double servico;
	int i = 0;
	int flag = 0;

#ifdef DEBUG
	/* printf( "Disciplina DRR, processando pacote:\n" ); */
	PrintEvento( NULL, ev );
#endif

	/* O router acabou de servir este pacote. 
	 * Se houver pacotes em espera, escolhemos um para enviar. 
	 * Se nao houver pacotes para enviar, o router fica livre.
	 */

	/* Tentamos servir a mesma sessao. */
	if( 0 > ( i = AindaPodeServirSessao( ev -> r -> servico, ev -> r ) ) ) {
		/* Ja nao podemos continuar a servir a mesma sessao. */
		do{
			/* Visitamos todas as sessoes ate' encontrar uma 
			 * com pacote para enviar, e com direito a envia-lo. */
			i = ev -> r -> servico ;

			do{
				i = (i + 1) % N_SESSOES;
				/* Verificamos se a sessao ja esta' em condicoes
				 * de ser servida. */
				if( ! (0 > JaPodeServirSessao( i, ev -> r ) ) ) {
					/* Temos um pacote para enviar. */
					flag = 1;
					break;
				}
			}while( i != ev -> r -> servico );

			/* Se ha sessoes com pacotes para enviar, mas
			 * ainda nao tem direito para isso. */
		}while( AlgumaFilaNaoVazia( ev -> r ) && !flag );

		/* Se nao encontramos pacotes para enviar. */
		if( !flag ) {
			i = -1;
		}
	}

	/* Ha pelo menos um pacote para enviar. */
	if( !(0 > i) ) {
		/* O router continua ocupado. */
		ev -> r -> livre = 0;
		/* Actualizacao da posicao actual na ronda. */
		ev -> r -> servico = i;

		/* O proximo evento corresponde a enviar o proximo pacote. */
		prox = RemoveDoInicio( ev -> r -> filas[ i ] );

		/* Actualiza estatisticas. */
		ActualizaOcupacaoFilas( ev -> tempo, prox -> s, ev -> r, -1 );

		/* Tempo de servico.*/
		servico =
			(double)(prox -> s -> TamanhoPacote) / 
			(double)(prox -> r -> capacidade) ;

		prox -> tempo = 
			/* Tempo actual. */
			ev -> tempo +
			/* Tempo de transmissao */
			servico;

		/* Acumulacao do tempo de actividade do Router. */
		ev -> r -> actividade += servico;

		prox -> tipo = PARTIDA;

		/* Reinsere o envento na fila. */
		ColocaNaFila( fila, prox );
	}
	else {
		/* O router ficou livre. */
		ev -> r -> livre = 1;
	}

	/* Se nao ha proximo router, entao chegou ao fim do percurso. */
	if( NULL == ev -> r -> next ) {

#ifdef DEBUG
		printf( "Pacote chegou.\n" );
#endif

		/* Acumulacao do numero de pacotes entregues. */
		ev -> s -> estatisticas -> PacotesEntregues1seg ++;
		ApagaEvento( ev );
		/* Contabilizacao do atraso do pacote. */
		ActualizaAtrasoPacotes( ev -> s, ev -> tempo - ev -> t_zero );
		return;
	}
	/* O tempo de transmicao ja terminou. O pacote vai chegar ao proximo
	 * router apos o atraso de propagacao (TAU).
	 */
	ev -> tempo += ev -> r -> atraso;
	ev -> tipo = CHEGADA;
	ev -> r = (Router*)ev -> r -> next;

	/* Reinsere o envento na fila. */
	ColocaNaFila( fila, ev  );

}

int JaPodeServirSessao( int i, Router* r ) {
	Evento* prox;

	/* Testamos se a fila de espera tem pacotes. */
	if( NULL == ( prox = ConsultaInicio( r -> filas[ i ] ) ) ) {
		/* A fila esta' vazia, a sessao esta' inactiva. */
		r -> DeficitCounter[ i ] = 0;
		/* Nao se pode servir esta sessao. */
		return -1;
	}

	/* Testamos se o pacote pode ser enviado. */
	if( prox -> s -> TamanhoPacote <= 
			r -> DeficitCounter[ i ] +
			r -> quantum * prox -> s -> prioridade ) {
		/* O pacote pode ser servido. */
		r -> DeficitCounter[ i ] +=
			r -> quantum * prox -> s -> prioridade -
			prox -> s -> TamanhoPacote;
		return i;
	}
	else{
		/* Esta sessao ainda nao pode ser servida. */
		r -> DeficitCounter[ i ] += 
			r -> quantum * prox -> s -> prioridade;
		return -1;
	}

	return -1;
}

int AindaPodeServirSessao( int i, Router* r ) {
	Evento* prox;

	/* Testamos se a fila de espera tem pacotes. */
	if( NULL == ( prox = ConsultaInicio( r -> filas[ i ] ) ) ) {
		/* A fila esta' vazia, a sessao esta' inactiva. */
		r -> DeficitCounter[ i ] = 0;
		/* Nao se pode servir esta sessao. */
		return -1;
	}

	/* Testamos se o pacote pode ser enviado. */
	if( prox -> s -> TamanhoPacote <= r -> DeficitCounter[ i ] ) {
		/* O pacote pode ser servido. */
		r -> DeficitCounter[ i ] -= prox -> s -> TamanhoPacote;
		return i;
	}

	return -1;
}

int AlgumaFilaNaoVazia( Router* r ) {
	int i;

	for( i = 0 ; i < N_SESSOES ; i++ ) {
		if( NULL != ConsultaInicio( r -> filas[ i ] ) ) {
			return 1;
		}
	}

	return 0;
}
