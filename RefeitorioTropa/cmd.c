/*
 * Processo
 * Ficheiro:cmd.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmd.h"

char* ProcessaArgumentos(int argc, char* argv[])
{
	char* msg;
	int i;
	
	msg = (char*)malloc(sizeof(char));
        msg[0]='\0';
	
	for( i = 1 ; i < argc ; i++ )
	{
		msg = (char*)realloc( msg, sizeof(char)*
				      ( strlen(msg) + strlen(argv[i]) + 2) );
		strcat(msg, " ");
		strcat(msg, argv[i]);
	}
	
	return msg;
}
