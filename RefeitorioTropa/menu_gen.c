/*
 * Processo Refeitorio
 *
 * Ficheiro: menu_general.c
 */

#include"menu_gen.h"

void Escreve_Menu_General(void)
{

  puts("\t*==================================================================");
  puts("\t* comando    argumetos                 descricao                   ");
  puts("\t* =================================================================");
  printf("\t* q              Terminar a refeicao e sair (logo que possivel)\n");
  puts("\t* =================================================================");
    putchar('\n');

}
void Menu_Gen(void)
{
	char opcao;
	
	while(1)
	{
		Escreve_Menu_General();
		opcao = getchar();
		
		if( 'q' == opcao )
		  {
		    /* chamar a funcao que permite terminar a 
		       refeicao e sair logo que possivel*/
		    return;
		  }
	}
}

