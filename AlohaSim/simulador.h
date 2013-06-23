/*
 * simulador.h
 *
 * Estruturas de dados comuns e definicoes
 *
 */

#ifndef _SIMULADOR_H_
#define _SIMULADOR_H_

/* #define DEBUG 1 */

#define N_ESTACOES 10
#define PQ_SIZE   400

#define P_STEP	0.01

#define LAMBDA_MAX  0.050
#define LAMBDA_STEP 0.001

/* Tipos de evento */
#define CHEGADA		1  /* chegada de um pacote a um router*/
#define PARTIDA 	2  /* fim de envio de um pacote num router */
#define OCUPACAO_FILAS  3
#define DEBITO		4

/* Macro que compara a prioridade dos evnetos. 
   Esta condicao e' TRUE se (a) for mais prioritario que (b). */
   
#define FIRST(a,b)		( (a!=NULL) && (b!=NULL) && \
		( ((Evento*)a)->tempo < ((Evento*)b)->tempo ) )

/* #define FIRST(a,b) 	(a<b) */ /* teste */




/* definicoes de estruturas */


typedef struct EstatisticaStr{
  
  double Integral;
  double IntegralTotal;
  double UltimaAlteracao;
  int Ocupacao;


  /* Contabilizacao do atraso dos pacotes. */
  double AtrasoMaximo;
	
}Estatistica;


#define MAXLINE 80
typedef struct EstacaoStr{
  int id;              	/* identificador unico */
  bf fila;		/* fila de espera */
  int sucesso;    	/* acumulador dos slots de transmissao com sucesso */
  double atraso;	/* somatorio dos atrasos dos pacotes na fila de espera*/
  Estatistica* estatisticas;
  FILE* fila_output;
}Estacao;

typedef struct EventoStr{
  double tempo;
  char tipo;
  Estacao* es;
  double t_zero;
}Evento;

/* prototipos de funcoes */
void Simula( double p, double lambda, int verbose );
int ExecutaEvento( pq fila, Estacao** es, double lambda, double p );

Estatistica* NovaEstatistica( void );
void PrintEstatistica( FILE* output, Estatistica* e );
void ApagaEstatistica( Estatistica* e );

Evento* NovoEvento( double tempo, char tipo, Estacao* es );
void PrintEvento( FILE* output, Evento* ev );
void ApagaEvento( Evento* ev );

Estacao* NovaEstacao( int id, char* ficheiro_fila_output );
void PrintEstacao( FILE* output, Estacao* es );
void ApagaEstacao( Estacao* e );

void ActualizaAtrasoPacotes( Estacao* es, float atraso );
void RegistaOcupacaoFilas( Evento* ev, pq fila );
void ActualizaOcupacaoFila( float tempo, Estacao* es, int delta ); 


#endif
