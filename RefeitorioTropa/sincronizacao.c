/*
 * Ficheiro: sincronizacao.c
 *
 */

#include"sincronizacao.h"

void CriaSemaforo(int key)
{
	int id;

	struct{
		int val;
	}arg;

	if( ( id = semget( key, 1, 0666|IPC_CREAT ) ) < 0 ) {
		fprintf( stderr, "Erro em CriaSemaforo: semget().\n" );
		perror("Erro em CriaSemaforo: semget().");
		exit( -1 );
	}

	arg.val = 0;
	if( semctl( id, 0, SETVAL,arg ) < 0 ) {
		fprintf( stderr, "Erro em CriaSemaforo: semctl().\n" );
		perror("Erro em CriaSemaforo: semctl().");
		exit( -1 );
	}

	return;
}

void DefineSemaforo(int key, int valor)
{
	int id;

	struct{
		int val;
	}arg;

	if( ( id = semget( key, 1, 0666|IPC_CREAT ) ) < 0 ) {
		fprintf( stderr, "Erro em DefineSemaforo: semget().\n" );
		perror("Erro em DefineSemaforo: semget().");
		exit( -1 );
	}

	arg.val = valor;
	if( semctl( id, 0, SETVAL,arg ) < 0 ) {
		fprintf( stderr, "Erro em DefineSemaforo: semctl().\n" );
		perror("Erro em DefineSemaforo: semctl().");
		exit( -1 );
	}

	return;
}

int DevolveSemaforo(int key)
{
	int id;
	int val;
	
	if( ( id = semget( key, 1, 0) ) < 0 ) {
		fprintf( stderr, "Erro em Devolve Semaforo: semget().\n" );
		exit( -1 );
	}
	
	if( ( val = semctl( id, 0, GETVAL ) ) < 0 ) {
		fprintf( stderr, "Erro em DevolveSemaforo: semctl().\n" );
		perror("Erro em DevolveSemaforo: semctl().");
		exit( -1 );
	}

	return val;
}

void AssinalaSemaforo(int key)
{
	struct sembuf op;
	int id;

	if( ( id = semget( key, 1, 0) ) < 0 ) {
		fprintf( stderr, "Erro em AssinalaSemaforo: semget().\n" );
		exit( -1 );
	}

	op.sem_num = 0;
	op.sem_op = +1;
	op.sem_flg = 0;

	if( semop( id, &op, 1 ) < 0 ) {
		fprintf( stderr, "Erro em AssinalaSemaforo: semop().\n" );
		exit( -1 );
	}

	return;
}

void EsperaSemaforo(int key)
{
	struct sembuf op;
	int id;

	if( ( id = semget( key, 1, 0 ) ) < 0 ) {
		fprintf( stderr, "Erro em EsperaSemaforo: semget().\n" );
		exit( -1 );
	}

	op.sem_num = 0;
	op.sem_op = -1;
	op.sem_flg = 0;

	if( semop( id, &op, 1 ) < 0 ) {
		fprintf( stderr, "Erro em EsperaSemaforo: semop().\n" );
		exit( -1 );
	}

	return;
}

void EsperaSemaforoZero(int key)
{
	struct sembuf op;
	int id;

	if( ( id = semget( key, 1, 0 ) ) < 0 ) {
		fprintf( stderr, "Erro em EsperaSemaforoZero: semget().\n" );
		exit( -1 );
	}

	op.sem_num = 0;
	op.sem_op = 0;
	op.sem_flg = 0;

	if( semop( id, &op, 1 ) < 0 ) {
		fprintf( stderr, "Erro em EsperaSemaforoZero: semop().\n" );
		exit( -1 );
	}

	return;
}

void DestroiSemaforo(int key)
{
	int id;

	if( ( id = semget( key, 1, 0 ) ) < 0 ) {
		fprintf( stderr, "Erro em DestroiSemaforo: semget().\n" );
		exit( -1 );
	}

	if( semctl( id, 0, IPC_RMID) < 0 ) {
		fprintf( stderr, "Erro em DestroiSemaforo: semctl().\n" );
		exit( -1 );
	}

	return;
}
