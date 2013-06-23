/**********************************************
 *Processo: Oficial  Refeitorio Soldado General
 *Ficheiro:signal.h
 *Data: Dezembro 2005
 **********************************************/

#ifndef _SIGNALN_H_
#define _SIGNALN_H_

#define NMilitares 200

void INTOrd_signal(void (*)(int));
void Terminar_Militar(int signum);
void Terminar_Ref(int signum);
#endif


/*Informacao so para nao tar a ir ao info*/

/*

Here is a simple example of setting up a handler to delete temporary
files when certain fatal signals happen:

     #include <signal.h>

     void
     termination_handler (int signum)
     {
       struct temp_file *p;

       for (p = temp_file_list; p; p = p->next)
         unlink (p->name);
     }

     int main (void)
     {
       ...
       if (signal (SIGINT, termination_handler) == SIG_IGN)
         signal (SIGINT, SIG_IGN);
       if (signal (SIGHUP, termination_handler) == SIG_IGN)
         signal (SIGHUP, SIG_IGN);
       if (signal (SIGTERM, termination_handler) == SIG_IGN)
         signal (SIGTERM, SIG_IGN);
       ...
*/
