/*
 * Ficheiro: sai.c
 */

#include "sai.h"

int escreveSaidas(Circuito* circuito, char* ficheiro )
{
  int i;
  
  printf("Escrita dos resultados: INICIO \n");
  
  /* Abrir o ficheiro para escrita. */
  
  /* ??? Sera que vale a pena ??? Pode ir para o 
   * stdout e ser redireccionado na shell !!!
   */
  
  /* Percorrer o circuito e escrever o nome e valor das saidas. */
  
  /* POR ENQUANTO escreve a informacao de todos os no's. */
  for( i = 0 ; i <= circuito->nNodes ; i++ )
    {
      if( '\0' != circuito->vectorNodes[ i ].flag )
	{
	  printf( "%d - %s => %d\n",
		  i, 
		  circuito->vectorNodes[ i ].nome, 
		  circuito->vectorNodes[ i ].valor );
	}
    }
  
  printf("Escrita dos resultados: FIM \n");
  
  return 0;
}
