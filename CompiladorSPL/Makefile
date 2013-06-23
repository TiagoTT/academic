# 
# Makefile
# 

CC=gcc
CFLAGS=-Wall -pedantic -g
LIBS=-ly -lfl
LEX=lex -v
YACC=yacc -dv
LEX_CFILE=lex.yy.c
YACC_CFILE=y.tab.c
YACC_HFILE=y.tab.h
YACC_OUTPUT=y.output
MODULES=yacc_util.o hash.o stack.o tabelas.o
HEADERS=yacc_util.h hash.h stack.h tabelas.h
TESTDIR=teste

all: compilador
	mv $< $(TESTDIR)

compilador: lexico gramatica $(MODULES) $(HEADERS)
	$(CC) $(CFLAGS) $(LEX_CFILE) $(YACC_CFILE) $(MODULES) -o $@ $(LIBS)

lexico: lexico.l gramatica
	$(LEX) $<

gramatica: gramatica.y
	$(YACC) $<

clean: clean_lex clean_yacc
	rm -f compilador *.o

clean_lex:
	rm -f $(LEX_CFILE)

clean_yacc:
	rm -f $(YACC_CFILE) $(YACC_HFILE) $(YACC_OUTPUT)

