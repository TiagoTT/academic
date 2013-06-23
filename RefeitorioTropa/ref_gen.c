/*
 * Processo Refietorio
 * Ficheiro: ref_gen.c
 */
#include "sincronizacao.h"
#include "mailbox.h"
#include "registo.h"

#include "ref_gen.h"

void *AtendeGenerais(void* argumento)
{
	pthread_t Generais[1];
	char* msg;
	unsigned key;
	/*
	 * 
	 */
	CriaMailbox( KEY_REFEITORIO );

	while(1){
		
		/* So recebe mensagens do tipo TIPO_INICIO */
		msg = RecebeMailMessage( &key, KEY_REFEITORIO, TIPO_INICIO );
		/* key contem agora a key (= ao pid) do general */
		puts( msg );
		free( msg );
		/* Regista a presenca do General */
		RegistaEntrada( key, GENERAL );

		/* Lanca a thread que serve o general que acabou de se apresentar. */
		pthread_create(&Generais[0], NULL, ThreadGeneral, (void*)&key);
	}
		
	return 0;
}

void *ThreadGeneral(void * arg)
{
	unsigned key;
	char* msg;
	Pedido* pedido;

	key = *(unsigned*)arg; /* Para evitar a utilizacao de semaforos 
			   * damos uma pequena "martelada" para realizar 
			   * a passagem do argumento por valor. 
			   */
	/*
	 * key identifica o processo General com o qual estamos a
	 * comunicar nesta thread
	 */
	
	/* Recebe o pedido do General */
	msg = RecebeMailMessage(&key, KEY_REFEITORIO, key);
	
	/* Processa o pedido */	
	puts( msg );
	pedido = ProcessaMsgGeneral( msg );
	free( msg );
	
	if( NULL != pedido )
	  {
	    /* Espera ordem de refeitorio */
	    EsperaSemaforoZero( BARREIRA_SERVE );
	    /* Espera ingredientes */
	    EsperaStock( pedido->prod, pedido->n_prod );
	    /* Prepara refeicao */
	    
	    /* Serve refeicao */
	    EnviaMailMessage(KEY_REFEITORIO, key, TIPO_COMIDA, msg );
	
	  }
	
	
	/* Espera anuncio do fim da refeicao */
	msg = RecebeMailMessage( &key, KEY_REFEITORIO, key );
	puts( msg );
	ProcessaMsgGeneral( msg );
	free( msg );
	
	/* Regista a saida do General */
	RegistaSaida( key );

	/* Neste nomento a ENTRADA encontra-se fechada. 
	 * Vamos refrescar a tabela de clientes, recarregar o stock, reiniciar
	 * os contadores...
	 * So depois disto é que permitimos que entrem mais Militares.
	 */
	ReiniciaClientes();
	DestroiStock();
	CriaStock();
	DefineSemaforo( INDICE_ESCOLHA, 0 );
	DefineSemaforo( MUTEX_INDICE_ESCOLHA, 1 );

	return 0;
}

Pedido* ProcessaMsgGeneral( char* msg )
{
  int i;
  char* buf;
  char nome[MAX_NOME];
  char quantidade[MAX_QUANT];
  Produto produtos[10];
  Pedido* pedido = NULL;

  buf = (char*)malloc(sizeof(char)*80);

  if( 0 == strncmp(msg, "Pedido:", strlen("Pedido:") ) ){
    
    strcpy( buf, &msg[7] );
    
    for( i = 0 ;  2 == sscanf( buf, "%s %s", nome, quantidade ) ; i++ )
      {
	strcpy( produtos[i].nome, nome );
	strcpy(produtos[i].quantidade, quantidade );
	/* Avanca no buf */
	buf = &buf[ strlen(nome) + strlen(quantidade) +1 ];
      }
    
    pedido = (Pedido*)malloc(sizeof(Pedido));
    pedido->prod = (Produto*)malloc( i * sizeof(Produto) );
    pedido->n_prod = i;
    
    for( i = 0 ; i < pedido->n_prod ; i++ )
      {
	strcpy( pedido->prod[i].nome, nome );
	strcpy( pedido->prod[i].quantidade, quantidade );
      }
    
  }else if ( 0 == strncmp(msg, "Saida", strlen("Saida") ) ) {
    pedido = NULL;
  }
  
  return pedido;
}
