/*
 * fifo.c
 *
 * FIFO- first in first out
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "queue.h"
#include "simulador.h"
#include "lcgrand.h"

extern int TEMPO_GLOBAL;
extern int SUCESSO_GLOBAL;
extern int COLISOES_GLOBAL;

/*
 * Codigo correspondente 'a chegada de um pacote a uma estacao.
 * O pacote e' colocado na fila de espera da estacao.
 * E' determinado aleatoriamente o instante da chegada do proximo pacote.
 *
 */
void ProcessaChegada( Evento* ev, pq fila, double lambda ) {
	Evento* prox;
	float prox_tempo;

#ifdef DEBUG
	/* printf( "Chegou o pacote:\n" ); */
	PrintEvento( NULL, ev );
#endif

	/* O pacote vai ser colocado na fila de espera. */
	if( 0 > ColocaNoFim( ev -> es -> fila, ev ) ) {
		/* A fila de espera esta' cheia e o pacote perdeu-se. */
		fprintf( stdout, "Pacote perdido!\n" );
		ApagaEvento( ev );
	}
	else {
		/* Actualiza estatisticas. */
		ActualizaOcupacaoFila( ev -> tempo, ev -> es, +1 );	
	}

	/* Dado o instante actual, vamos calcular o instante da chegada do 
	 * proximo pacote a esta estacao. */
	prox_tempo = ev -> tempo - log( lcgrand(1) ) / lambda;

	prox = NovoEvento( prox_tempo, CHEGADA, ev -> es );

	ColocaNaFila( fila, prox );
}

/*
 * Codigo correspondente ao inicio de um slot de tempo.
 * Determinam-se aleatoriamente quais as estacoes que vao transmitir.
 * Caso haja uma colisao, nao se alteram as filas de espera.
 * Caso nao haja colisao, e uma das estacoes tenha transmitido, retira-se
 * o pacote que foi transmitido.
 *
 */

void ProcessaPartida( Evento* ev, pq fila, Estacao** es, double p ) {
	Evento* enviado;
	int i, transmite, tentativas;

#ifdef DEBUG
	printf( "Inicio do slot: %d \n", TEMPO_GLOBAL );
	PrintEvento( NULL, ev );
#endif

	/* Determina quais as estacoes que vao transmitir. */
	for( i = 0, tentativas = 0, transmite = -1 ; i < N_ESTACOES ; i++ ) {

		/* Se a estacao tiver pacotes para enviar e 
		 * decidir transmitir agora. */
		if( ( NULL != ConsultaInicio(  es[ i ] -> fila ) )
				&& ( lcgrand(1) <= p ) ) {
			transmite = i;
			tentativas++;
		}
	}

	/* Se mais que uma estacao vai transmitir, temos colisao. */
	if( 1 == tentativas ) {
		/* Houve um pacote enviado com sucesso. */
		enviado = RemoveDoInicio( es[ transmite ] -> fila );

		/* Actualiza estatisticas. */
		es[ transmite ] -> sucesso ++;
		SUCESSO_GLOBAL ++;

		ActualizaOcupacaoFila( ev -> tempo, es[ transmite ], -1 );

#ifdef DEBUG
		PrintEvento( NULL, enviado );
		printf( "Pacote foi transmitido com sucesso.\n" );
#endif

		/* Contabilizacao do atraso do pacote. */
		es[ transmite ] -> atraso = 
			ev -> tempo - enviado -> t_zero ;
		
		/* O pacote foi entregue. */
		ApagaEvento( enviado );
	}
	else{
		/* Em caso de colisao nao se retiram pacotes das filas de espera.*/
		/* Actualizacao das estatisticas.*/
		if( tentativas > 1 ) {
			/* Ocorreu uma colisao. */
			COLISOES_GLOBAL ++;
		}
	}

	/* Em qualquer caso vamos agendar o proximo inicio de slot. */
	ev -> tempo += 1.0;
	ColocaNaFila( fila, ev );

	return;
}

/*
 *	EOF
 */
