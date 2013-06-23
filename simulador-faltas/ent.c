/*
 * Ficheiro: ent.c
 */

#include "ent.h"

int leEntradas(Circuito* circuito, char* input)
{
  FILE* fp;
  char* inputs;
  int i,k;
  
  printf("Leitura das entradas: INICIO \n");
  
  /* 
   * O input pode ser um ficheiro ou o stdin.
   * Caso seja um ficheiro, este sera' aberto so para leitura.
   * Caso seja ao stdin a leitura e' como o habitual.
   */
  
  /* POR ENQUANTO a leitura apenas e' efectuada de um ficheiro. */
  fp = abreFicheiro( input, "r", "Erro ao abrir o ficheiro.ent");
  
  /* O numero de valores na entrada deve ser igual numero de entradas primarias. */
  inputs = (char*) malloc( ( circuito->nInputs + 1 ) * sizeof( char ) );
  memset( inputs, 0, ( circuito->nInputs + 1 ) * sizeof( char ));
  
  /* 
   * Recebe duma so' vez tantos simbolos como entradas tem o circuito, 
   * para nao intercalar IO com processamento.
   * (A ordem das entradas tem de ser a mesma que a ordem pela qual elas
   * aparecem no ficheiro do circuito ISCAS85.)
   */
  
  fgets( inputs, circuito->nInputs + 1 , fp );

  /* Fecha o ficheiro. */
  fechaFicheiro( fp, "Erro ao fechar o ficheiro ficheiro.ent" );
  
  /* 
   * Coloca os valores lidos nas estruturas correspondentes aos inputs
   * pela ordem de entrada.
   */
  
  for( i = 0, k = 0 ; i <= circuito->nNodes /*  && k < circuito->nInputs */; i++ )
    {
      /* As entradas primarias recebem um valor inicial. */
      if( 'i' == circuito->vectorNodes[ i ].flag ||
	  't' == circuito->vectorNodes[ i ].flag )
	{
	  switch( inputs[ k++ ] )
	    {
	    case '0': 
	      circuito->vectorNodes[ i ].valor = ZERO;
	      break;
	    case '1':
	      circuito->vectorNodes[ i ].valor = ONE;
	      break;
	    default:
	      circuito->vectorNodes[ i ].valor = X;
	    }
	}
      else
	{ 
	  /* Os restantes no's sao inicializados com o valor logico X. */
	  circuito->vectorNodes[ i ].valor = X;
	}
    }
  
  /* Liberta o vector de inputs. */
  free( inputs );
  
  printf("Leitura das entradas: FIM \n");
  
  return 0;
}
