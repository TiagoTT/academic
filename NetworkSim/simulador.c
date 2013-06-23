/*
 * simulador.c
 * 
 * Simulador de eventos discretos generico.
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "queue.h"
#include "simulador.h"
#include "disciplinas.h"
#include "ajustes.h"

int MAX_TIME = 0;
double TEMPO_GLOBAL = 0.0;

int main( int argc, char* argv[] ) {

	Router *r1, *r2, *r3;
	Sessao *s1, *s2, *s3;
	Disciplina *disc;
	pq FilaEventos;

	/* Inicializar o simulador. */
	FilaEventos = IniciaFila( PQ_SIZE );
	r3 = NovoRouter( 2, 2000000, 0.000, NULL );
	r2 = NovoRouter( 1,  500000, 0.010, r3 );
	r1 = NovoRouter( 0, 2000000, 0.010, r2 );

	/* Primeira Experiencia. */
	if( argv[1][0] == '1' ) {
#if AJUSTE
		MAX_TIME = 110;
#else
		MAX_TIME = 90;
#endif
		s1 = NovaSessao( 0, 3, "Exp_1_Sessao_1.txt", 8000*AJUSTE_PACOTE_S1,
				"exp1ses1deb.txt",
				"exp1ses1filas.txt" );
		s2 = NovaSessao( 1, 2, "Exp_1_Sessao_2.txt", 6400*AJUSTE_PACOTE_S2, 
				"exp1ses2deb.txt",
				"exp1ses2filas.txt" );
		s3 = NovaSessao( 2, 1, "Exp_1_Sessao_3.txt", 4800*AJUSTE_PACOTE_S3, 
				"exp1ses3deb.txt",
				"exp1ses3filas.txt" );

	}
	else if( argv[1][0] == '2' ) {
#if AJUSTE
		MAX_TIME = 12;
#else
		MAX_TIME = 10;
#endif
		s1 = NovaSessao( 0, 3, "Exp_2_Sessao_1.txt", 800*AJUSTE_PACOTE_S1,
				"exp2ses1deb.txt",
				"exp2ses1filas.txt" );
		s2 = NovaSessao( 1, 2, "Exp_2_Sessao_2.txt", 640*AJUSTE_PACOTE_S2,
				"exp2ses2deb.txt",
				"exp2ses2filas.txt" );
		s3 = NovaSessao( 2, 1, "Exp_2_Sessao_3.txt", 480*AJUSTE_PACOTE_S3,
				"exp2ses3deb.txt",
				"exp2ses3filas.txt" );
	}
	/* Coloca META EVENTOS na fila de eventos. */
	ColocaNaFila( FilaEventos, (void*)NovoEvento( 0, RECARREGA, s1, r1 ) );
	ColocaNaFila( FilaEventos, (void*)NovoEvento( 0, RECARREGA, s2, r1 ) );
	ColocaNaFila( FilaEventos, (void*)NovoEvento( 0, RECARREGA, s3, r1 ) );

	ColocaNaFila( FilaEventos, (void*)NovoEvento( 0, DEBITO, s1, NULL ) );
	ColocaNaFila( FilaEventos, (void*)NovoEvento( 0, DEBITO, s2, NULL ) );
	ColocaNaFila( FilaEventos, (void*)NovoEvento( 0, DEBITO, s3, NULL ) );

	ColocaNaFila( FilaEventos, (void*)NovoEvento( 0, OCUPACAO_FILAS, s1, NULL ) );
	ColocaNaFila( FilaEventos, (void*)NovoEvento( 0, OCUPACAO_FILAS, s2, NULL ) );
	ColocaNaFila( FilaEventos, (void*)NovoEvento( 0, OCUPACAO_FILAS, s3, NULL ) );

	/* Define a disciplina a utilizar. */
	disc = NovaDisciplina( atoi( argv[2] ) );

	/* Ciclo de simulacao. */
	while( ExecutaEvento( FilaEventos,
				disc -> ProcessaChegada ,
				disc -> ProcessaPartida ) );

	/* Apresentacao das estatisticas. */
	PrintRouter( NULL, r1 );
	PrintRouter( NULL, r2 );
	PrintRouter( NULL, r3 );

	PrintSessao( NULL, s1 );
	PrintSessao( NULL, s2 );
	PrintSessao( NULL, s3 );

	/* Limpeza do simulador. */
	ApagaFila( FilaEventos );

	ApagaRouter( r1 );
	ApagaRouter( r2 );
	ApagaRouter( r3 );

	ApagaSessao( s1 );
	ApagaSessao( s2 );
	ApagaSessao( s3 );

	ApagaDisciplina( disc );

	exit( 0 );
}

int ExecutaEvento( pq fila, void (*ProcessaChegada)(), void (*ProcessaPartida)() ) {
	Evento *ev;

	if( 0 == fila -> n_items  ) {
		/* Se a fila de eventos estiver vazia, a simulacao deve acabar. */
		return 0;
	}

	ev = RetiraDaFila( fila );

	switch( ev -> tipo ) {

		case CHEGADA:
			/* Chega um pacote ao router. */
			TEMPO_GLOBAL = ev -> tempo;
			(*ProcessaChegada)( ev, fila );
			break;

		case PARTIDA:
			/* O router fica disponivel para enviar. */
			TEMPO_GLOBAL = ev -> tempo;
			(*ProcessaPartida)( ev, fila );
			break;

		case RECARREGA:
			/* Temos de ler mais valores do ficheiro. META_EVENTO */
			RecarregaSessao( ev, fila );
			break;
		case OCUPACAO_FILAS:
			/* Temos de registar a ocupacao das filas. META_EVENTO*/
			RegistaOcupacaoFilas( ev, fila );
			break;
		case DEBITO:
			RegistaDebito( ev, fila );
			/* Temos de registar o debito medio. META_EVENTO! */
			break;
	}

	if( MAX_TIME > ev -> tempo ) {
		/* A simulacao deve continuar. */
		return 1;
	}
	else{
		/* A simulacao vai parar.  */
		return 1;
	}


}

/*
 * Funcao que le um dado numero de tempos de chegada de pacotes de uma 
 * sessao de um ficheiro e adiciona os eventos CHEGADA correspondentes 
 * na fila. O router deve ser sempre r1.
 */
void RecarregaSessao( Evento* ev, pq fila ) {
	int i;
	double tempo;

	for( i = 0 ; i < IN_BUFFER ; i++) {
		if( 1 != fscanf( ev -> s -> input, "%lf", &tempo ) ) {
			ApagaEvento( ev );	    
			return;
		}
		ColocaNaFila( fila, NovoEvento( tempo, CHEGADA, ev -> s, ev -> r ));
	}
	ev -> tempo = tempo;
	/* Reinsere o evento na fila. */
	ColocaNaFila( fila, ev );

	return;
}

/*
 * Para uma dada sessao, regista a ocupacao da dila de espera
 * correspondente. Especialmente interessante para o router que impoe restricoes
 * 'a rede, r2.
 */
void RegistaOcupacaoFilas( Evento* ev, pq fila ) {
	Estatistica* stat;
	int i;
	FILE* output;

	stat = ev -> s -> estatisticas;
	output = ev -> s -> filas;

	/* Actualiza os integrais totais. */
	for( i = 0 ; i < N_ROUTERS ; i++ ) {
		stat -> IntegralTotal[ i ] += stat -> Integral[ i ];
	}

	/* Imprime resultado. */
	for( i = 0 ; i < N_ROUTERS ; i++ ) {
		fprintf( output, "%f %f (router: %d)\n", 
				ev -> tempo, 
				stat -> Integral[ i ] / 0.5,
				i + 1 );
	}

	/* Reinicia o contador PacotesEntregues1seg. */
	for( i = 0 ; i < N_ROUTERS ; i++ ) {
		stat -> Integral[ i ] = 0.0;
	}

	/* Reagenda o evento para daqui a um segundo. */
	ev -> tempo += 0.5;

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
 * Regista o debito ???
 */
void RegistaDebito( Evento* ev, pq fila ) {

	/* Actualiza a contagem total de pacotes. */
	ev -> s -> estatisticas -> PacotesEntregues += 
		ev -> s -> estatisticas -> PacotesEntregues1seg;

	/* Imprime resultado. */
	fprintf( ev -> s -> debito, "%f %d\n", 
			ev -> tempo, 
			ev -> s -> estatisticas -> PacotesEntregues1seg * 
			ev -> s -> TamanhoPacote );

	/* Reinicia o contador PacotesEntregues1seg. */
	ev -> s -> estatisticas -> PacotesEntregues1seg = 0;

	/* Reagenda o evento para daqui a um segundo. */
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
 * Cria, inicia e devolve uma estrutura de estatistica.
 */
Estatistica* NovaEstatistica( void ){
	Estatistica* stat;
	int i;

	stat = (Estatistica*)malloc( sizeof( Estatistica ) );
	if( NULL == stat ){
		fprintf(stderr, "ERRO: malloc() = NULL in NovaEstatistica\n");
		exit( -1 );
	}

	/* estatisticas */
	stat -> PacotesEntregues = 0;
	stat -> PacotesEntregues1seg = 0;
	for( i = 0 ; i < N_ROUTERS ; i++ ) {
		stat -> IntegralTotal[ i ] = 0.0;
		stat -> Integral[ i ] = 0.0;
		stat -> Ocupacao[ i ] = 0;
		stat -> UltimaAlteracao[ i ] = 0.0;
	}
	stat -> AtrasoMaximo = 0.0;
	stat -> AtrasoSomado = 0.0;

	stat -> DebitoSessoes1seg = 0.0 ; 
	stat -> OcupacaoInstantFilaSes = 0.0 ;
	stat -> DebitoMedioSes = 0.0;
	stat -> AtrasoMedioPktSes = 0.0;
	stat -> NumMedioPktSes = 0.0;
	stat -> AtrasoMaximoPktSes = 0.0;
	stat -> OcupacaoMediaFilaSes = 0.0;
	stat -> PercentPktDescartSes = 0.0;	
	stat -> UtilisMediaComut = 0.0;

	return stat;
}

/*
 * Actualiza os dados estatisticos das filas de espera FIFO.
 */
void ActualizaOcupacaoFila( float tempo, Sessao* s, Router* r, int delta ) {
	Estatistica* stat;

	stat = s -> estatisticas;

	/* Acumula a area de mais um troco do grafico da ocupacao da fila. */
	stat -> Integral[ r -> id ] +=
		( tempo - stat -> UltimaAlteracao[ r -> id ] ) * 
		( stat -> Ocupacao[ r -> id ] );
	/* Actualiza a ocupacao. */
	stat -> Ocupacao[ r -> id ] += delta;
	/* Actualiza o tempo da UltimaAlteracao. */
	stat -> UltimaAlteracao[ r -> id ] = tempo;

	/*
	   fprintf( stdout, "Debug: dt: %1.10f  router: %d sessao: %d ocupacao: %d integral %f\n", 
	   (ev -> tempo) - (stat -> UltimaAlteracao[ router_id ]), 
	   router_id + 1,
	   ev -> s -> id + 1,
	   stat -> Ocupacao[ router_id ],
	   stat -> Integral[ router_id ] );
	 */
	return;
}
/*
 * Actualiza os dados estatisticos das filas de uma sessao num dado router.
 */
void ActualizaOcupacaoFilas( float tempo, Sessao* s, Router* r, int delta ) {
	Estatistica* stat;

	stat = s -> estatisticas;

	/* Acumula a area de mais um troco do grafico da ocupacao da fila. */
	stat -> Integral[ r -> id ] +=
		( tempo - stat -> UltimaAlteracao[ r -> id ] ) * 
		( stat -> Ocupacao[ r -> id ] );
	/* Actualiza a ocupacao. */
	stat -> Ocupacao[ r -> id ] += delta;
	/* Actualiza o tempo da UltimaAlteracao. */
	stat -> UltimaAlteracao[ r -> id ] = tempo;

	return;
}

/*
 * Actualiza os dados estatisticos sobre o atraso dos pacotes de uma sessao.
 */
void ActualizaAtrasoPacotes( Sessao* s, float atraso ) {

	/* Actualiza o atraso maximo. */
	if( atraso > s -> estatisticas -> AtrasoMaximo ) {
		s -> estatisticas -> AtrasoMaximo = atraso;
	}

	/* Contabiliza o atraso para efeitos de media. */
	s -> estatisticas -> AtrasoSomado += atraso;

	return;
}


/*
 * Imprime uma estrutura Estatistica para o ficheiro output.
 */
void PrintEstatistica( FILE* output, Estatistica* e ) {
	FILE* fp;

	if( NULL != output ) fp = output;
	else fp = stdout;

	fprintf( fp, "PacotesEntregues: %d \n", e -> PacotesEntregues );
	fprintf( fp, "PacotesEntregues1seg: %d \n", e -> PacotesEntregues1seg );
	fprintf( fp, "AtrasoMaximo: %f s\n", e -> AtrasoMaximo );
	fprintf( fp, "AtrasoMedio: %f s\n", e -> AtrasoSomado / e -> PacotesEntregues );

}

/*
 * Liberta os recursos consumidos pela estrutura.
 */
	void ApagaEstatistica( Estatistica* e ) {
		if( NULL != e )
			free( e );
	}

/*
 * Cria, inicia e devolve uma estrutura de router.
 */
Router* NovoRouter( int id, int capacidade, double atraso, Router* next ) {
	Router* rt;
	int i;

	rt = (Router*)malloc( sizeof( Router ) );
	if( NULL == rt ){
		fprintf(stderr, "ERRO: malloc() = NULL in NovoRouter\n");
		exit( -1 );
	}
	rt -> id = id;
	for( i = 0 ; i < N_SESSOES ; i++ ) {
		rt -> filas[ i ] = IniciaBuffer();
		rt -> FinishNumber[i] = 0.0;
	}
	rt -> RoundNumberT_Zero = 0.0;
	rt -> T_Zero = 0.0;
	rt -> SomatorioPesos = 0;
	rt -> capacidade = capacidade;
	rt -> atraso = atraso;
	rt -> livre = 1;
	rt -> next = (void*)next;
	rt -> actividade = 0.0;
	rt -> ronda = (int*)malloc( sizeof( int ) * ROUND_SIZE );
	/* A ronda e' definida 'a mao! */
	for( i = 0 ; i < ROUND_S1 ; i++ ) {
		rt -> ronda[i] = 0;
	}
	for( i = ROUND_S1 ; i < ROUND_S1 + ROUND_S2 ; i++ ) {
		rt -> ronda[i] = 1;
	}
	for( i = ROUND_S1 + ROUND_S2 ; i < ROUND_SIZE ; i++ ) {
		rt -> ronda[i] = 2;
	}
	rt -> servico = 0;
	/* DRR*/
	rt -> quantum = QUANTUM;

	for (i = 0; i < N_SESSOES; i++){
		rt -> DeficitCounter [i]= 0;
	}

	return rt;	
}

/*
 * Descreve o Router para o output.
 */
void PrintRouter( FILE* output, Router* r ) {
	FILE* fp;
	/*  int i ; */

	if( NULL != output ) fp = output;
	else fp = stdout;

	fprintf( fp, "Router: %d activo: %f %% do tempo(%f s).\n",
			r -> id +1 , 
			r -> actividade / TEMPO_GLOBAL * 100.0,
			r -> actividade );

	/*
	   for( i = 0 ; i < N_SESSOES ; i++ ) {
	   fprintf( fp, "DeficitCounter[ Sessao: %d ] = %d\n",
	   i, r -> DeficitCounter[ i ] );
	   }
	 */

}

/*
 * Liberta os recursos alocados para a estrutura.
 */
void ApagaRouter( Router* r ) {
	int i; 

	/*	fprintf( stdout, "ApagaRouter(router:%d)\n", r -> id ); */
	if( NULL != r ) {
		for( i = 0 ; i < N_SESSOES ; i++ ) {
			ApagaBuffer( r -> filas[ i ] );
		}
		free( r -> ronda );
		free( r );
	}
}

/*
 * Cria, inicia e devolve uma estrutura de sessao.
 */
Sessao* NovaSessao( int id, int prioridade, char* ficheiro_input, 
		int TamanhoPacote, char* ficheiro_debito,
		char* ficheiro_filas ){
	Sessao* sess;

	sess = (Sessao*)malloc( sizeof( Sessao ) );
	if( NULL == sess ){
		fprintf(stderr, "ERRO: malloc() = NULL in NovaSessao\n");
		exit( -1 );
	}

	sess -> id = id;
	sess -> prioridade = prioridade;
	sess -> input = fopen( ficheiro_input, "r" );
	if( NULL == sess -> input ){
		fprintf(stderr, "ERRO: fopen() = NULL in NovaSessao\n");
		exit( -1 );
	}

	sess -> estatisticas = NovaEstatistica();
	sess -> TamanhoPacote = TamanhoPacote;

	sess -> debito = fopen( ficheiro_debito, "w" );
	if( NULL == sess -> debito ){
		fprintf(stderr, "ERRO: fopen() = NULL in NovaSessao\n");
		exit( -1 );
	}
	sess -> filas = fopen( ficheiro_filas, "w" );
	if( NULL == sess -> filas ){
		fprintf(stderr, "ERRO: fopen() = NULL in NovaSessao\n");
		exit( -1 );
	}

	return sess;
}

/*
 * Descreve a Sessao para o output.
 */
void PrintSessao( FILE* output, Sessao* s ) {	
	FILE* fp;
	int i;

	if( NULL != output ) fp = output;
	else fp = stdout;

	fprintf( fp, "\nSessao: %d \n", s -> id + 1 );
	PrintEstatistica( output, s -> estatisticas );
	fprintf( fp, "Debito Medio: %f bit/s\n",
			s -> estatisticas -> PacotesEntregues *
			s -> TamanhoPacote /
			TEMPO_GLOBAL );

	for( i = 0 ; i < N_ROUTERS ; i++ ) {
		fprintf( fp, "Ocupacao Media Fila no Router %d: %f pacotes\n",
				i + 1,
				s -> estatisticas -> IntegralTotal[ i ] /
				TEMPO_GLOBAL );
	}
}

/*
 * Lberta os recursos alocados para a sessao.
 */
void ApagaSessao( Sessao* s ) {
	if( NULL != s ) {
		fclose( s -> input );
		ApagaEstatistica( s -> estatisticas );
		fclose( s -> debito );
		fclose( s -> filas );
		free( s );
	}
}


/*
 * Cria, inicia e devolve uma estrutura de evento.
 */
Evento* NovoEvento( double tempo, char tipo, Sessao* s, Router* r ) {
	Evento* ev;

	ev = (Evento*)malloc( sizeof( Evento ) );
	if( NULL == ev ){  
		fprintf(stderr, "ERRO: malloc() = NULL in NovoEvento\n");
		exit( -1 ); 
	}
	ev -> tempo = tempo;
	ev -> tipo = tipo;
	ev -> s = s;
	ev -> r = r;
	ev -> t_zero = tempo;
	ev -> FinishNumber = 0.0;

	return ev;
}

void PrintEvento( FILE* output, Evento* ev ) {
	FILE* fp;

	if( NULL != output ) fp = output;
	else fp = stdout;

	/* fprintf( fp, "ev->tempo = %f\n", ev -> tempo ); */
	fprintf( fp, "%f (t_zero: %f tipo:%d  sessao:%d  router:%d)\n",
			ev -> tempo, ev -> t_zero, ev -> tipo,
			ev -> s -> id + 1, ev -> r -> id + 1 );

}

	void ApagaEvento( Evento* ev ) {
		if( NULL != ev )
			free( ev );
	}

/*
 * Cria, inicia e devolve uma estrutura de disciplina de servico.
 */
Disciplina* NovaDisciplina( int code ) {
	Disciplina* disc;

	disc = (Disciplina*)malloc( sizeof( Disciplina ) );
	if( NULL == disc ){  
		fprintf(stderr, "ERRO: malloc() = NULL in NovaDisciplina\n");
		exit( -1 ); 
	}

	switch( code ) {
		case FIFO: 
			disc -> ProcessaChegada = &FIFOPC;
			disc -> ProcessaPartida = &FIFOPP;
			break;
		case WRR: 
			disc -> ProcessaChegada = &WRRPC;
			disc -> ProcessaPartida = &WRRPP;
			break;
		case DRR: 
			disc -> ProcessaChegada = &DRRPC;
			disc -> ProcessaPartida = &DRRPP;
			break;
		case WFQ: 
			disc -> ProcessaChegada = &WFQPC;
			disc -> ProcessaPartida = &WFQPP;
			break;
	}

	return disc;
}

	void ApagaDisciplina( Disciplina* disc ) {
		if( NULL != disc )
			free( disc );
	}


/*
 *	EOF
 */
