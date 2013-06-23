/*
 * Ficheiro: main.c
 */

#include "main.h"


int main(int argc, char* argv[])
{

  /* Estrutura que suporta o circuito. */
  Circuito* circuito;
  
  printf("Simulador Logico: INICIO \n");
  
  /* Leitura do circuito de um ficheiro.cir . */
  circuito = leCircuito( argv[1] );
	
  /* Leitura das entradas do stdin. */
  leEntradas( circuito, argv[2] );
  
  /* Simulacao logica. */
  simula( circuito );
  
  /* Deducao de faltas. */
  deduz( circuito );
  
  /* Apresentacao das saidas no stdout. */
  escreveSaidas( circuito, argv[3] );
  
  /* Destroi os dados e liberta a memoria ocupada pelas estrututas. */
  limpaCircuito( circuito );

  printf("Simulador Logico: FIM \n");
  
  exit(0);
}
