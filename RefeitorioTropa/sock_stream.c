/*
 * Ficheiro: sock_stream.c
 *
 * Contem funcoes para utilizar Mailboxes, Sockets Stream,
 * Sockets Datagram e Shared Memory
 */

#include "sock_stream.h"

#define	MAX_BUF	20

SocketStream* CriaSocketStreamEscuta(char* nome) /* Servidor */
{
	SocketStream* novo;
	
	novo = (SocketStream*)malloc( sizeof(SocketStream) );
	
	novo->nome = (char*)malloc( sizeof(char)*(strlen(nome) + 1) );
	strcpy(novo->nome, nome); /* Nome do servidor no "file system" */
	
	if ((novo->fd = socket(PF_UNIX, SOCK_STREAM, 0)) < 0 ) {
		perror("Erro a criar socket");
		exit(-1);
	}

	novo->my_addr.sun_family = AF_UNIX;
	memset(novo->my_addr.sun_path, 0, sizeof(novo->my_addr.sun_path));
	strcpy(novo->my_addr.sun_path, nome);
	novo->addrlen = sizeof(novo->my_addr.sun_family) 
			+ strlen(novo->my_addr.sun_path);

	unlink(nome);
	
	if (bind(novo->fd, (struct sockaddr *)&(novo->my_addr), novo->addrlen) < 0 ) {
		perror("Erro no bind"); 
		exit(-1);
	}
	
	if (listen(novo->fd, 10) < 0 ) { /* Suporta 10 pedidos simultaneos. */
		perror("Erro no listen");
		exit(-1);
	}
	
	return novo;
}

SocketStream* AceitaLigacaoSocketStream(SocketStream* aberto) /* Servidor */
{
	SocketStream* novo;

	novo = (SocketStream*)malloc( sizeof(SocketStream) );

	while(1) {
		novo->addrlen = sizeof(novo->my_addr);
		if(-1==( novo->fd = accept(aberto->fd,
					(struct sockaddr*)&(novo->my_addr),
					(socklen_t*)&(novo->addrlen)) ))
		{
			if( EINTR == errno )
				continue;
			else{
				perror("Erro da funcao accept");
				exit( -1 );
			}
		}else
			break;
	}
	
	return novo;
}

SocketStream* CriaSocketStreamLiga(char* nome) /* Cliente */
{
	SocketStream* novo;
	
	novo = (SocketStream*)malloc( sizeof(SocketStream) );

	novo->nome = NULL; /* O cliente nao tem nome no "file system" */
	
	if ((novo->fd = socket(PF_UNIX, SOCK_STREAM, 0)) < 0 ) {
		perror("Erro a criar socket");
		exit(-1);
	}

	novo->my_addr.sun_family = AF_UNIX;
	memset(novo->my_addr.sun_path, 0, sizeof(novo->my_addr.sun_path));
	strcpy(novo->my_addr.sun_path, nome);
	novo->addrlen = sizeof(novo->my_addr.sun_family) 
			+ strlen(novo->my_addr.sun_path);

	if (connect(novo->fd, (struct sockaddr *)&(novo->my_addr), novo->addrlen) < 0 ) {
		perror("Erro no connect"); 
		exit(-1);
	}
	
	return novo;
}

int EnviaSocketStream(SocketStream* destino, char* msg)
{
	int size, writen;

	if(NULL == msg){
		return 0;
	}
	size = strlen(msg) + 1 /* '\0' */;

	while(size > 0){
		/* Apenas o ciclo while(1){...} basta se O_NONBLOCK nao for 
		 * especificado na criacao do socket.
		 */
		while(1){
			if ( (writen=write(destino->fd, msg, size)) < 0){
				if(EPIPE == errno){
					/* So se o sinal SIGPIPE for ignorado */
					perror("write() falhou: O socket foi fechado !?");
					return -1; /* Nao aborta o programa, mas 
						      devolve -1 para avisar que o 
						      socket foi fechado. */
				}else if(EINTR == errno){
					continue;
				}else{
					perror("Erro no write");
					exit(-1);
				}
			}else{
				break;
			}
		}
		size -= writen;
	}
	return 0;
}

char* RecebeSocketStream(SocketStream* destino)
{
	char buf[MAX_BUF];
	char* msg;
	int size;

	size = 0;
	msg = (char*)malloc(sizeof(char));
	msg[0]='\0';
	buf[0]='\0';
	
	do{
		while( (size = read(destino->fd, buf, MAX_BUF-1)) < 0 ){
			if(EINTR == errno){
				continue;
			}else{
				perror("Erro no read");
				exit(-1);
			}
		}
		
		if(size > 0){ /* read() leu alguns caracteres */
			if(buf[size-1] != '\0') /* fim de menssagem */
				buf[size]='\0';
		}else{ /* read() recebeu o EOF */
			break;
		}
		msg = (char*)realloc( msg, sizeof(char)*( strlen(msg) + size + 1) );
		strcat(msg, buf);
	}while(buf[size-1] != '\0');

	return msg;
}

void FechaSocketStream(SocketStream* sock){
	
	if(close(sock->fd) < 0){
		perror("Erro no close");
	}
	
	if(NULL != sock->nome){ /* So e' feito no caso do servidor. */
		unlink(sock->nome);
		free(sock->nome);
	}
	
	free(sock);
	
	return;
}
