/*
 * fifo.c
 *
 * Implementacao de um buffer fifo de dimensao fixa limitada.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#include "fifo.h"
/*
 * Inicializa o BufferFIFO.
 */
bf IniciaBuffer( unsigned long int size ) {
	bf buf;

	buf = (bf) malloc( sizeof(BufferFIFO) );
	if( NULL == buf ) {
		fprintf(stderr, "ERRO: malloc() = NULL in IniciaBuffer\n");
		exit( -1 );
	}
	buf -> table = (int*) malloc( sizeof( int ) * size );
	if( NULL == buf -> table ) {
		fprintf(stderr, "ERRO: malloc() = NULL in IniciaBuffer\n");
		exit( -1 );
	}
	memset( buf -> table, 0, sizeof( int ) * size );

	buf -> head = 0;
	buf -> free = 0;
	buf -> size = size;
	
	pthread_mutex_init( & buf -> mutex, NULL );
	sem_init( & (buf -> sem_ch_read), 0, 0 );
	sem_init( & (buf -> sem_ch_write), 0, size );
	
	return buf;
}

/*
 * Insere elemento no BufferFIFO.
 * Devolve o numero de posicoes livres no buffer.
 */
int ColocaNoFim( bf buf, int val ) {
	int cap;

	sem_wait( & (buf -> sem_ch_write) );
	pthread_mutex_lock( & (buf -> mutex) );

	buf -> table[ buf -> free ] = val;
	buf -> free = (buf -> free + 1) % (buf -> size) ;

	cap = (buf->free > buf->head)?
		( buf->size - (buf->free - buf->head) ) :
			( buf->head - buf->free );

	pthread_mutex_unlock( & (buf -> mutex) );
	sem_post( & (buf -> sem_ch_read) );
	
	return cap;
}

/*
 * Retira elemento do BufferFIFO.
 * Devolve o elemento.
 */
int RemoveDoInicio( bf buf ) {
	int val;
	
	sem_wait( & (buf -> sem_ch_read) );
	pthread_mutex_lock( & (buf -> mutex) );
	
	val = buf -> table[ buf -> head ];
	buf -> head = (buf -> head + 1) % (buf -> size) ;

	pthread_mutex_unlock( & (buf -> mutex) );
	sem_post( & (buf -> sem_ch_write) );
		
	return val;
}

/*
 * Consulta o proximo a sair do BufferFIFO.
 * Devolve o elemento.
 */
int ConsultaInicio( bf buf ) {
	return (buf -> table[ buf -> head ] );
}

void ApagaBuffer( bf buf ) {
	free( buf -> table );
	pthread_mutex_destroy( & (buf -> mutex) );
	sem_destroy( & (buf -> sem_ch_read) );
	sem_destroy( & (buf -> sem_ch_write) );
	free( buf );
}

/*
 * Imprime informacao sobre o buffer.
 */
void PrintBuffer( FILE* output, bf buf ) {
	int i;
	FILE* fp;

	if( NULL != output ) fp = output;
	else fp = stdout;

	fprintf( fp, "\nPrintBuffer():INICIO\n" );
	fprintf( fp, "\nbuf->head = %ld\n", buf->head );
	fprintf( fp, "buf->free = %ld\n", buf->free );
	fprintf( fp, "buf->size = %ld\n", buf->size );

	for( i = 0 ; i < buf -> size ; i++ ) {
		fprintf( fp, "buf->table[%d] = %d\n", i,buf->table[i] );
	}

	fprintf( fp, "\nPrintBuffer():FIM\n" );
}
/*
int main() {
	bf buf = IniciaBuffer( 5 );

	PrintBuffer( NULL, buf );
	ColocaNoFim( buf, 81 );
	PrintBuffer( NULL, buf );
	ColocaNoFim( buf, 82 );
	PrintBuffer( NULL, buf );
	ColocaNoFim( buf, 83 );
	PrintBuffer( NULL, buf );
	
	printf( "Retiramos: %d\n", RemoveDoInicio( buf ) );
	PrintBuffer( NULL, buf );
	
	ColocaNoFim( buf, 84 );
	PrintBuffer( NULL, buf );
	ColocaNoFim( buf, 85 );
	PrintBuffer( NULL, buf );
	ColocaNoFim( buf, 86 );
	PrintBuffer( NULL, buf );
	
	printf( "Retiramos: %d\n", RemoveDoInicio( buf ) );
	PrintBuffer( NULL, buf );
	printf( "Retiramos: %d\n", RemoveDoInicio( buf ) );
	PrintBuffer( NULL, buf );
	printf( "Retiramos: %d\n", RemoveDoInicio( buf ) );
	PrintBuffer( NULL, buf );
	printf( "Retiramos: %d\n", RemoveDoInicio( buf ) );
	PrintBuffer( NULL, buf );
	printf( "Retiramos: %d\n", RemoveDoInicio( buf ) );
	PrintBuffer( NULL, buf );
	
	return 0;
}
*/
/*
 * EOF
 */
