/*
 * simulador.h
 *
 * Estruturas de dados comuns e definicoes
 *
 */

#ifndef _SIMULADOR_H_
#define _SIMULADOR_H_

/* #define DEBUG 1 */

#define N_SESSOES 3
#define N_ROUTERS 3
#define N_Exp 2

#define IN_BUFFER 100
#define PQ_SIZE   400

#define ROUND_SIZE 43
#define ROUND_S1   18
#define ROUND_S2   15
#define ROUND_S3   10

#define QUANTUM 10000

/* Tempo de paragem para as medicoes de estatisticas. */
/* #define MAX_TIME  90  PASSOU A VARIAVEL GLOBAL */

/* definicoes de constantes*/
/* int TabelaTamanhoPacote[ N_SESSOES ][ N_Exp ] = { 
   { 8000, 800},
   { 6400, 640},
   { 4800, 480 } };

   int PesoSessoes[ N_SESSOES ] = { 3, 2, 1};
   int CapacidadeRouter[ N_Router ] = { 2000000, 500000, 2000000 };
   double Tau = 0.010;
 */

/* Tipos de evento */
#define CHEGADA		1  /* chegada de um pacote a um router*/
#define PARTIDA 	2  /* fim de envio de um pacote num router */
#define RECARREGA	3  /* vai ler ao ficheiro os novos pacotes */
#define OCUPACAO_FILAS	4 /*  para recolher as estatisticas de 500 ms*/
#define DEBITO		5/*  para recolher o debite a cada 1 seg*/

/* Disciplinas de servico */
#define FIFO	1
#define WRR	2
#define DRR	3
#define WFQ	4

/* Macro que compara a prioridade dos evnetos. 
   Esta condicao e' TRUE se (a) for mais prioritario que (b). */

#define FIRST(a,b)		( (a!=NULL) && (b!=NULL) && \
		( ((Evento*)a)->tempo < ((Evento*)b)->tempo ) )

/* #define FIRST(a,b) 	(a<b) */ /* teste */

/* definicoes de estruturas */

typedef struct RouterStr{
	int id;              /* identificador unico */
	bf filas[N_SESSOES]; /* filas de espera */
	double FinishNumber[N_SESSOES]; /* o maior finish number da sessao*/
	double RoundNumberT_Zero;
	double T_Zero;
	int SomatorioPesos;
	int capacidade;      /* capacidade em bit/s */
	double atraso;        /* atraso da ligacao seguinte */
	int livre;           /* bit de estado */
	void* next;          /* apontador para o proximo router no caminho */
	double actividade;    /* acumulador dos tempos de actividade */
	int* ronda;          /* tabela que representa a ronda de servico (RR) */
	int servico;         /* posicao actual na ronda (RR) */
	int quantum;		/* quantum necessário para o algoritimo do DFRR*/
	int DeficitCounter[N_SESSOES];	/* algoritimo do DRR*/
}Router;

typedef struct EstatisticaStr{
	/* Numero total de pacotes entregues. */
	unsigned int PacotesEntregues;
	/* Numero de pacotes entregues desde o inicio deste segundo. */
	unsigned int PacotesEntregues1seg;

	/* Integrais da ocupacao das filas de espera. */
	double IntegralTotal[ N_ROUTERS ];
	double Integral[ N_ROUTERS ];
	int Ocupacao[ N_ROUTERS ];
	double UltimaAlteracao[ N_ROUTERS ];

	/* Contabilizacao do atraso dos pacotes. */
	double AtrasoMaximo;
	double AtrasoSomado;


	/* recolhidos durante a simulacao 'a saída do ultimo comutador*/
	double DebitoSessoes1seg ;

	/* ocupacao recolhida a cada 500 ms*/
	double OcupacaoInstantFilaSes ;

	/* estatisticas recolhidas a saida do ultimo comutador*/
	double DebitoMedioSes;
	double AtrasoMedioPktSes;
	double NumMedioPktSes;
	double AtrasoMaximoPktSes;
	double OcupacaoMediaFilaSes;
	double PercentPktDescartSes;	
	/* percentagem de tempo que cada comutador esta oucupado a servir pacotes */
	double UtilisMediaComut;

}Estatistica;

typedef struct SessaoStr{
	int id;
	int prioridade; /* peso*/
	FILE* input;
	/* Estatisticas */
	Estatistica* estatisticas;
	int TamanhoPacote;
	FILE* debito;
	FILE* filas;
}Sessao;

typedef struct DisciplinaStr{
	void (*ProcessaChegada)();
	void (*ProcessaPartida)();
}Disciplina;

typedef struct EventoStr{
	double tempo;
	char tipo;
	Sessao* s;
	Router* r;
	double t_zero;
	double FinishNumber;
}Evento;

/* prototipos de funcoes */
int ExecutaEvento( pq fila, void (*ProcessaChegada)(), void (*ProcessaPartida)() );

void RecarregaSessao( Evento* ev, pq fila );
void RegistaOcupacaoFilas( Evento* ev, pq fila );
void RegistaDebito( Evento* ev, pq fila );
void ActualizaAtrasoPacotes( Sessao* s, float atraso );

Estatistica* NovaEstatistica( void );
void PrintEstatistica( FILE* output, Estatistica* e );
void ApagaEstatistica( Estatistica* e );

void ActualizaOcupacaoFilas( float tempo, Sessao* s, Router* r, int delta );
void ActualizaOcupacaoFila( float tempo, Sessao* s, Router* r, int delta );

Router* NovoRouter( int id, int capacidade, double atraso, Router* next );
void PrintRouter( FILE* output, Router* r );
void ApagaRouter( Router* r );

Sessao* NovaSessao( int id, int prioridade, char* ficheiro_input, int TamanhoPacote, char* ficheiro_debito, char* ficheiro_filas );
void PrintSessao( FILE* output, Sessao* s );
void ApagaSessao( Sessao* s );

Evento* NovoEvento( double tempo, char tipo, Sessao* s, Router* r );
void PrintEvento( FILE* output, Evento* ev );
void ApagaEvento( Evento* ev );

Disciplina* NovaDisciplina( int code );
void ApagaDisciplina( Disciplina* disc );


#endif
