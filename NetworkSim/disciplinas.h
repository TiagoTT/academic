/*
 * disciplinas.h
 *
 * Prototipos das funcoes de processamento de eventos proprias de cada
 * disciplina de escalonamento.
 *
 */


#ifndef _DISCIPLINAS_H_
#define _DISCIPLINAS_H_

void FIFOPP( Evento* ev, pq fila );
void FIFOPC( Evento* ev, pq fila );

void DRRPP( Evento* ev, pq fila );
void DRRPC( Evento* ev, pq fila );

void WFQPP( Evento* ev, pq fila );
void WFQPC( Evento* ev, pq fila );

void WRRPP( Evento* ev, pq fila );
void WRRPC( Evento* ev, pq fila );

#endif
