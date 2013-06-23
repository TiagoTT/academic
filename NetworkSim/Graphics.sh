#!/bin/bash
#
# Shell script que recolhe os resultados do simulador
# e gera os graficos correspondentes em png utilizando o gnuplot.
#
# O primeiro parametro deve ser a disciplina e o segundo deve ser
# o numero da experiencia.
#
# Os ficheiro txt com os resultados do simulador devem estar no
# directorio TESTDIR e os graficos serao colocados no directorio
# RESULTDIR.

RESULTDIR="resultados/experiencia_$2/$1";
TESTDIR="experiencia";

# Cria o directorio de resultados correspondente
mkdir -p ${RESULTDIR};

# Graficos dos debitos conseguidos por cada sessao na saida do ultimo router.
echo "set terminal png; \
	set output \"${RESULTDIR}/DebitosExp$2.png\"; \
	set title \"Debitos medios medidos a cada segundo.\n\
	Experiencia: $2\nDisciplina: $1\"; \
	plot \
	'${TESTDIR}/exp$2ses1deb.txt' title 'Sessao 1' with lines, \
	'${TESTDIR}/exp$2ses2deb.txt' title 'Sessao 2' with lines, \
	'${TESTDIR}/exp$2ses3deb.txt' title 'Sessao 3' with lines;" \
	| gnuplot;


# Graficos das ocupacoes das filas de espera em cada router.
echo "set terminal png; \
	set output \"${RESULTDIR}/FilasExp$2-r1.png\"; \
	set title \"Ocupacao media das filas de espera medidas a cada 0.5 segundos.\n\
	Experiencia: $2 Router: 1\n Disciplina: $1\"; \
	plot '${TESTDIR}/exp$2ses1filas.txt' every 3::0 title 'Sessao:1' with lines, \
	'${TESTDIR}/exp$2ses2filas.txt' every 3::0 title 'Sessao:2' with lines, \
	'${TESTDIR}/exp$2ses3filas.txt' every 3::0 title 'Sessao:3' with lines;"\
	| gnuplot;
	
echo "set terminal png; \
	set output \"${RESULTDIR}/FilasExp$2-r2.png\"; \
	set title \"Ocupacao media das filas de espera medidas a cada 0.5 segundos.\n\
	Experiencia: $2 Router: 2\n Disciplina: $1\"; \
	plot '${TESTDIR}/exp$2ses1filas.txt' every 3::1 title 'Sessao:1' with lines, \
	'${TESTDIR}/exp$2ses2filas.txt' every 3::1 title 'Sessao:2' with lines, \
	'${TESTDIR}/exp$2ses3filas.txt' every 3::1 title 'Sessao:3' with lines;"\
	| gnuplot;
	
echo "set terminal png; \
	set output \"${RESULTDIR}/FilasExp$2-r3.png\"; \
	set title \"Ocupacao media das filas de espera medidas a cada 0.5 segundos.\n\
	Experiencia: $2 Router: 3\n Disciplina: $1\"; \
	plot '${TESTDIR}/exp$2ses1filas.txt' every 3::2 title 'Sessao:1' with lines, \
	'${TESTDIR}/exp$2ses2filas.txt' every 3::2 title 'Sessao:2' with lines, \
	'${TESTDIR}/exp$2ses3filas.txt' every 3::2 title 'Sessao:3' with lines;"\
	| gnuplot;
	
#
#	EOF
#

