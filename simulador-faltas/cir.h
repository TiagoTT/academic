/*
 * Ficheiro: cir.h
 */

#ifndef _cir_h
#define _cir_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "io.h"

/* Estrutura que suporta um no', seja ele uma porta logica ou um fio. */
#define MAX_NOME 16 /* ATENCAO (MAX_NOME-1) esta' "hardcoded" em 
		     * --> cir.c:66,78
		     * dentro duma format string da funcao sscanf.
		     * A alteracao deste #define deve ser propagada!
		     */

/* Valores que representam as funcoes logicas. */
#define AND  0
#define NAND 1
#define OR   2
#define NOR  3
#define XOR  4
#define XNOR 5
/* NOT = NOR de 1 entrada */
/* BUFF = OR de 1 entrada */

typedef struct NodeStr{
  /* Para simulacao logica e de faltas. */
  char nome[MAX_NOME]; /* Nome do no' tal qual estiver no ISCAS. */
  int  tipo;           /* Tipo do no' traduzido para um numero. */
  int  fanin;          /* Numero de inputs. */
  int* inputs;         /* Endereco (indice no vector) desses inputs. */
  /* int* outputs; */  /* Endereco (indice no vector) desses outputs. */
  int valor;           /* Valor logico do no'. */
  char flag;           /* Indica a validade do no'. Pode haver posicoes do
			* vectorNodes que ficam vazias.
			* flag = '\0'; se o Node for invalido (inicializacao)
			* flag = 'i';  se o Node for um input primario
			* flag = 'o';  se o Node for um output primario
			* flag = 't';  se o Node for um input e output primario
			* flag = 'n';  nos outros casos (nodes)
			*/
  
  /* Apenas para simulacao de faltas. */
  int   fanout;         /* Numero de outputs. */
  char* sa0;
  char* sa1; /* Vectores de faltas que se manifestam no no'. 
			    * Sao usados dois vectores para facilitar as 
			    * operacoes entre conjuntos. 
			    * Desta forma podemos utilizar |, & e ~.
			    * So e' usado o bit de menor peso.
			    */
  int   leituras;       /* Numero de vezes que o vector de faltas foi 
			* consultado. Relevante para libertar a memoria
			* ocupada pelo vector quando este ja nao for necessario.
			*/
}Node;


/* Estrutura que suporta o circuito. */
#define INIT_VEC 20000
#define GROW_VEC 2000

typedef struct CircuitoStr{
  int   nNodes;        /* Endereco mais alto de todos os no's do ficheiro ISCAS. */
  Node* vectorNodes;   /* Vector de nodes que suporta o circuito. */
  int   nMax;          /* Dimensao do vector. */
  int   nInputs;       /* Numero de inputs primarios. */
  int   realNodes;     /* Numero de no's que realmente existe. */
}Circuito;


Circuito* leCircuito(char* ficheiro);

Circuito* criaCircuito();
void maisNodes(Circuito* circuito);
void limpaCircuito(Circuito* circuito);
int traduzTipo(char* tipo);
int indiceDoNome(Circuito* circuito, char* nome);

#endif
