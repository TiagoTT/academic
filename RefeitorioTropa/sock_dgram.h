#ifndef _SOCK_DGRAM_H
#define _SOCK_DGRAM_H

/*
 * Includes necessarios
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h> 

/*
 * definicao de estrutura
 */

typedef struct sock_dgram_str{

  int sd;                            /* socket descriptor */
  struct sockaddr_un addr;           /* endereço */
  socklen_t addrlen;                 /* dimensao */
  
}sock_dgram;

/*
 * Definicao de parametros comums
 */
#define MAX_BUF_MSG 80
#define SOCKET_DGRAM_SERV "/tmp/sdserv"


/* cabecalho das funcoes */

void sun_fill(char *name, struct sockaddr_un *addr, socklen_t *addrlen);
void father_fill (sock_dgram  *soketDgramRef, char *name);
sock_dgram *CriarSd(char *name);
sock_dgram *RecebMsgSock(sock_dgram *sender, sock_dgram *receiver, char msg[MAX_BUF_MSG]);
void EnviaMsgSock(sock_dgram *receiver, char *msg,sock_dgram *sender );
void TerminaSock(sock_dgram *socketDgram);
sock_dgram *CriaSdAuxiliar();


#endif
