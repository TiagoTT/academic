#
#	Makefile
#

CC=gcc
CFLAGS=-Wall -ansi -pedantic -g

SIMULADOR=Simulador
GRAPHICS=Graphics.sh
OBJECTS=simulador.o defrr.o fifo.o weigfq.o weigrr.o queue.o
DEFS=simulador.h queue.h ajustes.h
TESTDIR=experiencia
RESULTDIR=resultados
STAT=$(RESULTDIR)/estatisticas.txt
RESULTADOS=$(RESULTDIR)/resultados.txt

#
#	Compila, corre, gera e mostra os graficos
#
all: help

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
	$(CC) $(CFLAGS) $^ -o $@

simulador.o: simulador.c $(DEFS)
	$(CC) $(CFLAGS) -c $<
 
defrr.o: defrr.c $(DEFS)
	$(CC) $(CFLAGS) -c $<
 
fifo.o: fifo.c $(DEFS) 
	$(CC) $(CFLAGS) -c $<
 
weigfq.o: weigfq.c $(DEFS)
	$(CC) $(CFLAGS) -c $<
 
weigrr.o: weigrr.c $(DEFS)
	$(CC) $(CFLAGS) -c $<
 
queue.o: queue.c $(DEFS)
	$(CC) $(CFLAGS) -c $<

#######################################
#	Simulacao
#######################################
sim: fifo wrr drr wfq
	mv $(STAT) $(RESULTADOS)

fifo: bin
	mkdir -p $(RESULTDIR) && \
	echo -e "\n\n*** Disciplina FIFO ***\n" >> $(STAT) && \
	cd $(TESTDIR) && \
	echo -e "\nExperiencia 1\n" >> ../$(STAT) && \
	../$(SIMULADOR) 1 1 >> ../$(STAT) && \
	echo -e "\nExperiencia 2\n" >> ../$(STAT) && \
	../$(SIMULADOR) 2 1 >> ../$(STAT) && \
	cd .. \
	&& ./$(GRAPHICS) $@ 1 \
	&& ./$(GRAPHICS) $@ 2

wrr: bin
	mkdir -p $(RESULTDIR) && \
	echo -e "\n\n*** Disciplina WRR ***\n" >> $(STAT) && \
	cd $(TESTDIR) && \
	echo -e "\nExperiencia 1\n" >> ../$(STAT) && \
	../$(SIMULADOR) 1 2 >> ../$(STAT) && \
	echo -e "\nExperiencia 2\n" >> ../$(STAT) && \
	../$(SIMULADOR) 2 2 >> ../$(STAT) && \
	cd .. \
	&& ./$(GRAPHICS) $@ 1 \
	&& ./$(GRAPHICS) $@ 2

drr: bin
	mkdir -p $(RESULTDIR) && \
	echo -e "\n\n*** Disciplina DRR ***\n" >> $(STAT) && \
	cd $(TESTDIR) && \
	echo -e "\nExperiencia 1\n" >> ../$(STAT) && \
	../$(SIMULADOR) 1 3 >> ../$(STAT) && \
	echo -e "\nExperiencia 2\n" >> ../$(STAT) && \
	../$(SIMULADOR) 2 3 >> ../$(STAT) && \
	cd .. \
	&& ./$(GRAPHICS) $@ 1 \
	&& ./$(GRAPHICS) $@ 2

wfq: bin
	mkdir -p $(RESULTDIR) && \
	echo -e "\n\n*** Disciplina WFQ ***\n" >> $(STAT) && \
	cd $(TESTDIR) && \
	echo -e "\nExperiencia 1\n" >> ../$(STAT) && \
	../$(SIMULADOR) 1 4 >> ../$(STAT) && \
	echo -e "\nExperiencia 2\n" >> ../$(STAT) && \
	../$(SIMULADOR) 2 4 >> ../$(STAT) && \
	cd .. \
	&& ./$(GRAPHICS) $@ 1 \
	&& ./$(GRAPHICS) $@ 2


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
