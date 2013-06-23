/*
 * Ficheiro: tabelas.h
 *
 * Estruturas de dados que suportam as tabelas de identificadores.
 * Cabecalhos das funcoes de manipulacao das tabelas de identificadores.
 * 
 */

#ifndef _TABELAS_H_
#define _TABELAS_H_

#include"stack.h"
#include"hash.h"

/* Tipo de dados boolean (true ou false) */
typedef enum { true, false } boolean;

/* Tipos de variaveis que podemos ter. */
typedef enum {
	inteiro,	/* Variavel inteira. */
	semaforo,	/* Semaforo. */
	canal_limitado,	/* Canal de inteiros limitado. */
	canal_ilimitado,/* Canal de inteiros ilimitado. */
	lista,		/* Lista de inteiros. */
	constante	/* Constante inteira. */
}tipo;

/* Elemento basico das tabelas de identificadores. */
typedef struct identificador_str {
	char* nome;	/* Nome do simbolo. */
	tipo t;		/* Conteudo do simbolo. */
	boolean init;	/* Inicializado ou nao. */
	boolean use;	/* Utilizado ou nao. */
}identificador;

/* Prototipos de funcoes. */
identificador* NovoIdentificador( char* nome, tipo t );

void table_push( Item** p );
void table_pop( Item** p );

void table_insert( Item* p, identificador* id );
identificador* table_find( Item* p, char* nome );

int id_match( void* ref, void* dados );

#endif
