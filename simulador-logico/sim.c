/*
 * Ficheiro: sim.c
 */

#include "sim.h"

int simula(Circuito* circuito)
{
  int i;
  
  printf("Simulacao Logica: INICIO \n");
  
  /* Os valores de todos os no's, excepto as entradas primarias,
   * devem ser calculados.
   */
  for( i = 0 ; i <= circuito->nNodes ; i++ )
    {
      if( 'n' == circuito->vectorNodes[ i ].flag ||
	  'o' == circuito->vectorNodes[ i ].flag )
	{
	  calculaValor( circuito, i );
	}
    }
  
  printf("Simulacao Logica: FIM \n");
  
  return 0;
}
