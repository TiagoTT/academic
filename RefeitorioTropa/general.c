/*
 * Processo General
 * Ficheiro: general.c
 */

#include <stdio.h>
#include <stdlib.h>

#include "sincronizacao.h"
#include "mailbox.h"
#include "menu_gen.h"
#include "cmd.h"
#include "signaln.h"

#include "general.h"

unsigned pid;

int main(int argc, char* argv[])
{
	unsigned key;
	char *msg;
	
	pid = getpid();
	INTOrd_signal( TerminaGeneral );
	/*
	 * o general tenta entrar no refeitorio 
	 * se estiver lá outro general ele nao entra espera um próximo turno
	 */
	CriaMailbox( pid );

	EnviaMailMessage( pid, KEY_REFEITORIO, TIPO_INICIO, "Ola eu sou o General. Quero entrar!\0");
	
	/* Espera para entrar e fechar a entrada. */
	EsperaSemaforo( ENTRADA );

	AssinalaSemaforo( PRESENTES );

	/* Guarda o numero de Soldados presentes. */
	DefineSemaforo( CONTAGEM, DevolveSemaforo( SOLDADOS ) );
	
	/* Deixa os Soldados "'a vontade" para estes realizarem os pedidos. */
	EsperaSemaforo( SENTIDO );
	
	/*
	 * A composicao da refeicao e' dada na linha de comandos 
	 * antecipadamente. Vai ler da linha de comandos a 
	 * composicao do jantar.
	 */
	msg = ProcessaArgumentos(argc, argv);

	EnviaMailMessage( pid, KEY_REFEITORIO, pid, msg);
	puts("Enviei o pedido:");
	puts( msg );
	free( msg );
	
	/*
	 * Espera ate todos os soldados ja terem pedido
	 */
	EsperaSemaforoZero( CONTAGEM );

	/*
	 * Da ordem para o jantar comecar a ser servido 
	 */
	AssinalaSemaforo( SERVIR_JANTAR );
	puts("Dei ordem ao Refeitorio para comecar a servir refeicoes.");
	
	/*
	 * Espera ate o Refeitorio lhe proporcionar a refeicao
	 */
	msg = RecebeMailMessage( &key, pid, TIPO_COMIDA );

	/*
	 * consumir o jantar
	 */
	puts( msg );
	free( msg );
	
	Menu_Gen();
	
	/* 
	 * Termina, levanta-se e da' permite que os Soldados saiam
	 */
	AssinalaSemaforo( SAIDA );
	
	/*
	 *verifica se todos os soldados ja sairam para abandonar a sala
	 */
	EsperaSemaforoZero( SOLDADOS ); 
	
	/*
	 * Sai do refeitório e comunica a saida
	 */
	EsperaSemaforo( SAIDA ); 
	EnviaMailMessage( pid, KEY_REFEITORIO, pid, "Estou a ir embora.\0");
	sleep(5); 
	AssinalaSemaforo( SAIDA );
	
	DestroiMailbox( pid );
	
	return 0;
}

void TerminaGeneral(int arg)
{
  DestroiMailbox( pid );
  puts("A terminar. Recebi um SIG...");
}
