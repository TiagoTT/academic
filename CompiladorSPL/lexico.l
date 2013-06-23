%{
/* Analise e classificacao do input. */
#include<stdio.h>
#include<stdlib.h>

#include"y.tab.h"

extern FILE* out;

%}

%option yylineno

id	[_a-zA-Z][_a-zA-Z0-9]*
inteiro	[0-9]+
espaco	[\ \t\n\v]

operador_arit	[\+\-\*\/]
operador_bool	((\&\&)|(\|\|)|(\!))
operador_logic	((\=\=)|(\!\=)|(\<\=)|(\>\=)|(\<)|(\>))	
operando 	({id}|{inteiro})

expr_arit	(\-)?{espaco}*{operando}{espaco}*({operador_arit}{espaco}*{operando}{espaco}*)*
expr_bool	{espaco}*{operando}{espaco}*({operador_bool}{espaco}*{operando}{espaco}*)*
expr_logic	{espaco}*{operando}{espaco}*({operador_logic}{espaco}*{operando}{espaco}*)*

expr	({expr_arit}|{expr_bool}|{expr_logic})
condicao	(\!)?{expr}{espaco}*({operador_logic}{espaco}*{expr}{espaco}*)*

%%

int	{ 
	/* Declaracao de uma variavel do tipo int. */
	fprintf( out, "%3d: Declaracao de int: \"%s\"\n", yylineno, yytext);
	return INT;
}

semaphore	{ 
	/* Declaracao de uma variavel do tipo semaphore. */
	fprintf( out, "%3d: Declaracao de semaphore: \"%s\"\n", yylineno, yytext);
	return SEMAPHORE;
}

channel {
	/* Declaracao de uma variavel do tipo channel. */
	fprintf( out, "%3d: Declaracao de channel: \"%s\"\n", yylineno, yytext);
	return CHANNEL;
}

size	{
	/* Definicao opcional da dimensao da variavel do tipo channel. */
	fprintf( out, "%3d: Definicao da dimensao: \"%s\"\n", yylineno, yytext);
	return SIZE;
}

list	{
	/* Declaracao de uma variavel do tipo list. */
	fprintf( out, "%3d: Declaracao de list: \"%s\"\n", yylineno, yytext);
	return LIST;
}

const	{ 
	/* Declaracao de uma constante do tipo int. */
        fprintf( out, "%3d: Declaracao de int: \"%s\"\n", yylineno, yytext);
	return CONST;
}

of	{ 
	/* Conjuncao of. */
        fprintf( out, "%3d: Conjuncao of: \"%s\"\n", yylineno, yytext);
	return OF;
}

bounded	{ 
	/* Adjectivo bounded. */
        fprintf( out, "%3d: Adjectivo bounded: \"%s\"\n", yylineno, yytext);
	return BOUNDED;
}

by	{ 
	/* Conjuncao by. */
        fprintf( out, "%3d: Conjuncao by: \"%s\"\n", yylineno, yytext);
	return BY;
}

\=\=	{
	/* Igual. */
	fprintf( out, "%3d: Igual: \"%s\"\n", yylineno, yytext);
	return IGUAL;
}

\!\=	{
	/* Diferente. */
	fprintf( out, "%3d: Difrente: \"%s\"\n", yylineno, yytext);
	return DIFERENTE;
}

\>\=	{
	/* Maior ou igual. */
	fprintf( out, "%3d: Maior ou igual: \"%s\"\n", yylineno, yytext);
	return MAIOR_IGUAL;
}

\<\=	{
	/* Menor ou igual. */
	fprintf( out, "%3d: Menor ou igual: \"%s\"\n", yylineno, yytext);
	return MENOR_IGUAL;
}

\&\&	{
	/* AND logico. */
	fprintf( out, "%3d: AND logico: \"%s\"\n", yylineno, yytext);
	return AND;
}

\|\|	{
	/* OR logico. */
	fprintf( out, "%3d: OR logico: \"%s\"\n", yylineno, yytext);
	return OR;
}

if	{
	/* Instrucao if. */
 	fprintf( out, "%3d: Instrucao IF: \"%s\"\n", yylineno, yytext);
	return IF;
}

else	{
	/* Instrucao else. */
	fprintf( out, "%3d: Instrucao ELSE: \"%s\"\n", yylineno, yytext);
	return ELSE;
}

while	{
	/* Instrucao while. */
	fprintf( out, "%3d: Instrucao WHILE: \"%s\"\n", yylineno, yytext);
	return WHILE;
}

procreate	{
	/* Instrucao procreate. */
	fprintf( out, "%3d: Instrucao PROCREATE: \"%s\"\n", yylineno, yytext);
	return PROCREATE;
}

request	{
	/* Instrucao request. */
	fprintf( out, "%3d: Instrucao REQUEST: \"%s\"\n", yylineno, yytext);
	return REQUEST;
}

release	{
	/* Instrucao release. */
	fprintf( out, "%3d: Instrucao RELEASE: \"%s\"\n", yylineno, yytext);
	return RELEASE;
}

print	{
	/* Instrucao print. */
	fprintf( out, "%3d: Instrucao PRINT: \"%s\"\n", yylineno, yytext);
	return PRINT;
}

scan	{
	/* Instrucao scan. */
	fprintf( out, "%3d: Instrucao SCAN: \"%s\"\n", yylineno, yytext);
	return SCAN;
}

read	{
	/* Instrucao read. */
	fprintf( out, "%3d: Instrucao READ: \"%s\"\n", yylineno, yytext);
	return READ;
}

write	{
	/* Instrucao write. */
	fprintf( out, "%3d: Instrucao WRITE: \"%s\"\n", yylineno, yytext);
	return WRITE;
}

{id}	{
	/* Identificador de variavel. */
	fprintf( out, "%3d: Identificador: \"%s\"\n", yylineno, yytext);
	yylval.sval = ( char* )malloc( sizeof( char ) * ( strlen( yytext ) + 1 ) );
	strcpy( yylval.sval, yytext );
	return ID;
}

{inteiro}	{
	/* Numero inteiro literal. */
	fprintf( out, "%3d: Inteiro: \"%s\"\n", yylineno, yytext);
	/* yylval.ival = atoi( yytext ); */
	yylval.sval = ( char* )malloc( sizeof( char ) * ( strlen( yytext ) + 1 ) );
	strcpy( yylval.sval, yytext );
	return INTEIRO;
}

[\+\-\*\/\(\)\{\}\!\=\<\>\,\;\:]	{
	/* Caracter especial. */
	fprintf( out, "%3d: Caracter especial: \"%c\"\n", yylineno, *yytext);
	return *yytext;
}

{espaco}	{
	/* Espaco, tab (horiz. ou vert.) , nova linha */
	;
}

"/*"	{
	/* Ignora comentarios. */
	char m,n;
	m = input();
	n = input();
	while( printf("[m,n] = [%c,%c]\n",m,n), ( m != '*' ) || ( n != '/') ) {
		m = n;
		n = input();
	}
}

\"[^"]*\"	{ 

	/* falta verificar aspas "escapadas" que podem estragar a compilacao do codigo C !!! */
	int len;
	/* Recolhe string literal, eliminando logo as aspas delimitadoras. */
	fprintf( out, "%3d: Texto literal: \"%s\"\n", yylineno, yytext);
	len = strlen( yytext );
	yylval.sval = ( char* )malloc( sizeof( char ) * ( len - 1 ) );
	strncpy( yylval.sval, yytext+1, len - 2 );
	yylval.sval[ len - 2 ] = '\0';
	return TEXTO;
}

[.]+	{ 
	/* Erro: palavra desconhecida. */
	fprintf( out, "%3d: Erro: palavra desconhecida: \"%s\"\n", yylineno, yytext);
}

.	{ 
	/* Erro: caracter desconhecido. */
	fprintf( out, "%3d: Erro: caracter desconhecido: \"%s\"\n", yylineno, yytext);
}

%%


