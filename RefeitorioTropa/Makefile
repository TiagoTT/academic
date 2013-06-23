CC=gcc
#CFLAGS= -Wall -ansi -pedantic -g # A flag "-ansi" bloqueia algumas macros... /usr/include/features.h
CFLAGS= -Wall -pedantic -pthread -g

all: refeitorio soldado oficial general

refeitorio: sock_stream.o sock_dgram.o mailbox.o \
	sincronizacao.o ref_sol.o ref_ofi.o ref_gen.o\
	 stock.o registo.o memoria.o menu_ref.o signaln.o

soldado: sock_stream.o sincronizacao.o memoria.o menu_sol.o signaln.o

oficial: sock_dgram.o sincronizacao.o signaln.o cmd.o signaln.o

general: mailbox.o sincronizacao.o menu_gen.o cmd.o signaln.o

clean: 
	rm -f *.o *~ refeitorio soldado oficial general
	cp stock.txt.bak stock.txt
	cp menus.txt.bak menus.txt
