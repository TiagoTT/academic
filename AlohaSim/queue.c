/*
 * queue.c
 *
 * Implementacao de fila prioritaria em acervo
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"
#include "simulador.h"

/*
 * Aloca memoria e devolve uma ponteiro para a estrutura.
 */
pq IniciaFila( int size ) {	
  pq fila;

  fila = ( pq )malloc( sizeof( FilaPrioritaria ) );
  if( NULL == fila ){
    fprintf(stderr, "ERRO: malloc() = NULL in IniciaFila()\n");
    exit( -1 );
  }
  /* A tabela e' indexada a 1. */
  fila -> table = (void**)malloc( sizeof( void* ) * (size+1) );
  if( NULL == fila->table ){
    fprintf(stderr, "ERRO: malloc() = NULL in IniciaFila()\n");
    exit( -1 );
  }
  memset( fila -> table, (int)NULL, sizeof( void* ) * (size+1) );
  fila -> n_items = 0;
  fila -> size = size;
	
  return fila;
}

/*
 * Adiciona um elemento 'a fila, mantendo a ordenecao.
 * Retorna o numero de posicoes livres.
 */
int ColocaNaFila( pq fila, void* item ) {

  fila -> n_items ++;
  fila -> table[ fila -> n_items ] = item;
  FixUp( fila -> n_items , fila );

  return fila -> size - fila -> n_items;
}

/*
 * Retira e devolve o primeiro elemento da fila.
 */
void* RetiraDaFila( pq fila ) {
  void* top;

  top = fila -> table[ 1 ];
  fila -> table[ 1 ] = fila -> table[ fila -> n_items ];
  fila -> n_items --;
  FixDown( 1, fila );

  return top;
}

/*
 * Liberta a memoria utilizada pela fila.
 */
void ApagaFila( pq fila ) {
  int i;

  for( i = 1 ; i <= fila->n_items ; i++ ) {
    /* Isto nao devi aser necessario! */
    free( fila -> table[i] ); 
  }
	
  free( fila -> table );
  free( fila );

  return;
}


/*
 * Assegura a ordenacao "por ali a cima".
 * Retorna o numero de posicoes que o elemento subiu na fila.
 */
int FixUp( int n, pq fila ) {
  void* aux;
  int i;

  for( i = n ; i > 0 ; i = i/2 ) {
    /* Se o elemento i for mais prioritario que o elemento i/2. */
    if( FIRST( fila -> table[i], fila -> table[i/2] ) ) {
      /* Tocam-se os elementos. */
      TROCA( i , i/2 );
    }else{
      break;
    }
  }
	
  return n - i;
}

/*
 * Assegura a ordenacao "por ali a baixo".
 * Retorna o numero de posicoes que o elemento desceu na fila.
 */
int FixDown( int n, pq fila ) {
  void* aux;
  int i, child;

  for( i = n ; i <= ( fila -> n_items )/2 ;  ) {
    child = i*2;
    /* Determina-se qual dos descendentes tem maior prioridade. */
    if( child < (fila -> n_items) /* existe o segundo descendente */ && 
	FIRST( fila -> table[ child+1 ], fila -> table[child] ) ) {
      child ++;
    }
		
    /* Se o descendente for mais prioritario que o elemento (i).  */
    if( FIRST( fila -> table[ child ], fila -> table[i] ) ) {
      /* Tocam-se os elementos. */
      TROCA( child , i );
      i = child;
    }else{
      break;
    }
  }

  return i - n;
}

/*
 * Imprime informacao sobre a fila.
 */
void PrintQueue( FILE* output, pq fila ) {
  int i;
  FILE* fp;
	
  if( NULL != output ) fp = output;
  else fp = stdout;
	
  fprintf( fp, "\nPrintQueue():INICIO\n" );
  fprintf( fp, "\nfila->n_items = %d\n", fila->n_items );
  fprintf( fp, "fila->size = %d\n", fila->size );
	
  for( i = 1 ; i <= ( fila -> n_items ) ; i++ ) {
    fprintf( fp, "fila->table[%d] = %d\n", i, (int)fila->table[i]);
    /* PRINT() */
  }

  fprintf( fp, "\nPrintQueue():FIM\n" );
}



/*
 * Inicializa o BufferFIFO.
 */
bf IniciaBuffer( void ) {
  bf buf;

  buf = (bf) malloc( sizeof(BufferFIFO) );
  if( NULL == buf ){
    fprintf(stderr, "ERRO: malloc() = NULL in IniciaBuffer\n");
    exit( -1 );
  }
  memset( buf -> table, (int)NULL, sizeof(void*) * B_FIFO_SIZE );
  buf -> head = 0;
  buf -> free = 0;
  
  return buf;
}

/*
 * Insere elemento no BufferFIFO.
 * Devolve o numero de posicoes livres no buffer.
 */
int ColocaNoFim( bf buf, void* item ) {
	
  /* Verifica se ha espaco livre. */
  if( NULL != buf -> table[ buf -> free ] ) {
    fprintf( stdout, "BufferFIFO: elemento perdido."\
	     "(head,free)=(%ld,%ld)\n", buf->head, buf->free );
    return -1;
  }

  buf -> table[ buf -> free ] = item;
  buf -> free = (buf -> free + 1) & BIT_MASK;
	
  return (buf->free > buf->head)?
    ( B_FIFO_SIZE - (buf->free - buf->head) ) :
    ( buf->head - buf->free );
}

/*
 * Retira elemento do BufferFIFO.
 * Devolve o elemento.
 */
void* RemoveDoInicio( bf buf ) {
  void* item;

  item = buf -> table[ buf -> head ];
  if( NULL == item ){
    fprintf( stdout, "Buffer vazio in RemoveDoInicio."\
	     "(head,free)=(%ld,%ld)\n", buf->head, buf->free );
    return NULL;
  }
  buf -> table[ buf -> head ] = NULL;
  buf -> head = (buf -> head + 1) & BIT_MASK;
	
  return item;
}

/*
 * Consulta o proximo a sair do BufferFIFO.
 * Devolve o elemento.
 */
void* ConsultaInicio( bf buf ) {
	
  return buf -> table[ buf -> head ];
}

void ApagaBuffer( bf buf ) {
  int i,k;
  
  for( i = 0, k = 0 ; i < B_FIFO_SIZE ; i++ )
    if( NULL != buf->table[i] )
      k++;
  if( k )
    fprintf( stderr, "Possivel memory leak in ApagaBuffer: %d items\n", k );
  
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
	
  for( i = 0 ; i < B_FIFO_SIZE ; i++ ) {
    fprintf( fp, "buf->table[%d] = %d\n", i, (int)buf->table[i]);
    /* PRINT() */
  }

  fprintf( fp, "\nPrintBuffer():FIM\n" );
}

/*
 * EOF
 */
