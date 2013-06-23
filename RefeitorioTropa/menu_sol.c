/*
 * Processo Refeitorio
 *
 * Ficheiro: menu_sol.c
 */


#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"menu_sol.h"



void Escreve_Menu_Sol(void)
{

  puts("\t* ===============================================================");
  puts("\t* comando argumetos  descricao                                   ");
  puts("\t* ===============================================================");
  printf("\t* m             ver menus do turno \n");
  printf("\t* s       n     seleccionar um dos menus (n) \n");
  printf("\t* q             Terminar a refeicao e sair (logo que possivel)\n");
  puts("\t* ================================================================");
  putchar('\n');
}


