/*************************************************************** 
 * Processo  Oficial
 * Ficheiro: oficial.c
 * Autores:  Joana Dionisio, Tiago Teresa, Carla Duarte
 * Data:     Novembro 2005
 **************************************************************/
#include<stdio.h>
#include <stdlib.h>
#include "sock_dgram.h"
#include "sincronizacao.h"
#include "signaln.h"
#include "cmd.h"

#include "oficial.h"


int main(int argc, char* argv[])
{
  
  sock_dgram *socketDgramOfic;
  sock_dgram *socketDgramRef;
  char sdLink[MAX_LINK];
  int pid;
  char* refeicao;
  char buf[MAX_BUF];
  char msg[MAX_BUF_MSG];
  /* 
   * le da linha de comandos a composicao do jantar
   * [nome quantidade] 
   */  
  refeicao = ProcessaArgumentos( argc, argv);
  /*
   * apanha  o PID do processo
   */
  pid=getpid();

  /*
   * escreve para dentro do vector sdLink o a localizacao do novo socket
   * a ser criado ("/tmp/(pid do oficial)")
   */
  if( snprintf(sdLink, MAX_LINK, "%s%d", BASE_LINK, pid) >= MAX_LINK ){
    fprintf(stderr, "O nome do link para o socket dgram excedeu MAX_LINK \n");
  }
  socketDgramOfic= CriarSd( sdLink );
  socketDgramRef = CriaSdAuxiliar();
  father_fill(socketDgramRef, SOCKET_DGRAM_SERV);
  /* Envia mensagem de chegada ao refeitorio.
   * (Antes de entrar)
   */
  if( snprintf(buf, MAX_BUF, "Apresenta-se o Oficial %d", pid) >= MAX_BUF ){
    fprintf(stderr, "erro na passagem para o buf a apresetacao \n");
  }
  EnviaMsgSock(socketDgramRef, buf, socketDgramOfic);
  /*
   * espera no semaforo da porta para entrar
   */
  EsperaSemaforo( ENTRADA );
  puts("Estou a entrar no Refeitorio.");
  sleep(3);/*para teste*/
  AssinalaSemaforo(PRESENTES);
  AssinalaSemaforo( ENTRADA );

  /*++++comunicacao  com refeitorio durante o jantar SOCKETS DATAGRAMA+++++++*
   * 
   * trarnsmitir a ementa pretendida ao refeitorio
   */
 
  EnviaMsgSock(socketDgramRef, refeicao, socketDgramOfic);
 
  RecebMsgSock(socketDgramRef, socketDgramOfic, msg);
  /*
   * recebe signals SIGUSR1 ou SIGUSR2
   * comunica ao refeitÃ³rio da sua saida
   *
   */ 

  INTOrd_signal(Terminar_Militar);/* Instalar handler de terminação */
  EnviaMsgSock(socketDgramRef, "Saida do refeitorio", socketDgramOfic);
  
  EsperaSemaforo( PRESENTES );
    
  TerminaSock(socketDgramOfic);
  
  return 0;
}
