#
#	Makefile
#

#CC=gcc-4.1.1
CC=gcc
CFLAGS=-Wall -ansi -pedantic -g

SIMULADOR=Simulador
GRAPHICS=Graphics.sh
OBJECTS=simulador.o fifo.o lcgrand.o queue.o
DEFS=simulador.h queue.h lcgrand.h
TESTDIR=experiencia
RESULTDIR=resultados
STAT=$(RESULTDIR)/estatisticas.txt
RESULTADOS=$(RESULTDIR)/resultados.txt

#
#	Compila, corre, gera e mostra os graficos
#
all: bin

#######################################
#	Manual, Instrucoes de uso
#######################################
help:
	less README

#######################################
#	Compilacao
#######################################
bin: $(SIMULADOR)
	
# cp  $^ $(TESTDIR)

$(SIMULADOR): $(OBJECTS) 
	$(CC) $(CFLAGS) $^ -lm -o $@

simulador.o: simulador.c $(DEFS)
	$(CC) $(CFLAGS) -c $<
 
fifo.o: fifo.c $(DEFS) 
	$(CC) $(CFLAGS) -c $<
 
lcgrand: lcgrand.c $(DEFS)
	$(CC) $(CFLAGS) -lm -c $<

queue.o: queue.c $(DEFS)
	$(CC) $(CFLAGS) -c $<

#######################################
#	Simulacao
#######################################
sim: bin
#	mv $(STAT) $(RESULTADOS)
	./$(SIMULADOR) > res.csv
#	mkdir -p $(RESULTDIR) && \
	echo -e "\n\n*** Canal ALOHA Sincronizado ***\n" >> $(STAT) && \
	cd $(TESTDIR) && \
	echo -e "\nExperiencia 1\n" >> ../$(STAT) && \
	../$(SIMULADOR) 1 1 >> ../$(STAT) && \
	cd .. \
	&& ./$(GRAPHICS) $@ 1 \

#######################################
#	Limpeza
#######################################
clean:
	rm -f *.o $(TESTDIR)/exp*

reset: clean
	rm -rf $(RESULTDIR) $(SIMULADOR)

#
#	EOF
#
