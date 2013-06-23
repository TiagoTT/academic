/*
 * Ficheiro: cir.c
 */

#include "cir.h"

#define LINE_SIZE 80
#define WORD_SIZE 6  /* ATENCAO (WORD_SIZE-1) esta' "hardcoded" em 
		     * --> cir.c:65
		     * dentro duma format string da funcao sscanf.
		     * A alteracao deste #define deve ser propagada!
		     */

Circuito* leCircuito(char* ficheiro)
{
  int i;
  FILE* fp;
  Circuito* circuito;
  char line[ LINE_SIZE ];
  int indice;
  char tipo[ WORD_SIZE ];
  char input[ MAX_NOME ]; /* Usado para o "from". */
  int fanin;              /* Numero de inputs do no'. */
  int fanout;             /* Para testar se o no' e' uma saida primaria. */
  
  char* token;
  char* lasts;
  
  printf("Leitura do circuito: INICIO \n");
  
  /* Abertura do ficheiro para leitura. */
  fp = abreFicheiro( ficheiro, "r", "Erro ao abrir o ficheiro.cir" );
  
  /* Cria as estruturas de memoria que representam o circuito. */
  circuito = criaCircuito();
    
  /* Armazenamento da informacao dos nos. */
  while( NULL != fgets( line, LINE_SIZE,fp ) )
    {
      /* Ignora os comentarios. */
      if( '*' == line[0] || '#' == line[0] )
	continue;
      
      /* Le o endereco do no' que deve estar no inicio da linha. */
      if( 1 != sscanf( line, "%d", &indice ) )
	{
	  fprintf( stderr, "Erro a processar a linha: %s \n", line );
	  limpaCircuito( circuito );
	  fechaFicheiro( fp, "Erro ao fechar o ficheiro.cir" );
	  exit( -1 );
	}
      
      /* Verifica se e' necessario aumentar o vectorNodes. */
      while( indice >= circuito->nMax )
	{
	  maisNodes( circuito );
	}
      
      /* Actualiza o valor nNodes. */
      circuito->nNodes = indice;

      /* Actualiza o valor realNodes. */
      circuito->realNodes ++;
      
      /* Guarda a informacao relevante na estrutura do Node correspondente. */
      sscanf( line, "%*d%15s", circuito->vectorNodes[ indice ].nome );
      sscanf( line, "%*d%*s%5s", tipo );
      circuito->vectorNodes[ indice ].tipo = traduzTipo( tipo );
      
      /* No caso de uma ligacao (from) apenas lemos o nome do no' de origem. */
      if ( ( strcmp( tipo, "from") == 0 ) || ( strcmp( tipo, "FROM") == 0 ) )
	{
	  /* O no' tem apenas uma entrada. */
	  circuito->vectorNodes[ indice ].fanin = 1;
	  circuito->vectorNodes[ indice ].inputs = (int*) malloc( sizeof( int ) );
	  
	  /* Leitura e armazenamento do no' de origem. */
	  sscanf( line, "%*d%*s%*s%15s", input );
	  circuito->vectorNodes[ indice ].inputs[ 0 ] = indiceDoNome( circuito, input );
	  circuito->vectorNodes[ indice ].flag = 'n'; /* node */
	  circuito->vectorNodes[ indice ].fanout = 1;
	}
      else
	{
	  /* Leitura do fanout do no' para marcacao das saidas primarias. */
	  sscanf( line, "%*d%*s%*s%d", &fanout );
	  circuito->vectorNodes[ indice ].fanout = fanout;
	  
	  /* Ler numero de entradas, alocar e preencher vector de inputs. */
	  sscanf( line, "%*d%*s%*s%*d%d", &fanin );
	  circuito->vectorNodes[ indice ].fanin = fanin;
	  
	  /* As entradas primarias nao tem fanin. */
	  if( 0 == fanin )
	    {
	      circuito->vectorNodes[ indice ].flag = 'i'; /* input */
	      circuito->vectorNodes[ indice ].inputs = NULL;
	      
	      /* Podem existir linhas das entradas directas para as saidas. */
	      if( 0 == fanout )
		circuito->vectorNodes[ indice ].flag = 't'; /* transfer */
	      
	      /* Incrementa o contador de entradas primarias. */
	      circuito->nInputs ++;
	    }
	  else
	    { /* Marcacao do no'. */
	      if( 0 == fanout )
		circuito->vectorNodes[ indice ].flag = 'o'; /* output */
	      else
		circuito->vectorNodes[ indice ].flag = 'n'; /* node */
	      
	      circuito->vectorNodes[ indice ].inputs = (int*) malloc( fanin * sizeof( int ) );
	      
	      /* Leitura da linha dos inputs. */
	      if( NULL == fgets( line, LINE_SIZE,fp ) )
		{
		  /* A string line fica com a linha anterior. */
		  fprintf( stderr, "Erro a ler os inputs de: %s \n", line );
		  limpaCircuito( circuito );
		  fechaFicheiro( fp, "Erro ao fechar o ficheiro.cir" );
		  exit( -1 );
		}
	      
	      /* Armazenamento dos enderecos dos inputs. */
	      token = strtok_r( line, " ", &lasts );
	      for( i = 0 ; i < fanin ; i++ )
		{
		  if( 1 != sscanf( token, "%d", circuito->vectorNodes[ indice ].inputs + i ) )
		    {
		      fprintf( stderr, "Erro a ler os inputs de: %s \n", line );
		      limpaCircuito( circuito );
		      fechaFicheiro( fp, "Erro ao fechar o ficheiro.cir" );
		      exit( -1 );
		    }
		  token = strtok_r( NULL, " ", &lasts );
		}
	    }
	}
      
      /* Mostra o que leu. Apenas para debug. */
      /*
      printf( "Indice:\t%d\tNome:\t%s\tTipo:\t%d\tFLAG:%c\t<= %s", 
              indice, 
	      circuito->vectorNodes[ indice ].nome, 
	      circuito->vectorNodes[ indice ].tipo, 
	      circuito->vectorNodes[ indice ].flag,
	      line );

      printf("Inputs:");

      for( i = 0 ; i < circuito->vectorNodes[ indice ].fanin ; i++ )
	{
	  printf( " %d ", circuito->vectorNodes[ indice ].inputs[i] );
	}
      putchar('\n');
      */
    } /* while() */
  
  printf( "Numero de INPUTS: \t%d \n", circuito->nInputs );
  printf( "Numero de NODES~: \t%d (Indice maximo do ISCAS85)\n", circuito->nNodes );
  printf( "Numero de NODES: \t%d (Numero real de nodes)\n", circuito->realNodes );
  
  /* Fecho do ficheiro. */
  fechaFicheiro( fp, "Erro ao fechar o ficheiro.cir" );
  
  /* Segunda passagem para efectuar ligacoes implicitas. */
  /* ??? */
  
  printf("Leitura do circuito: FIM \n");
  
  return circuito;
}

Circuito* criaCircuito()
{
  Circuito* circuito;
  
  /* Aloca memoria para a estrutura base do circuito. */
  if( NULL == ( circuito = (Circuito*) malloc( sizeof( Circuito ) ) ) )
    {
      fprintf( stderr, "Pedido de malloc de circuito de %d bytes FALHOU !!! \n",
	       sizeof( Circuito ) );
      exit( -1 );
    }
  
  /* Aloca memoria para o vector que contem os no's. */
  if( NULL == ( circuito->vectorNodes = 
		(Node*) malloc( ( INIT_VEC ) * sizeof( Node ) ) ) )
    {
      fprintf( stderr, "Pedido de malloc de vectorNodes de %d bytes FALHOU !!! \n",
	       INIT_VEC * sizeof( Node ) );
      exit( -1 );
    }
  
  /* Inicializa a estrutura do circuito. */
  circuito->nNodes  = 0;
  circuito->nMax    = INIT_VEC;
  circuito->nInputs = 0;
  circuito->realNodes = 0;
  
  /* Inicializa o vector que contem os no's. */
  memset( circuito->vectorNodes, 0, ( INIT_VEC ) * sizeof( Node ) );
  
  return circuito;
}

void maisNodes(Circuito* circuito)
{
  /* Realoca o vector.
   * Isto e' MAU, INEFICIENTE e FEIO.
   * Mas com jeitinho ... tambem funciona.
   */
  if( NULL == ( circuito->vectorNodes = (Node*) realloc( circuito->vectorNodes,
					   ( circuito->nMax + GROW_VEC ) 
						       * sizeof( Node ) ) ) )
    {
      fprintf( stderr, "Pedido de realloc de vectorNodes de %d bytes para"
	       "%d bytes FALHOU !!! \n",
	       ( circuito->nMax ) * sizeof( Node ),
	       ( circuito->nMax + GROW_VEC ) * sizeof( Node )  );
      exit( -1 );
    }
  
  /* Inicializa as novas posicoes do vector. */
  memset( circuito->vectorNodes + circuito->nMax,
	  0,
	  ( GROW_VEC ) * sizeof( Node ) );
  
  /* Actualiza o tamanho do vector. */
  circuito->nMax += GROW_VEC;
  
  return;
}

void limpaCircuito(Circuito* circuito)
{
  int i;
  Node* node;
  
  /* Limpa e liberta os inputs e os vectores de faltas dos no's. */
  for( i = 0 ; i <= circuito->nNodes ; i++ )
    {
      node = circuito->vectorNodes + i ;
      
      if( 'o' == node->flag || 't' == node->flag || '\0' != node->flag )
	{
	  memset( node->sa0, 0,
		  ( circuito->nNodes + 1 ) * sizeof( char ) );
	  free( node->sa0 );
	  
	  memset( node->sa1, 0,
		  ( circuito->nNodes + 1 ) * sizeof( char ) );
	  free( node->sa1 );
	}
      
      if( 'o' == node->flag || 'n' == node->flag )
	{
	  memset( node->inputs, 0, node->fanin * sizeof( int ) );
	  free( node->inputs );
	}
    }
  
  /* Limpa e liberta o vector que contem os no's. */
  memset( circuito->vectorNodes, 0, ( circuito->nMax ) * sizeof( Node ) );
  free( circuito->vectorNodes );
  
  /* Limpa e liberta a estrutura base do circuito. */
  memset( circuito, 0, sizeof( Circuito ) );
  free( circuito );

  return;
}

int traduzTipo(char* tipo)
{
  /* ligacao (from) */
  if ( ( strcmp( tipo, "from") == 0 ) || ( strcmp( tipo, "FROM") == 0 ) )
    return OR;
  
  /* input */
  else if ( ( strcmp( tipo, "inpt") == 0 ) || ( strcmp( tipo, "INPT") == 0 ) )
    return OR;
  
  /* buffer */
  else if ( ( strcmp( tipo, "buff") == 0 ) || ( strcmp( tipo, "BUFF") == 0 ) )
    return OR;
  
  /* porta logica NOT */
  else if ( ( strcmp( tipo, "not") == 0 ) || ( strcmp( tipo, "NOT") == 0 ) )
    return NOR;

  /* porta logica AND */
  else if ( ( strcmp( tipo, "and") == 0 ) || ( strcmp( tipo, "AND") == 0 )  )
    return AND;
  
  /* porta logica OR */
  else if ( ( strcmp( tipo, "or") == 0 ) || ( strcmp( tipo, "OR") == 0 ) )
    return OR;
  
  /* porta logica XOR */
  else if ( ( strcmp( tipo, "xor") == 0 ) || ( strcmp( tipo, "XOR") == 0)  )
    return XOR;
  
  /* porta logica NAND */
  else if ( ( strcmp( tipo, "nand") == 0 ) || ( strcmp( tipo, "NAND") == 0 ) )
    return NAND;
  
  /* porta logica NOR */
  else if ( ( strcmp( tipo, "nor") == 0 ) || ( strcmp( tipo, "NOR") == 0 ) )
    return NOR;
  
  /* porta logica XNOR */
  else if ( ( strcmp( tipo, "xnor") == 0 ) || ( strcmp( tipo, "XNOR") == 0 ) )
    return XNOR;
  
  else 
    {
      fprintf(stderr,"Tipo desconhecido: %s -> ficheiro.cir invalido.\n", tipo );
      return -1;
    }
}

int indiceDoNome(Circuito* circuito, char* nome)
{
  int i;
  
  /* A procura e' realizada do fim para o inicio. 
   * Pelo principio da localidade espacial:
   * as entradas de um no' provem de no's proximos.
   * Logo, mais vale comecar 'a procurar pelo fim 
   * (onde estao os no's mais recentes).
   */
  for( i = circuito->nNodes; i >= 0 ; i-- )
    if( 0 == strcmp( circuito->vectorNodes[ i ].nome, nome ) )
      return i;
  
  return 0;
}
