/* 
 * Processo Refeitorio Soldado
 * Ficheiro: memoria.c
 */
#include "sincronizacao.h"

#include "memoria.h"

void* CriaMemoriaPartilhada(int key, int n_bytes)
{
  void* ptr;
  int id;

  assert( ( id = shmget( key, n_bytes, 0666 | IPC_CREAT ) ) >= 0 );
  
  assert( ( ptr = shmat( id, 0, 0) ) >= (void*)0 );

  return ptr;
}

Escolha* AcedeMemoriaPedidos()
{
  int id;
  Escolha* escolhas;
  int size;
  
  size = sizeof( Escolha ) * DevolveSemaforo( PRESENTES );
  assert( ( id = shmget( SHM_PEDIDOS, size, 0666 | IPC_CREAT ) ) >= 0 );
  assert( ( escolhas = (Escolha*) shmat( id, 0, 0 ) ) >= (Escolha*) 0 );

  return escolhas;
}

Menus AcedeMemoriaMenus()
{
  int id;
  Menus menus;
  int size;

  /* Retira de Semaforos o numero de linhas e colunas. */
  menus.linhas = DevolveSemaforo( SEM_LINHAS );
  menus.colunas = DevolveSemaforo( SEM_COLUNAS );
  
  size = (menus.linhas * menus.colunas )*sizeof(char);

  assert( ( id = shmget( SHM_MENUS, size, 0 ) ) >= 0 );
  assert( ( menus.texto = (char*) shmat( id, 0, 0 ) ) >=  (char*)0 );

  return menus;
}

void EscreveMemoriaPedido(int pid, int escolha)
{
  int id;
  int indice;
  Escolha* memoria;
  int size;
  
  /* Neste momento ja sabemos quantos sao os Soldados */
  size = sizeof( Escolha ) * DevolveSemaforo( PRESENTES );

  /* O primeiro Soldado a ecrever cria o segmento de SHM */
  assert( ( id = shmget( SHM_PEDIDOS, size, 0666 | IPC_CREAT ) ) >= 0 );

  assert( ( memoria = (Escolha*) shmat( id, 0, 0 ) ) >= (Escolha*) 0 );
  
  indice = GetIndiceEscolha();
  memoria[ indice ].pid = pid;
  memoria[ indice ].menu = escolha;

  assert( shmdt( memoria ) >= 0 );
  
  return;
}

int GetIndiceEscolha()
{
  int i;

  EsperaSemaforo( MUTEX_INDICE_ESCOLHA );
  i = DevolveSemaforo( INDICE_ESCOLHA );
  AssinalaSemaforo( INDICE_ESCOLHA );
  AssinalaSemaforo( MUTEX_INDICE_ESCOLHA );
  
  return i;
}
