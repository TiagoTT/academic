#ifndef _SOCK_STREAM_H
#define _SOCK_STREAM_H

/*
 * Includes necessarios
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

/* 
 * Definicao de estruturas
 */
typedef struct SocketStreamStr
{
	int	fd;
	char*	nome;
	struct 	sockaddr_un my_addr;
	socklen_t addrlen;
	  
}SocketStream;

/*
 * Definicao de parametros comums
 */
#define SOCKET_STREAM_SERV "/tmp/ssserv"

/*
 * Prototipos das funcoes
 */
SocketStream* CriaSocketStreamEscuta(char* nome);
SocketStream* AceitaLigacaoSocketStream(SocketStream* aberto);
SocketStream* CriaSocketStreamLiga(char* nome);
int EnviaSocketStream(SocketStream* destino, char* msg);
char* RecebeSocketStream(SocketStream* destino);
void FechaSocketStream(SocketStream* sock);
#endif


