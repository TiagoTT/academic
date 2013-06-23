#ifndef _MAILBOX_H
#define _MAILBOX_H

/*
 * Includes necessarios
 */
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <string.h>

/* 
 * Definicao de estruturas
 */

#define MAX_BODY 80

typedef struct MailMesageStr
{
	long	mtype;		/* Tipo de mensagem  */
  	unsigned senderId;	/* Id do remetente   */
	char	msgBody[MAX_BODY];	/* Corpo da mensagem */
}MailMesage;

/*
 * Definicao de parametros comums
 */
#define KEY_REFEITORIO	1

#define TIPO_QUALQUER	0
#define TIPO_INICIO	1
#define TIPO_COMIDA	2

/*
 * Prototipos das funcoes
 */
void CriaMailbox(unsigned key);
void EnviaMailMessage(unsigned fromKey, unsigned toKey, unsigned tipo, char* msgBody);
char* RecebeMailMessage(unsigned* fromKey, unsigned toKey, unsigned tipo); 
void DestroiMailbox(unsigned key);
#endif

