/***********************************************************************
 *Processo: Oficial Refeitorio Soldado General
 *Ficheiro:signal.c
 *Data: Dezembro 2005
 ***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>


#include "signaln.h"

void INTOrd_signal(void (*handler)(int))
{

  /*
   * Macro: int SIGINT
   * The `SIGINT' ("program interrupt") signal is sent when the user
   * types the INTR character (normally `C-c').  *Note Special
   * Characters::, for information about terminal driver support for
   * `C-c'
   */
  signal(SIGINT,handler);
  /*
   *Macro: int SIGTERM
   *The `SIGTERM' signal is a generic signal used to cause program
   *termination.  Unlike `SIGKILL', this signal can be blocked,
   *handled, and ignored.  It is the normal way to politely ask a
   *program to terminate.
   *The shell command `kill' generates `SIGTERM' by default.
   */
  signal(SIGTERM,handler);
  /*
  *Macro: int SIGUSR1
  *Macro: int SIGUSR2
  *The `SIGUSR1' and `SIGUSR2' signals are set aside for you to use
  *any way you want.  They're useful for simple interprocess
  *communication, if you write a signal handler for them in the
  *program that receives the signal.
  */
  signal(SIGUSR1, handler);
  signal(SIGUSR2, handler);

}


void Terminar_Militar(int signum)
{
  /*libertar as variaveis, fechar funcoes */
  /* printf("%s: Libertação de variáveis\n");*/
  printf(" Terminação normal do processo \n");
  getchar();
  exit(0);   /* Terminação normal do processo */
}

