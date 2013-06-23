/*
 * Ficheiro: mailbox.c
 *
 * Contem funcoes para utilizar Mailboxes
 */

#include "mailbox.h"

void CriaMailbox(unsigned key)
{
	if (msgget( key, 0666|IPC_CREAT) < 0) {
		perror("Erro a criar queue mailbox.");
		exit(-1);
	}
	
	return;
}

void EnviaMailMessage(unsigned fromKey, unsigned toKey, unsigned tipo, char* msgBody)
{
	int /*fromId,*/ toId;
	MailMesage msg;

	/*if ((fromId=msgget(fromKey, 0)) < 0) {
		perror("Erro a associar a queue from");
		exit(-1);
	}*/

	if ((toId=msgget(toKey, 0)) < 0) {
		perror("Erro a associar a queue to.");
		exit(-1);
	}

	msg.mtype = tipo;
	msg.senderId = fromKey;
	strncpy( msg.msgBody, msgBody, MAX_BODY);
	
	if (msgsnd(toId, &msg, sizeof(msg.msgBody)+sizeof(msg.senderId), 0) < 0)
	{
		perror("Erro a enviar mensagem.");
		exit(-1);
	}
	
	return;
}

/* 
 * NOTA: nao esquecer de libertar o bloco de memoria devolvido 
 * contendo o corpo da mensagem, pois e' alocado dinamicamente
 */
char* RecebeMailMessage(unsigned* fromKey, unsigned toKey, unsigned tipo)
{
	int toId;
	char* body;
	MailMesage msg;
	
	if ((toId=msgget(toKey, 0)) < 0) {
		perror("Erro a associar a queue to.");
		exit(-1);
	}


	if (msgrcv(toId, &msg, sizeof(msg.msgBody)+sizeof(msg.senderId), tipo, 0) < 0) 
	{
		perror("Erro a receber mensagem.");
		exit(-1);
	}

	body = (char*)malloc( (strlen(msg.msgBody)+1)*sizeof(char) );
	strcpy(body, msg.msgBody);
	
	*fromKey = msg.senderId;

	return body;
}

void DestroiMailbox(unsigned key)
{
	int id;

	if ((id=msgget(key, 0)) < 0) {  
		perror("Erro a associar a queue.");
		exit(-1);
	}   

	if (msgctl(id, IPC_RMID, 0) < 0) {
		perror("Erro a eliminar queue.");
		exit(-1);
	}

	return;
}

