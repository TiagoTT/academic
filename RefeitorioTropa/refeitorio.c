/*
 * Processo Refeitorio
 * Ficheiro: refeitorio.c
 */

#include "ref_sol.h"
#include "ref_ofi.h"
#include "ref_gen.h"
#include "sincronizacao.h"
#include "stock.h"
#include "registo.h"
#include "menu_ref.h"
#include "signaln.h"

#include "refeitorio.h"

#define MAX_ARG

/* Variaveis globais */
Clientes clientes;
Stock	 stock;

pthread_t ThreadAtendeSoldados;	
pthread_t ThreadAtendeOficiais;	
pthread_t ThreadAtendeGenerais;
pthread_t ThreadMenu;
	
int main(int argc, char* argv[])
{
        INTOrd_signal( TerminaRefeitorio );

	/*
	 * Abertura e verificacao do ficheiro de existencias 
	 * e respecti+vo mapeamento em memoria.
	 */
	MapeiaFicheiroStock( argv[1] );
	
	/*
	 * Inicializacao dos mecanismos de sincronizacao.
	 */
	CriaSemaforo( ENTRADA );
	CriaSemaforo( SAIDA );
	CriaSemaforo( PRESENTES );
	CriaSemaforo( SERVIR_JANTAR );
	CriaSemaforo( CONTAGEM );
	CriaSemaforo( SENTIDO );
	CriaSemaforo( PEDIDOS );
	CriaSemaforo( MUTEX_INDICE_ESCOLHA );
	CriaSemaforo( INDICE_ESCOLHA );
	CriaSemaforo( SEM_LINHAS );
     	CriaSemaforo( SEM_COLUNAS );
	CriaSemaforo( BARREIRA_LE );
	CriaSemaforo( BARREIRA_SERVE );
	CriaSemaforo( SOLDADOS );
	CriaSemaforo( PEDIDOS_PRONTOS );
	CriaSemaforo( CONTAGEM );
	/* Indica que os pedidos ainda nao estao completos */
	AssinalaSemaforo( PEDIDOS_PRONTOS );
	/* Impede as threads de ler os pedidos */
	AssinalaSemaforo( BARREIRA_LE );
	/* Impede as threads de servir. */
	AssinalaSemaforo( BARREIRA_SERVE );
	/* Os soldados esperam em sentido. */
	AssinalaSemaforo( SENTIDO );
	/* Abertura da porta de entrada. */
	AssinalaSemaforo( ENTRADA );


	/* Separacao do fluxo de execucao em 4 threads */
	pthread_create( &ThreadAtendeSoldados, NULL, AtendeSoldados, NULL);
	pthread_create( &ThreadAtendeOficiais, NULL, AtendeOficiais, NULL);
	pthread_create( &ThreadAtendeGenerais, NULL, AtendeGenerais, NULL);
	pthread_create( &ThreadMenu, NULL, Menu_Ref, NULL);
	
	/* Espera a ordem do General. */
	EsperaSemaforo( SERVIR_JANTAR );
	/* Indica que os pedidos ja estao prontos. */
	puts("Os pedidos ja estao prontos.");
	/* Inicia o contador decrescente PEDIDOS_PRONTOS */
	DefineSemaforo( PEDIDOS_PRONTOS, DevolveSemaforo( PRESENTES ) );
	
	/* Indica que as threads ja podem ler os pedidos */
	EsperaSemaforo( BARREIRA_LE );

	/* Espera que as threads terminem de ler os pedidos. */
	EsperaSemaforoZero( PEDIDOS_PRONTOS );
	puts("Os pedidos ja foram processados.");

	/* Representa o Stock em semaforos */
	CriaStock( stock.n_prod );
	puts("O stock ja foi representado em semaforos.");
	
	/* Baixa barreira para as threads comecarem a servir */
	EsperaSemaforo( BARREIRA_SERVE );
	puts("Os jantares vao comecar a ser servidos.");
	

	exit( 0 );
}

