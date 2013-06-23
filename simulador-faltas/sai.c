/*
 * Ficheiro: sai.c
 */

#include "sai.h"

int escreveSaidas(Circuito* circuito, char* ficheiro )
{
  int i, k;
  Node* node;
  printf("Escrita dos resultados: INICIO \n");
  
  /* Abrir o ficheiro para escrita. */
  
  /* ??? Sera que vale a pena ??? Pode ir para o 
   * stdout e ser redireccionado na shell !!!
   */
  
  /* Percorrer o circuito e escrever o nome e valor das saidas. */
  
  /* POR ENQUANTO escreve a informacao de todos os no's. */
  printf( "Valores das saidas (Resultado da simulacao):\n" );
  for( i = 0 ; i <= circuito->nNodes ; i++ )
    {
      node = circuito->vectorNodes + i ;
      if( 'o' == node->flag || 't' == node->flag || '\0' != node->flag )
	{
	   printf( "%d - %s => %d\n", i, node->nome, node->valor ); 
	   printf( "Faltas detectadas: " );
	   for( k = 0 ; k <= circuito->nNodes ; k++ )
	     {
	       printf( "(%hd%hd)", node->sa0[ k ], node->sa1[ k ] );
	       /*
	       if( node->sa0[ k ] )
		 {
		   printf("%s sa0; ", circuito->vectorNodes[ k ].nome );
		 }
	       else if( node->sa1[ k ] )
		 {
		   printf("%s sa1; ", circuito->vectorNodes[ k ].nome );
		 }
	       */
	     }
	   putchar('\n');
	}
    }
  
  printf("Escrita dos resultados: FIM \n");
  printf( "\n*** sizeof(unsigned short int) = %d ***\n", sizeof(unsigned short int) );
  printf( "\n*** sizeof( short int) = %d ***\n", sizeof( short int) );
  printf( "\n*** sizeof( int) = %d ***\n", sizeof( int) );
  printf( "\n*** sizeof(char) = %d ***\n", sizeof(char) );
  
  return 0;
}
