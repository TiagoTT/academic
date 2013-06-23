/*
 * Ficheiro: stack.h
 *
 * Modulo de manipulacao de uma pilha.
 * Stack manipulation module.
 */

#ifndef _STACK_H_
#define _STACK_H_

typedef struct ItemStr {
	void* dados;
	struct ItemStr* next;
} Item;

/* Funcoes publicas. */
void stack_push( Item** base, void* dados );
void* stack_pop( Item** base );
void* stack_find( Item* base, int (*comparador)(), void* ref );

/* Funcoes privadas. */
void stack_show( Item* base, FILE* out );

#endif

/*
 * EOF
 */
