/*
 * Processo Refeitorio
 *
 * Ficheiro: menu.c
 */
#include "menu.h"
#include "stock.h"
#include "registo.h"

extern Stock stock;
extern Clientes clientes;

/*
 * =======================================================================================
 * comando argumetos           descricao
 * =======================================================================================
 * a       produto quantidade Incrementar o stock de produto de quantidade unidades
 * p       <v>                Listar pedidos pendentes
 * s       <v>                Listar stock de produtos
 * q                          Terminar ordeiramente a aplicacao concorrente
 * ======================================================================================
 */

void *Menu(void *arg)
{
	char opcao;
	char quantidade[MAX_QUANT];
	char produto[MAX_NOME];
	
	while(1)
	{
		puts("Opcoes disponiveis... a p s q");
		opcao = getchar();

		switch( opcao )
		{
			case 'a':
				scanf("%s %s", produto, quantidade);
				Abastece(produto, quantidade);
				printf("Abastece(%s,%s);\n", produto, quantidade);
				break;
			case 'p':
				opcao = getchar();
				if('v' == opcao){
					 ListaPedidosPendentes( 1 );
					puts("ListaPedidosPendentesVerboso();");

				}else{
					ListaPedidosPendentes( 0 );
					puts("ListaPedidosPendentes();");
				}	
				break;
			case 's':
				opcao = getchar();
				if('v' == opcao){
				        /* ListaStockVerboso();*/
					puts("ListaStockVerboso();");

				}else{
					ListaStock();
					puts("ListaStock();");
				}	
				break;
			case 'q':
				TerminaRefeitorio();
				puts("TerminaRefeitorio();");
				return 0;
				break;
			default:
				break;
		}
	}
	return 0;
}

void Abastece(char* produto, char* quantidade)
{
	int indice;
	int n_prod_antes;
	Produto produto;
	
	n_prod_antes = stock.n_prod;

	/* Identifica produto. */
	indice = GetIndiceStock(produto);
	
	/* Representa os dados */
	strcpy( produto.nome , produto );
	strcpy( produto.quantidade , quantidade );

	/* Se o produto ja existia. */
	if(  stock.n_prod == stock.n_prod ){
	  
	  /* Assinala o semaforo correspondente. */
	  AssinalaStock( produto, 1 );
	  
	}else{
	  /* Caso se trate de um produto novo (caso em que o n_prod 
	   * aumentou com a operacao de indentificacao) apenas o
	   * ficheiro mapeado em memoria e' alterado. */
	  AdicionaFicheiroStock( indice, quantidade );
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
		printf("Produto: %s - %d unidades\n", Produtos.inicio[i].nome, );
	}
	
	return;
}


void TerminaRefeitorio()
{
	/* Percorre a lista de Clientes e envia o sinal SIGUSER1 */
	
	return;
}


