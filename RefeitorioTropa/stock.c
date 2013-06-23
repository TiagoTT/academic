/***********************************************************************
 * falta testar as funcoes com os semaforos do stock
 * INICIALIZACAO DAS VARIAVEIS NA MEMORIA ANTES DE FAZER O MAPEAMENTTO 
 * PARA NAO APARECE OS CARACTERES @ 
 * Ficheiro: stock.c
 * 
 *Autor:
 ***********************************************************************/ 
#include "stock.h"

extern Stock stock; 

void CriaStock()
{
	int id;
	int i;
	
	struct{
		int val;  /*corresponde a uma union*/
	}arg;

	if( ( id = semget( STOCK, stock.n_prod, 0666|IPC_CREAT ) ) < 0 ) {
		fprintf( stderr, "Erro em CriaStock: semget().\n" );
		perror("Erro em CriaStock: semget().");
		exit( -1 );
	}
	
	for( i = 0 ; i < stock.n_prod ; i++ )
	{
		/* O valor colocado no semaforo "i" e' a quantidade do produto "i"*/	
		arg.val = atoi( stock.inicio[i].quantidade );
	
		if( semctl( id, i, SETVAL,arg ) < 0 ) {
			fprintf( stderr, "Erro em CriaStock: semctl().\n" );
			perror("Erro em CriaStock: semctl().");
			exit( -1 );
		}
	}
	
	stock.n_sem = stock.n_prod;
	
	return;
}

void AssinalaStock(Produto* produtos, int n_prod)
{
	
	struct sembuf* op;
	int id, i;

	if( ( id = semget( STOCK, 0, 0) ) < 0 ) {
		fprintf( stderr, "Erro em AssinalaStock: semget().\n" );
		exit( -1 );
	}

	op = (struct sembuf*)malloc(sizeof(struct sembuf)*n_prod);
	
	for( i = 0 ; i < n_prod ; i++ ) {
		op[i].sem_num = GetIndiceStock( produtos[i].nome );
		op[i].sem_op = atoi( produtos[i].quantidade );
		op[i].sem_flg = 0;
	}
	
	if( semop( id, op, n_prod ) < 0 ) {
		fprintf( stderr, "Erro em AssinalaStock: semop().\n" );
		exit( -1 );
	}

	free(op);
	return;
}

void EsperaStock(Produto* produtos, int n_prod)
{
	
	struct sembuf* op;
	int id, i;
	
	if( ( id = semget( STOCK, 0, 0 ) ) < 0 ) {
		fprintf( stderr, "Erro em EsperaSemaforo: semget().\n" );
		exit( -1 );
	}

	op = (struct sembuf*)malloc(sizeof(struct sembuf)*n_prod);
	
	for( i = 0 ; i < n_prod ; i++ ) {
		op[i].sem_num = GetIndiceStock( produtos[i].nome );
		op[i].sem_op = - atoi( produtos[i].quantidade );
		op[i].sem_flg = 0;
	}
	
	if( semop( id, op, n_prod ) < 0 ) {
		fprintf( stderr, "Erro em EsperaSemaforo: semop().\n" );
		exit( -1 );
	}

	return;
}

void DestroiStock()
{
	int id, i;

	if( ( id = semget( STOCK, 0, 0 ) ) < 0 ) {
		fprintf( stderr, "Erro em DestroiSemaforo: semget().\n" );
		exit( -1 );
	}	

	/*
	 * actualizar o ficheiro mapeado em memoria 
	 */
	for( i = 0 ; i < stock.n_sem ; i++ )
	{	
		sprintf( stock.inicio[i].quantidade, "%4d", semctl(id, i, GETVAL) );
	}

	if( semctl( id, 0, IPC_RMID) < 0 ) {
		fprintf( stderr, "Erro em DestroiSemaforo: semctl().\n" );
		exit( -1 );
	}

	return;
}

int DevolveValorStock(int indice)
{
	int id;
        int val;

	if( ( id = semget( STOCK, 1, 0) ) < 0 ) {
		fprintf( stderr, "Erro em DevolveValorStock: semget().\n" );
		exit( -1 );
	}

	if( ( val = semctl( id, indice, GETVAL ) ) < 0 ) {
		fprintf( stderr, "Erro em DevolveValorStock: semctl().\n" );
		perror("Erro em DevolveValorStock: semctl().");
		exit( -1 );
	}

	return val;
}

void AdicionaProdMemMap(char* nome, char* quantidade)
{
	size_t novadim;
	size_t fim;
	int nbytes; /*numero de bytes que ira ser adicionado*/
	/* 
	 * o pedacinho que foi ajustado para
	 * arredondar ao multiplo do PG_SZ
	 */
	size_t nleft;

	nleft = stock.dim - (stock.fim-stock.inicio)*sizeof(Produto);

	nbytes= sizeof(Produto);
	
	if(nleft < nbytes)
	{

		/* atribuicao da dimensao para ter espaco de adicionar o produto*/
		novadim = stock.dim + (1 + (nbytes-nleft)/PG_SZ)*PG_SZ;
		fim = (stock.fim)-(stock.inicio);
		/* apaga o conteudo mapeado anteriormente*/
		munmap(stock.inicio, stock.dim);
		/* o campo dimensao passa a ter a dimensao pretendida */
		stock.dim = novadim;
		assert( ftruncate( stock.fd, stock.dim ) >= 0 );
		/* mapea com a dimensao correcta*/
		assert( ( stock.inicio= mmap( NULL, stock.dim,
						PROT_READ | PROT_WRITE, MAP_SHARED,
						stock.fd, 0 ) ) != MAP_FAILED );
		stock.fim= &( stock.inicio[fim] );/* <=> stock.inico + fim */
						
	}


/*	memcpy( stock.fim->nome, nome, MAX_NOME*sizeof(char));*/
	strncpy( stock.fim->nome, nome, MAX_NOME );
	
/*	memcpy( stock.fim->quantidade, "0000", MAX_QUANT*sizeof(char));*/
	strncpy( stock.fim->quantidade, quantidade, MAX_QUANT );	
	stock.fim = &( stock.fim[1] );
	stock.n_prod++;
	
	return;
}

short GetIndiceStock(char* nome)
{
	short i;
	
	for(i=0 ; i < stock.n_prod ; i++)
	{
		if( strncmp( stock.inicio[i].nome, nome, MAX_NOME ) == 0)
		{
			 return i;
		}
	}
	
	/* Se nao encontrar */
	return -1;
}

void MapeiaFicheiroStock(char* nomefiche)
{
	struct stat infofich; /* estrtura que indica estado do ficheiro */

	assert( ( stock.fd = open(nomefiche,  O_RDWR | O_CREAT, 0666)) >=0 );

	fstat( stock.fd, &infofich); /* get file status */

	/* Verifica se o ficheiro de Stock tem um tamanho
	 * multiplo do tamaho de um produto. */
	
	/*	if( infofich.st_size % sizeof(Produto) )
		{
		fprintf(stderr, "O ficheiro de stock: %s nao e' valido.\n", nomefiche);
		exit( -1 );
		}
	*/

	/*Aumentar o tamanho do ficheiro para um numero inteiro de pagina*/
	stock.dim = (size_t)(1+infofich.st_size/PG_SZ)*PG_SZ;

	/* arredonda a dimensao do ficheiro a um multiplo do 
	 * page size superior ao tamanho original do ficheiro
	 * --int ftruncate(int fd, off_t lenght) */
	assert( ftruncate( stock.fd, stock.dim ) >= 0 );
	
	/*  Mapeia o ficheiro em memoria */
	assert( ( stock.inicio = mmap( NULL, stock.dim,
					PROT_READ | PROT_WRITE,
					MAP_SHARED, stock.fd, 0 ) ) != MAP_FAILED );
	/* Calcula a primeira posicaolivre */
	stock.fim = &( stock.inicio[ infofich.st_size/sizeof(Produto) ] );
	
	return;
}

void DesMapeiaFicheiroStock()
{
	munmap( stock.inicio, stock.dim);
	ftruncate( stock.fd, (stock.fim-stock.inicio)*sizeof(Produto));
	close( stock.fd );

	return;
}

