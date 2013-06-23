CC=gcc
CFLAGS=-O3 -Wall -ansi -pedantic -g
#CFLAGS=-Wall -ansi -pedantic -g

all: simulador

FICHEIROS=main.o cir.o ent.o sim.o fun.o sai.o io.o

simulador: ${FICHEIROS}
	${CC} ${CFLAGS} ${FICHEIROS} -o simulador

clean:
	rm -f *.o *~ simulador
