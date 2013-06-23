/**************************************************************
 * Processo  Refietorio
 * Ficheiro: ref_ofi.c
 * Autores:  Joana Dionisio, Tiago Teresa, Carla Duarte
 * Data:     Novembro 2005
 *************************************************************/
#include <pthread.h>
#include "sincronizacao.h"
#include "sock_dgram.h"
#include "ref_ofi.h"
#include "signaln.h"
#include "registo.h"

#define MSG "Refeitorio responde!!!\0"
Clientes clientes;
Stock stock;

void* ThreadOficiais(void* argumento)
{
  /* 
   * vai ao stock procura pelos items [nome quantidade]
   * recebidos do socket
   */
  /*
   * caso nao encontre chama a funcao adiciona do stock
   * fica a espera no semaforo do stock 
   */
  
 
  return 0;
}


void CancelaThreadOfic(void)
{  
  /* 
   * recebe a comunicacao de que  um oficial recebeu
   * um dos signals SIGUSR1 ou  SIGUSR2
   *------ desencadeia a saida-----
   */
  /*  kill(getpid(),SIGTERM); isso vai para o menu*/
  
  /* 
     INTOrd_signal(Terminar_Ref);
  */
  /* Instalar handler de terminação*/
  
  /* retira o pedido desse oficial do vector*/
  return; 
}

void *AtendeOficiais(void* argumento)
{
  pthread_t TidOficiais; 
  sock_dgram *socketDgramOfic;
  sock_dgram *socketDgramRef;
  char msg[MAX_BUF_MSG];
  /*
   * Inicializacao do socket (datagram)
   */
  socketDgramRef= CriarSd( SOCKET_DGRAM_SERV );
  socketDgramOfic= CriaSdAuxiliar();
  /*
   *  Ciclo de atendimento
   */
  while(1)
    {
      RecebMsgSock(socketDgramOfic,socketDgramRef, msg);
      
      if(strncmp( msg,"Apresenta", strlen("Apresenta") )== 0)
	{
	 
	  RegistaEntrada( IdentificaOficial(msg), OFICIAL);
	}
      else if (strncmp( msg, "Saida", strlen("Saida") )== 0)
	{
	  CancelaThreadOfic();   
	}
      else{
	/*
	 * Lanca threads oficiais
	 */ 
	pthread_create(&TidOficiais, NULL,
		       ThreadOficiais, (void*) socketDgramRef);    
	
	/*quando tiver no stock envia a comida por socket*/
	EnviaMsgSock(socketDgramOfic, MSG, socketDgramRef);
	 CancelaThreadOfic();
      } 
    }
  
 
  return 0;
}
