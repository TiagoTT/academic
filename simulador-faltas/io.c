/*
 * Ficheiro: io.c
 */

#include "io.h"

FILE* abreFicheiro(char* nome, char* modo, char* msgErro)
{
  FILE* fp;
  
  if( NULL == ( fp = fopen( nome, modo ) ) )
    {
      perror( msgErro );
      exit( -1 );
    }
  
  return fp;
}

void fechaFicheiro(FILE* fp, char* msgErro)
{
  if( EOF == fclose( fp ) )
    {
      perror( msgErro );
      exit( -1 );
    }
  
  return;
}
