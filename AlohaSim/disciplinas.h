/*
 * disciplinas.h
 *
 * Prototipos das funcoes de processamento de eventos proprias de cada
 * disciplina de escalonamento.
 *
 */


#ifndef _DISCIPLINAS_H_
#define _DISCIPLINAS_H_

void ProcessaChegada( Evento* ev, pq fila, double lambda );
void ProcessaPartida( Evento* ev, pq fila, Estacao** es, double p );

#endif
