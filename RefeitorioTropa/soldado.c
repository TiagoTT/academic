/* 
 * Processo Soldado
 * Ficheiro: soldado.c
 */

#include <stdio.h>
#include <stdlib.h>

#include "sincronizacao.h"
#include "sock_stream.h"
#include "memoria.h"
#include "menu_sol.h"

#include "soldado.h"

#define MAX_BUF 80

int main(int argc, char* argv[])
{
	int pid;
	char* msg;
	SocketStream* mySocket;
	Menus menus;
	int escolha;
	char buf[MAX_BUF];

	pid = getpid();
	
	/* Inicia o socket stream. */
	mySocket = CriaSocketStreamLiga( SOCKET_STREAM_SERV );
	
	/* Anuncia que quer entrar. */
	if( snprintf(buf, MAX_BUF, "Apresenta-se o Soldado %d", pid) >= MAX_BUF ){
	  fprintf(stderr, "erro na passagem para o buf a apresetacao \n");
	}
	if( EnviaSocketStream(mySocket, buf ) < 0 ){
		fprintf(stderr, "Soldado (%d): O socket foi fechado!", pid );
		exit(-1);
	}	
	
	/* Espera para entrar. */
	EsperaSemaforo( ENTRADA );
	puts("Ficarei na porta 5 segundos.");
	sleep(5);	
	AssinalaSemaforo( PRESENTES );
	AssinalaSemaforo( SOLDADOS );
	AssinalaSemaforo( ENTRADA );
	
	puts("Entrei e estou em sentido.");
	
	puts("Vou consultar os menus.");
	/* Consulta o menu do turno actual. */
	menus = AcedeMemoriaMenus();
	/* Recebe do utilizador */
	escolha = MenuSoldado( menus );
	
	/* Espera em sentido pela entrada do general. */
	EsperaSemaforoZero( SENTIDO );
	
	/* Escreve o pedido e o pid na memoria partilhada. */
	EsperaSemaforo( PEDIDOS );
	puts("Vou escrever o pedido em memoria.");
	EscreveMemoriaPedido( pid, escolha );
	AssinalaSemaforo( PEDIDOS );
	
	/* Indica que ja' pediu. */
	EsperaSemaforo( CONTAGEM );
	
	/* Espera pelo jantar. */
	msg = RecebeSocketStream(mySocket);
	puts( msg );
	free( msg );
	sleep( 5 ); /* espera um pouco */
	puts( "Terminei a refeicao" );
	
	/* Comunica ao Refeitorio que terminou e quer sair. */
	if( EnviaSocketStream(mySocket, "Ja comi e quero sair.\0") < 0 ){
		fprintf(stderr, "Soldado (%d): O socket foi fechado!", pid );
		exit(-1);
	}	
	
	/* Espera para sair. */
	EsperaSemaforo( SAIDA );
	puts("Ficarei na porta 5 segundos.");
	sleep(5);
	EsperaSemaforo( PRESENTES );
	EsperaSemaforo( SOLDADOS );
	AssinalaSemaforo( SAIDA );

	puts("Ja sai do refeitorio");

	exit( 0 );
}

int MenuSoldado(Menus menus)
{
  char opcao;
  int escolha;
  
  while(1)
    {
      Escreve_Menu_Sol();
      opcao = getchar();
      
      switch( opcao )
	{
	case 'm':
	  /* Apresenta os menus no ecran */
	  MostraMenus( menus );
	  break;
	case 's':
	  /* Le a escolha do Soldado do teclado */
	  scanf( "%d", &escolha );
	  return escolha;
	  break;
	case 'q':
	  /* Desiste da refeicao e sai assim que possivel */
	  return -1;
	  break;
	default:
	  break;
	}
      
    }

  return -1;
}

void MostraMenus(Menus menus)
{
  int i;
  
  printf("Menus em %d linhas e %d colunas\n",
	 menus.linhas, menus.colunas);
  
  for( i = 0 ; i < menus.linhas ; i++ )
    {
      printf("%d - %s\n", i, &menus.texto[i*menus.colunas] );
    }
  
  return;
}
