/*
 * queue.h
 *
 * Estruturas, definicoes e prototipos da fila prioritaria
 * e do buffer FIFO.
 * 
 */

/* Fila prioritaria */
/* Estrutura de dados. */
typedef struct FilaPrioritariaStr{
	void** table;
	unsigned int n_items;
	unsigned int size;
}FilaPrioritaria;

typedef FilaPrioritaria* pq;


/* Prototipos principais. */
pq IniciaFila( int size );
int ColocaNaFila( pq fila, void* item );
void* RetiraDaFila( pq fila );
void ApagaFila( pq fila );

/* Prototipos auxiliares. */ 
int FixUp( int n, pq fila );
int FixDown( int n, pq fila );
void PrintQueue( FILE* output, pq fila );

/* Macros auxiliares. */
#define TROCA(a,b)	aux = fila -> table[a];\
			      fila -> table[a] = fila -> table[b];\
fila -> table[b] = aux;

/* Buffer FIFO */
/* Estrutura de dados. */
#define BIT_MASK	0x1fff /* 0000 0011 1111 1111  = 1023 */
#define B_FIFO_SIZE	8192
typedef struct BufferFIFOStr{
	void* table[ B_FIFO_SIZE ];
	unsigned long int head;
	unsigned long int free;
}BufferFIFO;

typedef BufferFIFO* bf;

/* Prototipos principais. */
bf IniciaBuffer( void );
int ColocaNoFim( bf buf, void* item );
void* RemoveDoInicio( bf buf );
void* ConsultaInicio( bf buf );
void ApagaBuffer( bf buf );

/* Prototipos auxiliares. */
void PrintBuffer( FILE* output, bf buf );

