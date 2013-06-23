/*
 * weigfq.c
 *
 * Weighted Fair Queueing
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "simulador.h"


void ActualizaRoundNumber( Evento* ev, Sessao* s );
double CalculaRoundNumber( Evento *ev );
double CalculaFinishNumber( Evento *ev );

void WFQPC( Evento* ev, pq fila ) {

	double servico;
#ifdef DEBUG
	/* printf( "Disciplina WFQ, processando pacote:\n" ); */
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

		/* Verifica se a sessao estava inactiva e ficou agora activa. */
		if( NULL == ConsultaInicio( ev -> r -> filas[ ev -> s -> id ] ) ) {
			/* A sessao ficou activa agora. */
			ActualizaRoundNumber( ev, ev -> s );
		}

		if( 0 > ColocaNoFim( ev -> r -> filas[ ev -> s -> id ], ev ) ) {
			fprintf( stdout, "Evento perdido!\n" );
			ApagaEvento( ev );
		}
		else {
			ev -> r -> FinishNumber[ ev -> s -> id ] =
				CalculaFinishNumber( ev );

			/* Actualiza estatisticas. */
			ActualizaOcupacaoFilas( ev -> tempo, ev -> s, ev -> r, +1 );
		}
	}


}

void WFQPP( Evento* ev, pq fila ){
	Evento* prox;
	Evento* topo;
	Evento* auxtopo;
	double servico;
	int i = 0;
	int IndiceSessaoServida = 0;

#ifdef DEBUG
	/* printf( "Disciplina WFQ, processando pacote:\n" ); */
	PrintEvento( NULL, ev );
#endif
	/* O router acabou de servir este pacote. 
	 * Se houver pacotes em espera, escolhemos um para enviar. 
	 * Se nao houver pacotes para enviar, o router fica livre.
	 */

	/* Determina qual a proxima sessao a ser servida. */
	for( i = 1, IndiceSessaoServida = 0,
			topo = ConsultaInicio ( ev -> r -> filas[ 0 ] ) 
			; i < N_SESSOES ; i++ ) {
		/* Se a fila[i] estiver vazia, continua para testar a proxima. */
		if( NULL == ( auxtopo = ConsultaInicio ( ev -> r -> filas[ i ] ) ) ) {
			continue;
		}
		/* Se o topo actual for NULL, ficamos com o novo e continuamos. */
		if( NULL == topo ) {
			topo = auxtopo;
			IndiceSessaoServida = i;
			continue;
		}
		/* Temos de comparar os FinishNumbers dos dois topos. */
		if( auxtopo -> FinishNumber < topo -> FinishNumber ) {
			topo = auxtopo;
			IndiceSessaoServida = i;
		}
	}

	/* Se ha pelo menos um pacote para enviar. */
	if( NULL != topo ) {
		/* O router continua ocupado. */
		ev -> r -> livre = 0;

		/* O proximo evento corresponde a enviar o proximo pacote. */
		prox = RemoveDoInicio( ev -> r -> filas[ IndiceSessaoServida ] );

		/* Verifica se a sessao ficou inactiva. */
		if( NULL == ConsultaInicio( ev -> r -> filas[ IndiceSessaoServida ] ) ) {
			ActualizaRoundNumber( ev, prox -> s );
		}

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
	else{
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

void ActualizaRoundNumber( Evento* ev, Sessao* s ){

	/* Regista o valor actual do Round Number e o tempo actual:
	 * 	R(t_zero)
	 * 	t_zero
	 */
	ev -> r -> RoundNumberT_Zero = ( 0 != ev -> r -> SomatorioPesos ) ?
		CalculaRoundNumber( ev ) :
			0.0 ;
	ev -> r -> T_Zero = ev -> tempo;

	/* Actualiza o somatorio dos pesos das sessoes activas. */
	switch( ev -> tipo ) {
		case CHEGADA :
			/* Se se trata de uma chegada de um pacote a um router,
			 * temos de considerar que a sessao ficou activa.
			 */
			ev -> r -> SomatorioPesos += s -> prioridade;
			break;
		case PARTIDA :
			/* Se se trata de um partida de um pacote de um router,
			 * temos de considerar que a sessao ficou inactiva.
			 */
			ev -> r -> SomatorioPesos -= s -> prioridade;
			/* Verifica se o router ficou sem sessoes activas. */
			if( 0 == ev -> r -> SomatorioPesos ) {
				ev -> r -> RoundNumberT_Zero = 0.0;
			}
			break;
	}
#ifdef DEBUG
	fprintf( stdout, "%f %f (RoundNumber router: %d)\n",
			ev -> r -> T_Zero,
			ev -> r -> RoundNumberT_Zero,
			ev -> r -> id + 1 );
#endif
}

double CalculaRoundNumber( Evento *ev){

	double RoundNumber = 0.0;

	RoundNumber = (ev -> r -> capacidade) / ( ev -> r -> SomatorioPesos ) *
		(ev -> tempo - ev -> r -> T_Zero) + 
		ev -> r -> RoundNumberT_Zero;

	return RoundNumber;
}


double CalculaFinishNumber( Evento *ev){

	double FinishNumber = 0.0;
	double MaxFR = 0.0;
	double RoundNumber = 0.0;

	RoundNumber = CalculaRoundNumber(ev);

	if( (ev->r->FinishNumber[ev->s->id] ) < RoundNumber ){

		MaxFR = RoundNumber;
	}
	else{

		MaxFR = ev->r->FinishNumber[ev->s->id]  ;
	}

	FinishNumber = MaxFR + (ev->s->TamanhoPacote)/(ev->s->prioridade); 

	ev -> FinishNumber = FinishNumber ;

	return FinishNumber;

} 


