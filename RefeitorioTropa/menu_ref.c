/*
 * Processo Refeitorio
 *
 * Ficheiro: menu.c
 */

#include "menu_ref.h"


extern Stock stock;
extern Clientes clientes;

extern pthread_t ThreadAtendeSoldados;	
extern pthread_t ThreadAtendeOficiais;	
extern pthread_t ThreadAtendeGenerais;

void Abastece(char* produto, char* quantidade)
{
	int indice;
	int n_prod_antes;
	Produto prod;
	
	n_prod_antes = stock.n_prod;

	/* Identifica produto. */
	indice = GetIndiceStock( produto );
	
	/* Representa os dados */
	strcpy( prod.nome , produto );
	strcpy( prod.quantidade , quantidade );

	/* Se o produto ja existia. */
	if(  stock.n_prod == stock.n_prod ){
	  
	  /* Assinala o semaforo correspondente. */
	  AssinalaStock( &prod, 1 );
	  
	}else{
	  /* Caso se trate de um produto novo (caso em que o n_prod 
	   * aumentou com a operacao de indentificacao) apenas o
	   * ficheiro mapeado em memoria e' alterado. */
	  AdicionaProdMemMap( produto, quantidade);
	}
	
	return;
}

void ListaPedidosPendentes(int verb)
{	
	return;
}

void ListaStock(int verb)
{ 
  int i;
  
  for( i = 0 ; i < stock.n_prod ; i++ )
    {
      printf("Produto: %s - %s unidades\n", 
	     stock.inicio[i].nome, 
	     stock.inicio[i].quantidade );
    }
  
  return;
}


void TerminaRefeitorio(int arg)
{
  int i, estado;

  /* Percorre a lista de Clientes e envia o sinal SIGUSER1 */
  for( i = 0 ; i < clientes.n_cli ; i++ )
    {
      /* Ordena ao processo que termine. */
      kill( clientes.vector[i].id, SIGINT );
      waitpid( clientes.vector[i].id, &estado,0 );
      /* Cancela a thread que o estava a atender. */
      pthread_cancel( clientes.vector[i].tid );
      pthread_join( clientes.vector[i].tid, NULL );
      /* Liberta a memoria ocupada pelo pedido. */
      free( clientes.vector[i].pedido );
    }
  
  pthread_cancel( ThreadAtendeSoldados );
  pthread_cancel( ThreadAtendeOficiais );
  pthread_cancel( ThreadAtendeGenerais );	
  
  pthread_join( ThreadAtendeSoldados, NULL );
  pthread_join( ThreadAtendeOficiais, NULL );
  pthread_join( ThreadAtendeGenerais, NULL );	
  
  /* Guarda as alteracoes feitas ao stock. */
  DesMapeiaFicheiroStock();
  /* Remove os segmentos de memoria partilhada. */

  /* Destroi os semaforos. */
  DestroiSemaforo( ENTRADA );
  DestroiSemaforo( SAIDA );
  DestroiSemaforo( PRESENTES );
  DestroiSemaforo( SERVIR_JANTAR );
  DestroiSemaforo( CONTAGEM );
  DestroiSemaforo( SENTIDO );
  DestroiSemaforo( PEDIDOS );
  DestroiSemaforo( MUTEX_INDICE_ESCOLHA );
  DestroiSemaforo( INDICE_ESCOLHA );
  DestroiSemaforo( SEM_LINHAS );
  DestroiSemaforo( SEM_COLUNAS );
  DestroiSemaforo( BARREIRA_LE );
  DestroiSemaforo( BARREIRA_SERVE );
  DestroiSemaforo( SOLDADOS );
  DestroiSemaforo( PEDIDOS_PRONTOS );
  DestroiSemaforo( CONTAGEM );

  /* No comments */
  DestroiMailbox( KEY_REFEITORIO );

  exit( 0 );
}



void Escreve_Menu_Ref(void)
{

puts("\t*=============================================================================");
puts("\t* comando argumetos           descricao   ");
puts("\t* ============================================================================");
printf("\t* a   produto quantidade Incrementar o stock de produto de quantidade unidades\n");
printf("\t* p       <v>                Listar pedidos pendentes        \n");
printf("\t* s       <v>                Listar stock de produtos        \n");
printf("\t* q                          Terminar ordeiramente a aplicacao concorrente\n");
puts("\t* ============================================================================");
putchar('\n');
 
}
void *Menu_Ref(void *argumento)
{
  char opcao;
  char quantidade[MAX_QUANT];
  char produto[MAX_NOME];
  
  while(1)
    {
      Escreve_Menu_Ref();
      opcao = getchar();
      
      switch( opcao )
	{
	case 'a':
	  scanf("%s %s", produto, quantidade);
	  printf("Abastece(%s,%s);\n", produto, quantidade);
	  Abastece(produto, quantidade);
	  break;
	case 'p':
	  opcao = getchar();
	  if('v' == opcao){
	    puts("ListaPedidosPendentesVerboso();");
	    ListaPedidosPendentes( 1 );
	  }
	  else
	    {
	      puts("ListaPedidosPendentes();");
	      ListaPedidosPendentes( 0 );
	    }	
	  break;
	case 's':
	  opcao = getchar();
	  if('v' == opcao){
	    puts("ListaStock(1);");
	    ListaStock(1);
	  }
	  else
	    { 
	      puts("ListaStock(0);");
	      ListaStock(0);
	    }	
	  break;
	case 'q':
	  puts("TerminaRefeitorio();");
	  TerminaRefeitorio(0);
	  return 0;
	  break;
	default:
	  puts("opcao invalida");
	  break;
	}
    }
  return 0;
}


