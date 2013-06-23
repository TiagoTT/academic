/*
 *	Ficheiro: yacc_util.c
 *	
 *	Modulo que contem funcoes utilizados no yacc
 *      	  para traduzir, concatenar strings. 
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"yacc_util.h"

/*
 * Concatena duas strings e devolve a string resultante.
 */
char* cola( char* s1, char* s2 ) {
	char* res;

	res = (char*)malloc( sizeof(char)*( strlen(s1)+strlen(s2)+1 ) );
	if( NULL == res ) {
		fprintf(stderr,"Erro ao alocar memoria em cola().\n");
		exit(-1);
	}

	strcpy( res, s1 );
	strcat( res, s2 );

	return res;
}

/*
 * Imprime um dado numero de tabs para um ficheiro.
 */
void identa( FILE* fp, unsigned char ident ) {
	while( ident-- )
		fputc( '\t', fp );
}

