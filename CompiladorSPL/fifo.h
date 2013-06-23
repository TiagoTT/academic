/*
 * fifo.h
 *
 * Estruturas, definicoes e prototipos do buffer FIFO.
 * 
 */

#ifndef _FIFO_H_
#define _FIFO_H_

/* Estrutura de dados. */
typedef struct BufferFIFOStr{
	int* table;
	unsigned long int head;	/* posicao do elemento mais antigo */
	unsigned long int free;	/* primeira posicao livre */
	unsigned long int size;	/* dimensao do canal */
	pthread_mutex_t mutex;	/* proteccao de leitura/escrita */
	sem_t sem_ch_read;	/* controlo de leitura do canal */
	sem_t sem_ch_write;	/* controlo de escrita no canal */
}BufferFIFO;

typedef BufferFIFO* bf;

/* Prototipos principais. */
bf IniciaBuffer( unsigned long int size );
int ColocaNoFim( bf buf, int val );
int RemoveDoInicio( bf buf );
int ConsultaInicio( bf buf );
void ApagaBuffer( bf buf );

/* Prototipos auxiliares. */
void PrintBuffer( FILE* output, bf buf );

#endif
