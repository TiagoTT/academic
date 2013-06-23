%{
/* Reconhecimento de instrucoes. */
#include<stdio.h>
#include<stdlib.h>

#include"yacc_util.h"
#include"stack.h"
#include"tabelas.h"

/* Estas variaveis sao definidas pelo lex. */
extern int yylineno;
extern FILE* yyin;/* ainda nao e utilizado*/

FILE* out = NULL;
FILE* ficheC = NULL;
unsigned char ident = 0;
unsigned int nprocess = 0;
Item* procreate_stack = NULL;
Item* tabela = NULL;

/* Funcoes */
int yyerror( char* error );
int yylex(void);

int erro_id_nao_declarado( char* msg );
int erro_tipo_errado( char* msg );
%}

%token INT SEMAPHORE CHANNEL SIZE LIST CONST OF BOUNDED BY
%token IF ELSE WHILE
%token PROCREATE REQUEST RELEASE PRINT SCAN READ WRITE

%token IGUAL DIFERENTE MAIOR_IGUAL MENOR_IGUAL AND OR

%union{
	int ival;	/* inteiro */
	char* sval;	/* string */
}
%token <sval> INTEIRO
%token <sval> ID TEXTO

/* definicao de prioridade*/
%left '-' '+'
%left '*' '/'
%nonassoc UMINUS

%left OR
%left AND
%nonassoc '!'

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
/******/

%type <ival> INSTRUCAO_SIMPLES INSTRUCAO_IF INSTRUCAO_ELSE INSTRUCAO_IF_ELSE INSTRUCAO_WHILE INSTRUCAO_PROCREATE 
%type <ival> INSTRUCAO INSTRUCOES DECLARACAO DECLARACOES LISTA_FUNCOES AC FC
%type <sval> OPERANDO OPERANDO_BOOL OPERANDO_REL EXPRESSAO_ARITMETICA EXPRESSAO_BOOLEANA
%type <sval> EXPRESSAO_RELACIONAL CONDICAO CABECALHO_FUNCAO FUNCAO

%%

PROGRAMA:
CODIGO	{
	/* Producao final. */
	fprintf( out, "PROGRAMA <- CODIGO (Producao final)\n" );
	exit(0);
}
;

CODIGO:
FUNCAO	{
	/* Funcao main. */
					/* Falta verificar se a funcao se chama main. */
	fprintf( out, "CODIGO <- FUNCAO\n" );
        fprintf( ficheC, "\n/* Fim do codigo traduzido. */\n"\
			"int main() {\n"\
			"\tFUNCAO_%s(NULL);\n"\
			"\treturn 0;\n"\
			"}\n"\
			"/* Compilacao: gcc -Wall -pedantic Traducao_SPL_C.c fifo.c -lpthread */\n"
			"/* EOF */\n", $1 );
	free( $1 );
}

FUNCAO:
CABECALHO_FUNCAO CORPO_FUNCAO	{ 
	fprintf( out, "FUNCAO <-CABECALHO_FUNCAO BLOCO_INSTRUCOES %s\n", $1);
	identa( ficheC, ident );
	fprintf( ficheC, "/* Fim do corpo da funcao %s. */\n", $1);
	$$ = $1;
}
;

CABECALHO_FUNCAO:
ID ':' ':'	{
	fprintf( out, "CABECALHO_FUNCAO <-ID_FUNCAO  ;\n");
	identa( ficheC, ident );
	fprintf( ficheC, "void* FUNCAO_%s( void * arg ) /* from line %d*/\n", $1, yylineno );
	$$ = $1;
}
;

BLOCO_INSTRUCOES:
CORPO_FUNCAO	{ fprintf( out, "BLOCO_INSTRUCOES <- CORPO_FUNCAO\n" ); }
| INSTRUCAO	{ fprintf( out, "BLOCO_INSTRUCOES <- INSTRUCAO\n" ); }
;

CORPO_FUNCAO:
AC DECLARACOES FC	{ fprintf( out, "CORPO_FUCAO <- { DECLARACOES }\n" );}
| AC DECLARACOES INSTRUCOES FC     { fprintf( out, "CORPO_FUCAO <- { DECLARACOES INSTRUCOES }\n" );}
| AC INSTRUCOES FC     { fprintf( out, "CORPO_FUCAO <- { INSTRUCOES }\n" );}
;

AC:
'{'	{
	/* Abertura de um bloco de instrucoes. */
	identa( ficheC, ident ++ );
	fprintf( ficheC, "{\n" );
	table_push( &tabela );
}
;

FC:
'}'	{
	/* Fecho de um bloco de instrucoes. */
							/* Pode ser necessario incluir codigo de limpeza aqui!!! */
	identa( ficheC, -- ident );
	fprintf( ficheC, "}\n" );
	table_pop( &tabela );
}
;

DECLARACOES:
DECLARACAO	{ fprintf( out, "DECLARACOES <- DECLARACAO\n" ); }
| DECLARACOES DECLARACAO { fprintf( out, "DECLARACOES <- DECLARACOES DECLARACAO\n" ); }
;

DECLARACAO:
INT ID ';'	{
	fprintf( out, "INSTRUCAO_DECLARACAO <- INT (%s)\n", $2 );
	identa( ficheC, ident );
	fprintf( ficheC, "int %s; /* from line %d */\n", $2, yylineno );
	table_insert( tabela, NovoIdentificador( $2, inteiro ) );
}
| SEMAPHORE ID ';'	{
	fprintf( out, "INSTRUCAO_DECLARACAO <- SEMAPHORE (%s)\n", $2 ); 
	identa( ficheC, ident );
	fprintf( ficheC, "sem_t %s ; /* from line %d */\n", $2, yylineno );
	table_insert( tabela, NovoIdentificador( $2, semaforo ) );
	/* E' necessario inicializar o semaforo apos as declaracoes e remove-lo no final do bloco de instrucoes. */
}
| CHANNEL OF INT SIZE BOUNDED BY INTEIRO ID ';'	{
	fprintf( out, "INSTRUCAO_DECLARACAO <- CHANEL (%s) SIZE (%s)\n", $8, $7 );
	identa( ficheC, ident );
	fprintf( ficheC, "bf %s = IniciaBuffer( %s ); /* from line %d */\n", $8, $7, yylineno );
	table_insert( tabela, NovoIdentificador( $8, canal_limitado ) );
	free( $7 );
}
| CHANNEL OF INT ID ';'	{
	fprintf( out, "INSTRUCAO_DECLARACAO <- CHANNEL (%s)\n", $4 ); 
	identa( ficheC, ident );
	fprintf( ficheC, "channel_of_int %s = fifo_list_init(); /* from line %d */\n", $4, yylineno );
	table_insert( tabela, NovoIdentificador( $4, canal_ilimitado ) );
}
| LIST OF INT ID ';'	{
	fprintf( out, "INSTRUCAO_DECLARACAO <- LIST (%s)\n", $4 ); 
	identa( ficheC, ident );
	fprintf( ficheC, "list_of_int %s = fifo_list_init(); /* from line %d */\n", $4, yylineno );
	table_insert( tabela, NovoIdentificador( $4, lista ) );
}
| CONST INT ID '=' INTEIRO ';'	{
	fprintf( out, "INSTRUCAO_DECLARACAO <- CONST (%s=%s)\n", $3, $5); 
	identa( ficheC, ident );
	fprintf( ficheC, "const int %s = %s ; /* from line %d */\n", $3, $5, yylineno );
	table_insert( tabela, NovoIdentificador( $3, constante ) );
	free( $5 );
}
;

INSTRUCOES:
INSTRUCAO	{ fprintf( out, "INSTRUCOES <- INSTRUCAO\n" ); 
}
| INSTRUCOES INSTRUCAO	{ fprintf( out, "INSTRUCOES <- INSTRUCOES INSTRUCAO\n" ); 
}
;

INSTRUCAO:
INSTRUCAO_SIMPLES	{ fprintf( out, "INSTRUCAO <- INSTRUCAO_SIMPLES\n" ); 
}
| INSTRUCAO_IF_ELSE	{ fprintf( out, "INSTRUCAO <- INSTRUCAO_IF_ELSE\n" ); 
}
| INSTRUCAO_WHILE	{ fprintf( out, "INSTRUCAO <- INSTRUCAO_WHILE\n" );
}
| INSTRUCAO_PROCREATE	{ fprintf( out, "INSTRUCAO <- INSTRUCAO_PROCREATE\n" );
}
| ';'	{ 
	fprintf( out, "INSTRUCAO <- (vazio)\n" );
	identa( ficheC, ident );	
	fprintf( ficheC, ";\n" );
}
;

INSTRUCAO_SIMPLES:
ID '=' EXPRESSAO_ARITMETICA ';'	{
	identificador* id;
	fprintf( out, "INSTRUCAO_SIMPLES <- %s = %s ;\n", $1, $3 );
	identa( ficheC, ident );
	fprintf( ficheC, "%s = %s ; /* from line %d*/\n", $1, $3, yylineno );
	if( NULL == ( id = table_find( tabela, $1 ) ) ) {
		erro_id_nao_declarado( $1 );
	}
	if( inteiro != id -> t ) {
		erro_tipo_errado( $1 );
	}
	free( $1 ); free( $3 );
}
| REQUEST '(' ID ')' ';'	{
	identificador* id;
	fprintf( out, "INSTRUCAO_SIMPLES <- REQUEST (%s)\n", $3 );
	identa( ficheC, ident );
	fprintf( ficheC, "sem_wait( &%s ); /* from line %d*/\n", $3, yylineno );
	if( NULL == ( id = table_find( tabela, $3 ) ) ) {
		erro_id_nao_declarado( $3 );
	}
	if( semaforo != id -> t ) {
		erro_tipo_errado( $3 );
	}
	free( $3 );
}
| RELEASE '(' ID ')' ';'	{
	identificador* id;
	fprintf( out, "INSTRUCAO_SIMPLES <- RELEASE (%s)\n", $3 );
	identa( ficheC, ident );
	fprintf( ficheC, "sem_post( &%s ); /* from line %d*/\n", $3, yylineno );
	if( NULL == ( id = table_find( tabela, $3 ) ) ) {
		erro_id_nao_declarado( $3 );
	}
	if( semaforo != id -> t ) {
		erro_tipo_errado( $3 );
	}
	free( $3 );
}
| PRINT '(' ID ')' ';'	{
	fprintf( out, "INSTRUCAO_SIMPLES <- PRINT ( ID %s )\n", $3 );
	identa( ficheC, ident );
	fprintf( ficheC, "printf( \"%%d\", %s ); /* from line %d*/\n", $3, yylineno );
	free( $3 );
}
| PRINT '(' TEXTO ',' ID ')' ';'	{
	fprintf( out, "INSTRUCAO_SIMPLES <- PRINT (TEXTO %s ID %s)\n", $3, $5 );
	identa( ficheC, ident );
	fprintf( ficheC, "printf( \"%s%%d\", %s ); /* from line %d*/\n", $3, $5, yylineno );
	free( $3 ); free( $5 );
}
| SCAN '(' ID ')' ';'	{ fprintf( out, "INSTRUCAO_SIMPLES <- SCAN (%s)\n", $3 ); 
	identa( ficheC, ident );
	fprintf( ficheC, "scanf( \"%%d\", &%s ); /* from line %d*/\n", $3, yylineno );
	free( $3 );
}
| SCAN '(' TEXTO ',' ID ')' ';'	{
	fprintf( out, "INSTRUCAO_SIMPLES <- SCAN (TEXTO %s ID %s)\n", $3, $5 );
	identa( ficheC, ident );
	fprintf( ficheC, "printf( \"%s\" ); scanf( \"%%d\", &%s ); /* from line %d*/\n", $3, $5, yylineno );
	free( $3 ); free( $5 );
}
| READ '(' ID ',' ID ')' ';'	{
	fprintf( out, "INSTRUCAO_SIMPLES <- READ (%s,%s)\n", $3, $5 ); 
	identa( ficheC, ident );
	/* Se for um canal limitado. */
	fprintf( ficheC, "%s = RemoveDoInicio( %s );\n", $5, $3 );
}
| WRITE '(' ID ',' ID ')' ';'	{
	fprintf( out, "INSTRUCAO_SIMPLES <- WRITE (%s,%s)\n", $3, $5 ); 
	identa( ficheC, ident );
	/* Se for um canal limitado. */
	fprintf( ficheC, "ColocaNoFim( %s, %s );\n", $3, $5 );
}
;

INSTRUCAO_IF_ELSE:
INSTRUCAO_IF BLOCO_INSTRUCOES %prec LOWER_THAN_ELSE	{
	fprintf( out, "INSTRUCAO_IF <- INSTRUCAO_IF BLOCO_INSTRUCOES\n" );
}
|INSTRUCAO_IF BLOCO_INSTRUCOES INSTRUCAO_ELSE BLOCO_INSTRUCOES	{
	fprintf( out, "INSTRUCAO_IF <- INSTRUCAO_IF BLOCO_INSTRUCOES INSTRUCAO_ELSE BLOCO_INSTRUCOES\n" );
}
;

INSTRUCAO_IF:
IF '(' CONDICAO ')'	{
	identa( ficheC, ident );
	fprintf( ficheC, "if( %s ) /* from line %d*/\n", $3, yylineno );
	free( $3 );
}
;

INSTRUCAO_ELSE:
ELSE 	{
	identa( ficheC, ident );
	fprintf( ficheC, "else /* from line %d*/\n", yylineno );
}
;

INSTRUCAO_WHILE:
INSTRUCAO_WHILE_INICIO BLOCO_INSTRUCOES	{
	fprintf( out, "INSTRUCAO_WHILE <- INSTRUCAO_WHILE_INICIO BLOCO_INSTRUCOES\n" );
}
;

INSTRUCAO_WHILE_INICIO:
WHILE '(' CONDICAO ')' 	{
	fprintf( out, "INSTRUCAO_WHILE <- WHILE(%s)\n", $3 );
	identa( ficheC, ident );
	fprintf( ficheC, "while( %s )\n", $3 );
	free( $3 );
}
;

INSTRUCAO_PROCREATE:
PROCREATE '(' LISTA_FUNCOES ')' ';'	{
	int i;
	char* s;
	fprintf( out, "INSTRUCAO_PROCREATE <- PROCREATE();\n" );

	/* Abertura do bloco. */
	identa( ficheC, ident++ );
	fprintf( ficheC, "{\n" );

	identa( ficheC, ident );
	fprintf( ficheC, "pthread_t threads[ %d ];\n", nprocess );
	
	for( i = 0 ; i < nprocess ; i++ ) {
		s = (char*)stack_pop( &procreate_stack );
		identa( ficheC, ident );
		fprintf( ficheC, "pthread_create( &threads[ %d ], NULL, FUNCAO_%s, NULL );\n", i, s );
		free( s );
	}
	
	for( i = 0 ; i < nprocess ; i++ ) {
		identa( ficheC, ident );
		fprintf( ficheC, "pthread_join( threads[ %d ], NULL );\n", i );
	}

	/* Fecho do bloco. */
	identa( ficheC, --ident );
	fprintf( ficheC, "}\n" );
		
	/* Limpeza. */
	nprocess = 0;
}
;

LISTA_FUNCOES:
FUNCAO	{
	fprintf( out, "LISTA_FUNCOES <- FUNCAO (%s)\n", $1 );
	stack_push( &procreate_stack, (void*)$1 );
	nprocess ++;
}
| LISTA_FUNCOES ',' FUNCAO	{
	fprintf( out, "LISTA_FUNCOES <- LISTA_FUNCOES () FUNCAO (%s)\n", $3 );
	stack_push( &procreate_stack, (void*)$3 );
	nprocess ++;
}
;

CONDICAO:
EXPRESSAO_BOOLEANA	{
	fprintf( out, "CONDICAO<-expressao booleana\n");
}
;

EXPRESSAO_ARITMETICA:
EXPRESSAO_ARITMETICA '+' EXPRESSAO_ARITMETICA	{
	char* aux;
	printf( "expressao aritmetica soma\n");
	aux = cola( $1, "+" );
	$$ = cola( aux, $3 );
	free( $1 ); free( $3 ); free( aux );
}
| EXPRESSAO_ARITMETICA '-' EXPRESSAO_ARITMETICA {
	char* aux;
	printf( "expressao aritmetica sub\n");
	aux = cola( $1, "-" );
	$$ = cola( aux, $3 );
	free( $1 ); free( $3 ); free( aux );
}
| EXPRESSAO_ARITMETICA '*' EXPRESSAO_ARITMETICA {
	char* aux;
	printf( "expressao aritmetica multip\n");
	aux = cola( $1, "*" );
	$$ = cola( aux, $3 );
	free( $1 ); free( $3 ); free( aux );
}
| EXPRESSAO_ARITMETICA '/' EXPRESSAO_ARITMETICA {
	char* aux;
	printf( "expressao aritmetica divisao\n");
	aux = cola( $1, "/" );
	$$ = cola( aux, $3 );
	free( $1 ); free( $3 ); free( aux );
}
| '-' EXPRESSAO_ARITMETICA %prec UMINUS  {
	printf( "expressao aritmetica simetrico\n");
	$$ = cola( "-", $2 );
	free( $2 );
}
| '+' EXPRESSAO_ARITMETICA %prec UMINUS  {
	printf( "expressao aritmetica positivo\n");
	$$ = cola( "+", $2 );
	free( $2 );
}
| '(' EXPRESSAO_ARITMETICA ')'  {
	char* aux;
	printf( "(expressao aritmetica)\n");
	aux = cola( "(", $2 );
	$$ = cola( aux, ")" );
	free( $2 ); free( aux );
}
| OPERANDO	{
	printf( "expressa aritmetica <- operando\n");
}
;

OPERANDO: ID 	{ printf( "OPERANDO<-ID\n");}
| INTEIRO	{ printf( "OPERANDO<-INTEIRO\n");}
;

EXPRESSAO_BOOLEANA:
EXPRESSAO_BOOLEANA  OR  EXPRESSAO_BOOLEANA	{
	char* aux;
	printf( "expressao booleana OR\n");
	aux = cola( $1, "||" );
	$$ = cola( aux, $3 );
	free( $1 );free( $3 );free( aux );
}
| EXPRESSAO_BOOLEANA AND  EXPRESSAO_BOOLEANA	{
	char* aux;
	printf( "expressao  booleana AND\n");
	aux = cola( $1, "&&" );
	$$ = cola( aux, $3 );
	free( $1 );free( $3 );free( aux );
}
| '!' EXPRESSAO_BOOLEANA	{
	printf( "expressao  booleana NEGACAO\n");
	$$ = cola( "!", $2 );
	free( $2 );
}
| '(' EXPRESSAO_BOOLEANA ')'	{
	char* aux;
	printf( "(expressao booleana)\n");
	aux = cola( "(", $2 );
	$$ = cola( aux, ")" );
	free( $2 ); free( aux );
}
| OPERANDO_BOOL	{
	printf( "operando booleano\n");
	$$ = $1;
}
;

OPERANDO_BOOL:
EXPRESSAO_RELACIONAL	{
	printf( "OPERANDO_BOOL<-EXPRESSAO_RELACIONAL\n");
	$$ = $1;
}
; 

EXPRESSAO_RELACIONAL:
OPERANDO_REL '>' OPERANDO_REL	{
	char* aux;
	printf( "expressao relacional maior\n");
	aux = cola( $1, ">" );
	$$ = cola( aux, $3 );
	free( $1 );free( $3 );free( aux );
}
| OPERANDO_REL '<' OPERANDO_REL	{
	char* aux;
	printf( "expressao relacional menor\n");
	aux = cola( $1, "<" );
	$$ = cola( aux, $3 );
	free( $1 );free( $3 );free( aux );
}
| OPERANDO_REL IGUAL OPERANDO_REL	{
	char* aux;
	printf( "expressao relacional igual\n");
	aux = cola( $1, "==" );
	$$ = cola( aux, $3 );
	free( $1 );free( $3 );free( aux );
}
| OPERANDO_REL MENOR_IGUAL OPERANDO_REL	{
	char* aux;
	printf( "expressao relacional menor_igual\n");
	aux = cola( $1, "<=" );
	$$ = cola( aux, $3 );
	free( $1 );free( $3 );free( aux );
}
| OPERANDO_REL MAIOR_IGUAL OPERANDO_REL	{
	char* aux;
	printf( "expressao relacional maior_igual\n");
	aux = cola( $1, ">=" );
	$$ = cola( aux, $3 );
	free( $1 );free( $3 );free( aux );
}
| OPERANDO_REL DIFERENTE OPERANDO_REL	{
	char* aux;
	printf( "expressao relacional diferente\n");
	aux = cola( $1, "!=" );
	$$ = cola( aux, $3 );
	free( $1 );free( $3 );free( aux );
}
;

OPERANDO_REL:
EXPRESSAO_ARITMETICA	{
	printf( "OPERANDO_REL<- EXPRESSAO_ARITMETICA\n");
	$$ = $1;
}
;

%%

int yyerror( char* msg ) {
	printf("%3d: Erro: %s\n", yylineno, msg );
	return 0;
}

int erro_id_nao_declarado( char* msg ) {
	printf("%3d: Erro: Variavel nao declarada: %s\n", yylineno, msg );
	return 0;
}

int erro_tipo_errado( char* msg ) {
	printf("%3d: Erro: Tipo errado da variavel: %s\n", yylineno, msg );
	return 0;
}

int main( int argc, char* argv[], char* env[] ) {
	out = stdout;
	
	ficheC = fopen("Traducao_SPL_C.c", "w");

	if( ficheC == NULL){
		fprintf(stderr, "unsuccessful completion of function fopen .\n");
		exit(-1);
	}	
	
	fputs( "/*\n"\
		" * Ficheiro: Traducao_SPL_C.c\n"\
		" *\n"\
		" * Ficheiro de codigo C gerado pelo transcompilador de SPL.\n"\
		" */\n\n"\
		"#include<stdio.h>\n"\
		"#include<stdlib.h>\n"\
		"#include<pthread.h>\n"\
		"#include<semaphore.h>\n\n"\
		"#include\"fifo.h\"\n\n"\
		"/* Inicio do codigo traduzido. */\n\n", ficheC );

	yyparse();

	fclose( ficheC );

	exit(0);
}

