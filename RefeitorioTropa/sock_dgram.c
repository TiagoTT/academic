/*************************************************************************** 
 * Ficheiro:  sock_dgram.c
 * Descricao: contém as funcoes para uso generalizado 
 *            dos socket datagrma 
 */


#include "sock_dgram.h"



void sun_fill(char *name, struct sockaddr_un *addr, socklen_t *addrlen)
{

/* Funcao que faz a inicializacao das estruturas do socket datagrama */

  addr->sun_family = AF_UNIX;
  
  /* coloca na  memoria  um byte constante nesse caso "0" */
  memset(addr->sun_path, 0, sizeof(addr->sun_path));
  
  strcpy(addr->sun_path, name);
  *addrlen = sizeof(addr->sun_family) + strlen(addr->sun_path);


}

void father_fill (sock_dgram *soketDgramRef, char *name)
{
  
  soketDgramRef->addr.sun_family = AF_UNIX;
  memset(soketDgramRef->addr.sun_path, 0, sizeof(soketDgramRef->addr.sun_path));
  strcpy(soketDgramRef->addr.sun_path,name);
  soketDgramRef->addrlen = sizeof(soketDgramRef->addr.sun_family) + strlen(soketDgramRef->addr.sun_path);
  
  return;
}

sock_dgram *CriarSd(char *name)
{
  sock_dgram *novo;
  
  novo= (sock_dgram*) malloc(sizeof(sock_dgram));
	
  /*
   * socket()   -> Cria um porto para comunicação assíncrona, bidireccional
   * e retorna um descritor
   * PF_UNIX    -> comunicacao local
   * SOCK_DGRAM -> sem ligação, mensagens limitadas no comprimento,
   * comunicação sem garantia de sequencialidade, fiabilidade ou não
   * existência de duplicações.
   * assert () ->  aborta o programa se o assert for falso
   */
  
  assert((novo->sd = socket(PF_UNIX, SOCK_DGRAM, 0)) >= 0);
  
  unlink(name);			/* Se não foi eliminado anteriormente */
  
  
  /* inicializacao do socket datagram */
  sun_fill(name, &(novo->addr), &(novo->addrlen));
  
  /* bind->Associa um nome a um socket já criado */
  assert(bind(novo->sd, (struct sockaddr *)&(novo->addr),novo->addrlen) >= 0 );
  
  
  return(novo);
}

sock_dgram *CriaSdAuxiliar()
{
  sock_dgram *socketDgramaux;
 
 /* reserva de espaço para o socket auxiliar*/
  socketDgramaux= (sock_dgram*) malloc(sizeof (sock_dgram));
  return (socketDgramaux);

}


sock_dgram *RecebMsgSock(sock_dgram *sender, sock_dgram *receiver, char msg[MAX_BUF_MSG])
{
  /* struct sockaddr_un from;*/     /*from - endereço do socket que enviou a mensagem.*/
  socklen_t fromlen;         /*fromlen - ponteiro para inteiro inicializado a sizeof(*from).*/
  
/* 
   * -int recvfrom (int s, char *buf, int len, int flags, struct sockaddr *from,
   int * fromlen) ;
   
 
 
   * -recvfrom()->Recebe uma mensagem através do socket s de um socket remoto
   independentemente de existir ou não ligação estabelecida 
   entre os dois. Devolve o tamanho da mensagem lida.
  */
  
  char buf[ MAX_BUF_MSG ];
  
  fromlen= sizeof(sender->addr);
  
  if (recvfrom(receiver->sd, buf, sizeof(buf), 0, (struct sockaddr *)&(sender->addr), &fromlen) < 0) {
    perror(" Erro no recvfrom");
  }
  else {
    strncpy(msg, buf,sizeof(buf) );
    printf(" Recebi: %s\n", buf);   
  } 
  
  return (sender); 
}


void EnviaMsgSock( sock_dgram *receiver, char *msg, sock_dgram *sender )
{
  /* struct sockaddr_un to; */       /*to - endereço do socket para onde vai ser enviada a mensagem.*/
  socklen_t tolen;            /*tolen - inteiro igual a sizeof(*to)*/
  
	
	/* 
	 *-int sendto (int s, char *msg, int len, int flags, struct sockaddr *to,
   int tolen) ;
   *-sendto->Envia uma mensagem através do socket s 
   para o socket especificado em to
   independentemente de existir ou não ligação
   estabelecida entre os dois sockets
	*/
  tolen= sizeof(receiver->addr);
  
  if (sendto(sender->sd, msg, strlen(msg)+1, 0, (struct sockaddr *)&(receiver->addr), 
	     tolen) < 0) {
    perror(" Erro no sendto");
  }
  
}


void TerminaSock(sock_dgram *socketDgram)
{
  struct sockaddr_un my_addr;


	my_addr= socketDgram->addr;
  
	close(socketDgram->sd);                    /* closes  a  file descriptor */
  unlink(my_addr.sun_path);     /* unlink function to remove the specified file */

}




