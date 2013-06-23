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

int deduz(Circuito* circuito)
{
  int i, k, n;
  Node* node;
  Node* input;
  int controlo;
  char* sa0;
  char* sa1;
 
  printf("Deducao de Faltas: INICIO \n");
  
  /* Isto nao vai ser bonito...
   * 
   */
  for( i = 0 ; i <= circuito->nNodes ; i++ )
    {
      node = circuito->vectorNodes + i;
      
      /* So analizamos os no's que realmente existem. */
      if( '\0' != node->flag )
	{
	  /* Inicializa os vectores de faltas do no'. */
	  node->sa0 = (char*)
	    malloc( ( circuito->nNodes + 1 ) * sizeof( char ) );
	  memset( node->sa0, 0, ( circuito->nNodes + 1 ) * sizeof( char ) );
	  
	  node->sa1 = (char*)
	    malloc( ( circuito->nNodes + 1 ) * sizeof( char ) );
	  memset( node->sa1, 0, ( circuito->nNodes + 1) * sizeof( char ) );
	  
	  node->leituras = 0;
	  controlo = valorControlo( circuito, i );
	  
	  /* Determina qual a operacao a realizar entre os conjuntos
	   * de faltas.
	   */
	  if( XOR == node->tipo || XNOR == node->tipo )
	    {
	      /* Dado que qualquer entrada duma porta xor domina a saida,
	       * estes casos requerem um tratamento diferente.
	       * Lista de faltas na saida =
	       *    uniao  {listas de faltas das entradas}
	       *    excepto {
	       *        interseccao da lista de faltas das 2 entradas  
	       *        (So e' valido para portas de 2 entradas!!!)
	       *            }
	       *    uniao   falta na saida (saida s-a-controlo)
	       *
	       */
	      
	      /* Preciso de vectores auxiliares para processar as
		   * entradas. O numero de no's considerados e' (no maximo) i.
		   */
	      sa0 = (char*)
		malloc( ( i ) * sizeof( char ) );
	      memset( node->sa0, 0, ( i ) * sizeof( char ) );
	      
	      sa1 = (char*)
		malloc( ( i ) * sizeof( char ) );
	      memset( node->sa0, 0, ( i ) * sizeof( char ) );
	      
	      /* Primeira passagem: Uniao. */
	      for( k = 0 ; k < node->fanin ; k++ )
		{
		  /* Todos os no's cuja lista de faltas vai ser consultada 
		   * devem ter um indice inferior ao do no' actual. 
		   */
		  input = circuito->vectorNodes + node->inputs[ k ];
		  
		  for( n = 0 ; n < i ; n++ )
		    { 
		      /* Uniao */
		      node->sa0[ n ] |= input->sa0[ n ];
		      node->sa1[ n ] |= input->sa1[ n ];
		    }
		}
	      
	      /* Segunda passagem: Interseccao. */
	      for( k = 0 ; k < node->fanin ; k++ )
		{
		  input = circuito->vectorNodes + node->inputs[ k ];
		  
		  for( n = 0 ; n < i ; n++ )
		    { 
		      /* Interseccao */
		      sa0[ n ] &= input->sa0[ n ];
		      sa1[ n ] &= input->sa1[ n ];
		    
		      
		      /* input->leituras ++; TESTE */
		      /* Se este vector ja foi lido na deducao da lista de faltas
		       * de todos os seus fanouts deixa de ser necessario.
		       */
		      if( input->leituras >= input->fanout )
			{
			  free( input->sa0 );
			  free( input->sa1 );
			}
		    }
		}
	      
	      /* Terceira passagem: Uniao excepto interseccao. */
	      for( k = 0 ; k < node->fanin ; k++ )
		{
		  for( n = 0 ; n < i ; n++ )
		    { 
		      /* Excepto Interseccao */
		      node->sa0[ n ] &= ( 1 & ( ~sa0[ n ] ) );
		      node->sa1[ n ] &= ( 1 & ( ~sa1[ n ] ) );
		    }
		}
	      
	      free( sa0 );
	      free( sa1 );
	      
	      /* Adiciona-se a falta na saida da porta. */
	      switch( node->valor )
		{
		case ZERO : 
		  node->sa0[ i ] = 0;
		  node->sa1[ i ] = 1;
		  break;
		case ONE :
		  node->sa0[ i ] = 1;
		  node->sa1[ i ] = 0;
		  break;
	     	case X :
	       	  node->sa0[ i ] = 1; /* ??? */
		  node->sa1[ i ] = 1;
		  break;                   
		default:
		  break;
		}
	      
	    }
	  else if( ! haEntradasControlo( circuito, i ) )
	    {
	      /* Se todas as entradas estao a um valor de nao controlo:
	       * Lista de faltas na saida =
	       *    uniao  {listas de faltas das entradas}
	       *    uniao   falta na saida (saida s-a-controlo)
	       */
	      
	      /* Percorre os inputs do no' e realiza a operacao OR (bit a bit)
	       * entre os elementos do vector de faltas do no' e os elementos
	       * do vector de faltas dos inputs.
	       */
	      for( k = 0 ; k < node->fanin ; k++ )
		{
		  /* Todos os no's cuja lista de faltas vai ser consultada 
		   * devem ter um indice inferior ao do no' actual. 
		   */
		  input = circuito->vectorNodes + node->inputs[ k ];
		  
		  for( n = 0 ; n < i ; n++ )
		    { 
		      /* Uniao */
		      node->sa0[ n ] |= input->sa0[ n ];
		      node->sa1[ n ] |= input->sa1[ n ];
		    }
		  
		  /* input->leituras ++; TESTE */
		  /* Se este vector ja foi lido na deducao da lista de faltas
		   * de todos os seus fanouts deixa de ser necessario.
		   */
		  if( input->leituras >= input->fanout )
		    {
		      free( input->sa0 );
		      free( input->sa1 );
		    }
		}
	      
	      /* Uniao, no' i s-a-(valorControlo) 
	       * Se nao ha entradas com o valor de controlo a saida
	       * nao tem o valor de controlo.
	       */
	      switch( node->valor )
		{
		case ZERO : 
		  node->sa0[ i ] = 0;
		  node->sa1[ i ] = 1;
		  break;
		case ONE :
		  node->sa0[ i ] = 1;
		  node->sa1[ i ] = 0;
		  break;
	     	case X :
	       	  node->sa0[ i ] = 1; /* ??? */
		  node->sa1[ i ] = 1;
		  break;                   
		default:
		  break;
		}
	    }
	  else
	    {
	      /* Se alguma das entradas esta' a um valor de controlo:
	       * Lista de faltas na saida =
	       *    interseccao {
	       *        interseccao {listas de faltas das entradas controlo}
	       *        complemento 
	       *             uniao {listas de faltas das entradas nao controlo}
	       *                }
	       *    uniao falta na saida (saida s-a-nao-controlo)
	       */

	      /* Como iniciamos as operacoes com uma interseccao temos de
	       * inicializar as primeiras i posicoes dos vectores de faltas
	       * com 11's (uns).
	       */
	      memset( node->sa0, 1, 
		      ( i ) * 
		      sizeof( char ) );
	      memset( node->sa1, 1, 
		      ( i ) * 
		      sizeof( char ) );
	      
	      for( k = 0 ; k < node->fanin ; k++ )
		{
		  /* Todos os no's cuja lista de faltas vai ser consultada 
		   * devem ter um indice inferior ao do no' actual. 
		   */
		  input = circuito->vectorNodes + node->inputs[ k ];
		  
		  if( controlo == input->valor )
		    {
		      
		      for( n = 0 ; n < i ; n++ )
			{
			  /* Interseccao */
			  node->sa0[ n ] &= input->sa0[ n ];
			  node->sa1[ n ] &= input->sa1[ n ];
			}
		    }
		  else
		    {
		      for( n = 0 ; n < i ; n++ )
			{
			  /* Interseccao do complemento, 
			   * equivalente a complemento da uniao.
			   */
			  node->sa0[ n ] &= ( ( ~input->sa0[ n ] ) );
			  node->sa1[ n ] &= ( ( ~input->sa1[ n ] ) );
			}
		    }
		  
		  /* input->leituras++; TESTE */
		  /* Se este vector ja foi lido na deducao da lista de faltas
		   * de todos os seus fanouts deixa de ser necessario.
		   */
		  if( input->leituras >= input->fanout )
		    {
		      free( input->sa0 );
		      free( input->sa1 );
		    }
		}
	      
	      /* Uniao, no' i s-a-(~valorControlo) 
	       * Se uma das entradas esta' com valor de controlo a 
	       * saida tambem esta'.
	       */
	      switch( node->valor )
		{
		case ZERO : 
		  node->sa0[ i ] = 0;
		  node->sa1[ i ] = 1;
		  break;
		case ONE :
		  node->sa0[ i ] = 1;
		  node->sa1[ i ] = 0;
		  break;
	     	case X :
	       	  node->sa0[ i ] = 1; /* ??? */
		  node->sa1[ i ] = 1;
		  break;                   
		default:
		  break;
		}
	    }
	}
    }

  printf("Deducao de Faltas: FIM \n");
  
  return 0;
}
