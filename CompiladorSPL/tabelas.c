/*
 * Ficheiro: tabelas.c
 *
 * Codigo das funcoes de manipulacao das tabelas de identificadores.
 * 
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"tabelas.h"

/*
 * Cria um novo identificador.
 * Retorna um ponteiro para ele.
 */
identificador* NovoIdentificador( char* nome, tipo t ) {
	identificador* id;

	id = (identificador*)malloc( sizeof( identificador ) );
	if( NULL == id ) {
		fprintf(stderr, "ERRO: malloc() = NULL in NovoIdentificador\n");
		exit( -1 );
	}

	id -> nome = nome;
	id -> t = t;
	id -> init = false;
	id -> use = false;
	
	return id;
}

/*
 * Liberta os recursos alocados para este identificador.
 */
void ApagaIdentificador( void* dados ) {
	free( ((identificador*)dados) -> nome );
	free( ((identificador*)dados) );
}

/*
 * Adidiona uma nova tabela de identificadores no topo da pilha
 * Corresponde a entrar num bloco de codigo mais aninhado.
 */
void table_push( Item** p ) {
	stack_push( p, (void*)hash_init() );
}

/*
 * Remove uma tabela de identificadores do topo da pilha
 * Corresponde a sair de um bloco de codigo mais aninhado.
 */
void table_pop( Item** p ) {
	Item** table;
	
	table = (Item**)stack_pop( p );
	hash_kill( table, ApagaIdentificador );
}

/*
 * Insere um identificador na tabela do topo da pilha de tabelas.
 */
void table_insert( Item* p, identificador* id ) {
	Item** table;

	if( NULL == p ) {
		fprintf(stderr,"Erro: Tentativa de insercao de um identificador numa pilha de tabelas vazia.\n");
		exit(-1);
	}
	table = (Item**)(p -> dados);

	hash_insert( table, (void*)id );
}

/*
 * Procura um identificador pelo nome.
 * Devolve um ponteiro para esse identificador caso o encontre,
 * caso contrario devolve NULL.
 */
identificador* table_find( Item* p, char* nome ) {
	Item* it;
	identificador* id;
	identificador* ref;

	/* Apenas interessa dafinir o nome. */
	ref = NovoIdentificador( nome, inteiro );
	
	for( it = p ; NULL != it ; it = it -> next ) {
		if( NULL != ( id = hash_find(
					(Item**)(it -> dados),
					id_match,
					(void*)ref ) ) ) {
			free( ref );
			return id;
		}
	}
	/* Nao se faz free do nome, pois quem faz a pesquisa nao 
	 * ia achar piada se a sua string desaparecesse. 
	 */
	free( ref );
		
	return NULL;
}

/*
 * Funcao que avalia a correspondencia entre um nome e um identificador.
 * Serve para encontrar um identificador pelo nome.
 * Devolve zero caso tenha sucesso.
 */
int id_match( void* ref, void* dados ) {
	return strcmp( ((identificador*)ref)->nome,
			((identificador*)dados)->nome );
}

/*
 * Testes
 */
/*
int main() {
	Item* pilha = NULL;

	char *nome, *cop;

	nome = (char*)malloc(2);
	nome[0] = 'x';
	nome[1] = '\0';
	cop = (char*)malloc(2);
	cop[0] = 'x';
	cop[1] = '\0';
	
	table_push( &pilha );
	table_insert( pilha, NovoIdentificador( nome, inteiro ) );
	table_push( &pilha );
	table_find( pilha, cop );
	table_pop( &pilha );
	table_pop( &pilha );
	
	exit(0);
}
*/

/*
 * EOF
 */
