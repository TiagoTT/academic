/*
 * weigrr.c
 *
 * WRR- Weighted Round Robin
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "simulador.h"


void WRRPC( Evento* ev, pq fila ) {
	double servico;

#ifdef DEBUG
	/* printf( "Disciplina WRR, processando pacote:\n" ); */
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

void WRRPP( Evento* ev, pq fila ){
	Evento* prox;
	double servico;
	int i = 0;
	int mesma = 0;
	int outra = 0;

#ifdef DEBUG
	/* printf( "Disciplina WRR, processando pacote:\n" ); */
	PrintEvento( NULL, ev );
#endif

	/* O router acabou de servir este pacote. 
	 * Se houver pacotes em espera, escolhemos um para enviar. 
	 * Se nao houver pacotes para enviar, o router fica livre.
	 */

	/* Determina qual a proxima sessao a ser servida. */
	for( i = ( ev -> r -> servico + 1 )% ROUND_SIZE ; 
			i != ev -> r -> servico ; 
			i = ( i + 1 )% ROUND_SIZE) {
		if( NULL != 
				ConsultaInicio( ev -> r -> filas[ ev -> r -> ronda[ i ] ] ) ) {
			outra = 1;
			break;
		}
	}
	/* Caso nao haja mais sessoes activas tentamos servir a mesma sessao. */
	if( ( outra == 0 ) &&
			NULL != ConsultaInicio( ev -> r -> filas[ ev -> r -> ronda[ i ] ] ) ) {
		mesma = 1;
		i = ev -> r -> servico;
	}
	/* A variavel "i" fica com a posicao actual na ronda de servico. */


	/* Ha pelo menos um pacote para enviar. */
	if( outra || mesma ) {
		/* O router continua ocupado. */
		ev -> r -> livre = 0;
		/* Actualizacao da posicao actual na ronda. */
		ev -> r -> servico = i;

		/* O proximo evento corresponde a enviar o proximo pacote. */
		prox = RemoveDoInicio( ev -> r -> filas[ ev -> r -> ronda[ i ] ] );

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
		/* Contabilizacao do atraso do pacote. */
		ActualizaAtrasoPacotes( ev -> s, ev -> tempo - ev -> t_zero );
		/* O pacote foi entregue. */
		ApagaEvento( ev );
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
