/*
 * Processo Refeitorio
 * Ficheiro: ref_sol.c
 */

#include "sock_stream.h"
#include "sincronizacao.h"
#include "stock.h"
#include "registo.h"
#include "memoria.h"
#include "menu_sol.h"
#include "ref_sol.h"


void *AtendeSoldados(void * parametros)
{
	pthread_t TidSoldados;

	SocketStream* SocketStreamRef;
	SocketStream* SocketStreamSol;

	/* Criacao dos menus e colocacao dos mesmos em 
	 * memoria partilhada */

	MenusFicheiroParaMemoriaPartilhada( "menus.txt", SHM_MENUS );
	
	/* Instala uma accao a realizar quando */
	/* pthread_cleanup_push(); */
	
	/* Inicializacao do semaforo que controla o acesso 'a zona de pedidos */
	DefineSemaforo( INDICE_ESCOLHA, 0 );
	DefineSemaforo( MUTEX_INDICE_ESCOLHA, 1 );

	/* Inicializacao do socket (stream) para ficar 'a escuta */
	SocketStreamRef = CriaSocketStreamEscuta( SOCKET_STREAM_SERV );

	/* Ciclo de atendimento */
	while(1){
		SocketStreamSol = AceitaLigacaoSocketStream( SocketStreamRef );
		/* Lanca thread ThreadSoldado() */
		pthread_create(&TidSoldados /* ?? */ ,NULL, ThreadSoldado, (void*)SocketStreamSol );
	}

	return 0;	
}

void *ThreadSoldado(void * argumento)
{

	SocketStream*	SocketStreamSol;
	char* msg;
	int pid;
	int menu;
       	Pedido* pedido;

	SocketStreamSol = (SocketStream*)argumento;

	msg = RecebeSocketStream( SocketStreamSol );

	pid = IdentificaSoldado( msg );
	puts( msg );
	free( msg );

	RegistaEntrada( pid, SOLDADO );

	/* Espera ordem de refeitorio para ir ler. */
	EsperaSemaforoZero( BARREIRA_LE );
	/* Le o pedido do Soldado da memoria partilhada */
	menu = RetiraPedido( pid );
        pedido = ProcessaMenuSoldado( menu );

	/* Indica que ja leu o pedido */
	EsperaSemaforo( PEDIDOS_PRONTOS );

	msg = CriaResposta( menu );
	
	/* Espera autorizacao para ir ao stock */
	EsperaSemaforoZero( BARREIRA_SERVE );
	/* Espera ingredientes */
	EsperaStock( pedido->prod, pedido->n_prod ); 

	if( EnviaSocketStream( SocketStreamSol, msg ) < 0)
	{
		fprintf( stderr, "Erro em EnviaSocketStream.\n" );
	}
       	free( msg );

	msg = RecebeSocketStream( SocketStreamSol );
	RegistaSaida( pid );
	puts( msg );
	free( msg );
	free( pedido );

	FechaSocketStream( SocketStreamSol );

	return 0;
}

void MenusFicheiroParaMemoriaPartilhada(char* ficheiro, int key)
{
  int fd;
  struct stat info;
  char* menus;
  char buf[100];
  int i, linhas, colunas=COLUNAS_MENUS;
  FILE* fp;
  
  assert( ( fd = open(ficheiro,  O_RDWR | O_CREAT, 0666 ) ) >= 0 );
  fstat( fd, &info );
  
  linhas = info.st_size / colunas;
  
  menus = (char*)CriaMemoriaPartilhada( SHM_MENUS, sizeof(char)*linhas*(colunas) );
  
  close( fd );
  
  fp = fopen( ficheiro, "r" );

  for( i = 0 ; NULL != fgets(buf, 98, fp) ; i++ )
    {
      strncpy( &menus[i*COLUNAS_MENUS], buf, colunas );
    }

  fclose( fp );
  
  DefineSemaforo( SEM_LINHAS, linhas );
  DefineSemaforo( SEM_COLUNAS, colunas );
  
  return;
}

int RetiraPedido(int pid)
{
  int i;
  Escolha* pedidos;
  int n_ped;
  
  pedidos =  AcedeMemoriaPedidos();
  n_ped = DevolveSemaforo( PRESENTES );
  
  for( i = 0 ; i < n_ped ; i++ )
    {
      if( pid == pedidos[i].pid )
	{
	  return pedidos[i].menu;
	}
    }
  
  return -1;
}

Pedido* ProcessaMenuSoldado( int menu )
{
  int i;
  char* buf;
  char nome[MAX_NOME];
  char quantidade[MAX_QUANT];
  Produto produtos[10];
  Pedido* pedido = NULL;
  Menus menus;
  
  buf = (char*)malloc(sizeof(char)*COLUNAS_MENUS);
  
  /* Traduz o menu de int para string */
  menus = AcedeMemoriaMenus();
 
  strncpy( buf,& menus.texto[ menu*COLUNAS_MENUS ], COLUNAS_MENUS );
  
  /* Passa de string para Pedido */
  for( i = 0 ;  2 == sscanf( buf, "%s %s", nome, quantidade ) ; i++ )
    {
      strcpy( produtos[i].nome, nome );
      strcpy(produtos[i].quantidade, quantidade );
      /* Avanca no buf */
      buf = &buf[ strlen(nome) + strlen(quantidade) +1 ];
    }
  
  pedido = (Pedido*)malloc(sizeof(Pedido));
  pedido->prod = (Produto*)malloc( i * sizeof(Produto) );
  pedido->n_prod = i;
  
  for( i = 0 ; i < pedido->n_prod ; i++ )
    {
      strcpy( pedido->prod[i].nome, nome );
      strcpy( pedido->prod[i].quantidade, quantidade );
    }
  
  return pedido;
}

char* CriaResposta(int menu)
{
  char *comida;
  Menus menus;

  /* Traduz o menu de int para string */
  menus = AcedeMemoriaMenus();

  comida = (char*) malloc ( sizeof(char)*COLUNAS_MENUS );

  strncpy( comida, &menus.texto[ menu*COLUNAS_MENUS ],COLUNAS_MENUS );

  return comida;
}
