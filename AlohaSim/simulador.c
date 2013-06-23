/*
 * simulador.c
 * 
 * Simulador de eventos discretos generico.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "lcgrand.h"

#include "queue.h"
#include "simulador.h"
#include "disciplinas.h"

int MAX_TIME = 170;
int TEMPO_GLOBAL = 0;

int INACTIVIDADE_GLOBAL = 0;
int SUCESSO_GLOBAL = 0;
int COLISOES_GLOBAL = 0;

int main( int argc, char* argv[] ) {
	double lambda, p;


	if( 3 == argc ) {
		lambda = atof( argv[ 2 ] );
		p = atof( argv[ 1 ] );
		
		lcgrandst( 281544088, 1);
		Simula( p, lambda, 1 );

		exit(0);
	}
	else if( 2 == argc ) {
		p = atof( argv[ 1 ] );

		for( lambda = LAMBDA_STEP ; lambda < LAMBDA_MAX ; lambda += LAMBDA_STEP ) {
			lcgrandst( 281544088, 1);
			Simula( p, lambda, 1 );
			putchar('\n');
		}
		exit(0);
	}
	else if( 4 == argc ) {
		lambda = atof( argv[ 1 ] );

		for( p = P_STEP ; p < 1 ; p += P_STEP ) {
			lcgrandst( 281544088, 1);
			Simula( p, lambda, 1 );
			putchar('\n');
		}
		exit(0);
	}
	else{
		for( p = P_STEP ; p < 1 ; p += P_STEP ) {
			for( lambda = LAMBDA_STEP ; lambda < LAMBDA_MAX ; lambda += LAMBDA_STEP ) {
				lcgrandst( 281544088, 1);
				Simula( p, lambda, 0 );
			}
			putchar('\n');
		}
		exit(0);
	}
	exit( 0 );
}

void Simula( double p, double lambda, int verbose ) {
  Estacao* es[ N_ESTACOES ];
  pq FilaEventos;
  int i;
  char NomeFicheiro[ MAXLINE ];
  float delay;
  float queue;
  
  TEMPO_GLOBAL = 0;
  INACTIVIDADE_GLOBAL = 0;
  SUCESSO_GLOBAL = 0;
  COLISOES_GLOBAL = 0;

  if( verbose ) {
  	printf( "\nSimulacao de um canal ALOHA sincronizado: p=%1.3f lambda=%1.3f \n",
		p, lambda );
  }

  /* Inicializar o simulador. */
  FilaEventos = IniciaFila( PQ_SIZE );
  for( i = 0 ; i < N_ESTACOES ; i++ ) {
  	snprintf( NomeFicheiro, MAXLINE,
		"resultados/p%1.3f_lambda%1.3f_fila%0000000002d.txt",
		 p, lambda, i+1 );
  	es[ i ] = NovaEstacao( i, NomeFicheiro );
  }

  /* Gera um evento chegada para cada estacao  */
  for( i = 0 ; i < N_ESTACOES ; i++ ) {
    ColocaNaFila( FilaEventos,
    			(void*)NovoEvento( - log( lcgrand(1) / lambda ),
			CHEGADA, 
			es[i] ) );

    ColocaNaFila( FilaEventos, (void*)NovoEvento( 0, OCUPACAO_FILAS, es[i]) ); 
  }
  
  /* Gera um inicio de slot */ 
  ColocaNaFila( FilaEventos, (void*)NovoEvento( 0, PARTIDA, NULL));

  /* Ciclo de simulacao. */
  while( ExecutaEvento( FilaEventos, es, lambda, p ) );

  /* Calcula ocupacao media das filas de espera. */
  for( i = 0, delay = 0, queue = 0 ; i < N_ESTACOES ; i++ ) {
	delay += es[ i ] -> atraso / es[ i ] -> sucesso;
	queue += es[ i ] -> estatisticas -> IntegralTotal / MAX_TIME;
  }
  delay /= N_ESTACOES;
  queue /= N_ESTACOES;
  
  if( verbose ) {
	  /* Apresentacao das estatisticas. */
	  for( i = 0, delay = 0 ; i < N_ESTACOES ; i++ ) {
		  PrintEstacao( NULL, es[ i ]  );
	  }
  }
  
  /* Limpeza do simulador. */
  ApagaFila( FilaEventos );

  for( i = 0 ; i < N_ESTACOES ; i++ ) {
    ApagaEstacao( es[ i ]  );
  }

  if( verbose ) {
	  printf( "Numero total de slots: %d\n", TEMPO_GLOBAL );
	  printf( "Numero de transmissoes com sucesso: %d slots (%f %%)\n",
			  SUCESSO_GLOBAL,
			  (float)SUCESSO_GLOBAL / (float)TEMPO_GLOBAL * 100 );
	  printf( "Numero de colisoes: %d slots (%f %%)\n",
			  COLISOES_GLOBAL,
			  (float)COLISOES_GLOBAL / (float)TEMPO_GLOBAL * 100 );
	  printf( "Inactividade: %d slots (%f %%)\n",
			  TEMPO_GLOBAL - ( SUCESSO_GLOBAL + COLISOES_GLOBAL),
			  (float)(TEMPO_GLOBAL - ( SUCESSO_GLOBAL + COLISOES_GLOBAL)) 
			  / (float)TEMPO_GLOBAL * 100);
  }
  else {
/*	printf( "%f\t%f\t%f\t%f\n", 
  		p,
		lambda * 10,
		(float)SUCESSO_GLOBAL / (float)TEMPO_GLOBAL,
	     	delay );
*/
/*  	printf( "%f;",(float)SUCESSO_GLOBAL / (float)TEMPO_GLOBAL );*/
/*	printf( "%f;", delay ); */
/*	printf( "%f;", queue ); */
  }
  return;
}

int ExecutaEvento( pq fila, Estacao** es, double lambda, double p ) {
  Evento *ev;

  if( 0 == fila -> n_items  ) {
    /* Se a fila de eventos estiver vazia, a simulacao deve acabar. */
    return 0;
  }

  ev = RetiraDaFila( fila );

  switch( ev -> tipo ) {

  case CHEGADA:
    /* Chega um pacote 'a estacao. */
    ProcessaChegada( ev, fila, lambda );
    break;

  case PARTIDA:
    /* Inicio de um slot de transmissao. */
    ProcessaPartida( ev, fila, es, p );
    TEMPO_GLOBAL++;
    break;

  case OCUPACAO_FILAS:
    /* Temos de registar a ocupacao das filas. META_EVENTO*/
    RegistaOcupacaoFilas( ev, fila );
    break;
  case DEBITO:
    /* Temos de registar o debito medio. META_EVENTO! */
    printf("erro no case Debito\n");
    exit(-1);
    break;
  }

  if( MAX_TIME > ev -> tempo ) {
	  /* A simulacao deve continuar. */
	  return 1;
  }
  else{
	  /* A simulacao vai parar.  */
	  return 0;
  }


}


/*
 * Para uma dada estacao, regista a ocupacao da fila de espera
 * correspondente
 */
void RegistaOcupacaoFilas( Evento* ev, pq fila ) {
	
	Estatistica* stat;
	FILE* output;
	
	stat = ev ->es -> estatisticas;
	output = ev ->es -> fila_output;
	
	/* Actualiza os integrais totais. */
	stat -> IntegralTotal += stat -> Ocupacao;

	/* Imprime resultado. */
	fprintf( output, "%f %d\n",
		ev -> tempo, 
		stat -> Ocupacao );
	/*	stat -> Integral ); */

	/* Reinicia o contador PacotesEntregues1seg. */
	stat -> Integral = 0.0;

	/* Reagenda o evento para daqui a um slot. */
	ev -> tempo += 1.0;
	
	/* Paragem! */
	if( ev -> tempo > MAX_TIME ) {
		ApagaEvento( ev );
		return;
	}

	/* Reinsere o evento na fila. */
	ColocaNaFila( fila, ev );

	return; 
}

/*
 * Actualiza os dados estatisticos das filas de espera FIFO.
 */
void ActualizaOcupacaoFila( float tempo, Estacao* es, int delta ) {
  	
	Estatistica* stat;
	
	stat = es -> estatisticas;
	
	/* Acumula a area de mais um troco do grafico da ocupacao da fila. */
	stat -> Integral +=
		( tempo - stat -> UltimaAlteracao ) * 
		( stat -> Ocupacao );
	
	/* Actualiza a ocupacao. */
	stat -> Ocupacao += delta;
	/* Actualiza o tempo da UltimaAlteracao. */
	stat -> UltimaAlteracao = tempo;

	/*
	fprintf( stdout, "Debug: dt: %1.10f  estacao: %d ocupacao: %d integral %f\n", 
			(ev -> tempo) - (stat -> UltimaAlteracao[ estacao_id ]), 
			estacao_id + 1,
			ev -> es -> id + 1,
			stat -> Ocupacao[ estacao_id ],
			stat -> Integral[ estacao_id ] );
	*/
	return;
}

/*
 * Cria, inicia e devolve uma estrutura de estatistica.
 */
Estatistica* NovaEstatistica( void ){
  Estatistica* stat;
  
  stat = (Estatistica*)malloc( sizeof( Estatistica ) );
  if( NULL == stat ){
    fprintf(stderr, "ERRO: malloc() = NULL in NovaEstatistica\n");
    exit( -1 );
  }


  stat -> Integral = 0.0;
  stat -> IntegralTotal = 0.0;
  stat -> UltimaAlteracao = 0.0;
  stat -> Ocupacao = 0;
  stat -> AtrasoMaximo = 0.0;  	
  

  return stat;
}


/*
 * Imprime uma estrutura Estatistica para o ficheiro output.
 */
void PrintEstatistica( FILE* output, Estatistica* e ) {
  FILE* fp;

  if( NULL != output ) fp = output;
  else fp = stdout;

  fprintf( fp, "AtrasoMaximo: %f s\n", e -> AtrasoMaximo );

}

/*
 * Liberta os recursos consumidos pela estrutura.
 */
void ApagaEstatistica( Estatistica* e ) {
  if( NULL != e )
    free( e );
}

/*
 * Cria, inicia e devolve uma estrutura de Estacao.
 */
Estacao* NovaEstacao( int id, char* ficheiro_fila_output ) {
  Estacao* es;
	
  es = (Estacao*)malloc( sizeof( Estacao ) );
  if( NULL == es ){
    fprintf(stderr, "ERRO: malloc() = NULL in NovaEstacao\n");
    exit( -1 );
  }

  es -> id = id;
  es -> fila = IniciaBuffer();

  if( NULL != ficheiro_fila_output ) {
  	es -> fila_output = fopen( ficheiro_fila_output, "w" );
  	if( NULL == es -> fila_output ){
    		fprintf(stderr, "ERRO: fopen() = NULL in NovaEstacao\n");
    		exit( -1 );
  	} 
  }
  else{
	es -> fila_output = NULL;
  }

  es -> estatisticas = NovaEstatistica();
  es -> sucesso = 0;
  es -> atraso = 0.0;

  return es;	
}

/*
 * Descreve a Estacao para o output.
 */
void PrintEstacao( FILE* output, Estacao* es ) {
  FILE* fp;

  if( NULL != output ) fp = output;
  else fp = stdout;

  fprintf( fp, "Estacao: %d activa: %d slots (%f %% do tempo) atraso medio: %f slots.\n",
		  es -> id +1 , 
		  es -> sucesso,
		  (float)es -> sucesso / (float)TEMPO_GLOBAL * 100.0,
		  es -> atraso / es -> sucesso);
}

/*
 * Liberta os recursos alocados para a estrutura.
 */
void ApagaEstacao( Estacao* e ) {

	if( NULL != e ) {
		while( NULL != ConsultaInicio( e->fila ) ) {
			ApagaEvento( RemoveDoInicio( e->fila ) );
		}
		ApagaBuffer( e -> fila );
		ApagaEstatistica( e -> estatisticas );
		if(  NULL != e -> fila_output ) {
			fclose( e -> fila_output );
		}
		free( e );
	}
}


/*
 * Cria, inicia e devolve uma estrutura de evento.
 */
Evento* NovoEvento( double tempo, char tipo, Estacao* es ) {
  Evento* ev;
	
  ev = (Evento*)malloc( sizeof( Evento ) );
  if( NULL == ev ){  
    fprintf(stderr, "ERRO: malloc() = NULL in NovoEvento\n");
    exit( -1 ); 
  }
  ev -> tempo = tempo;
  ev -> tipo = tipo;
  ev -> es = es;
  ev -> t_zero = tempo;
  
  return ev;
}

void PrintEvento( FILE* output, Evento* ev ) {
  FILE* fp;

  if( NULL != output ) fp = output;
  else fp = stdout;
	
  /* fprintf( fp, "ev->tempo = %f\n", ev -> tempo ); */
  fprintf( fp, "%f (t_zero: %f tipo:%d  estacao:%d)\n",
	  ev -> tempo, ev -> t_zero, ev -> tipo,
	  ( NULL != (ev -> es) ) ? (ev -> es -> id + 1) : 0 );

}

void ApagaEvento( Evento* ev ) {
	if( NULL != ev )
		free( ev );
}


/*
 *	EOF
 */
