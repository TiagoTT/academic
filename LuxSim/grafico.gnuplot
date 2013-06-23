#!/usr/bin/gnuplot

# Potencia de cada lampada -- [ W ]
P = 60

# Fluxo Luminoso / Potencia -- [ lm / W ]
flp = 100

# Angulo de Abertura (com a vertical) -- [ rad ]
alfa1 = 60 * ( pi / 180 ) 

# Angulo Solido de Abertura -- [ sr ]
asda = 2 * pi * ( cos( 0 ) - cos( alfa1 ) )
# 2 * pi * ( cos( alfa0 ) - cos( alfa1 ) ) = 2 * pi * ( cos(0) - cos(60) ) = 2 * pi * ( 1 - 0.5 )
# asda = pi

# Intensidade Luminosa Maxima -- [ cd ]
# Potencia * ( Fluxo Luminoso / Potencia ) / Angulo Solido de Abertura
Imax = P * flp / asda

# Angulo solido / Area -- [ sr / m2 ]
asa(x,y,z) = ( z / ( x**2 + y**2 + z**2 )**(1.5) )

# Intensidade Luminosa -- [ cd ]
il(x,y,z) = Imax * dl( atan( sqrt( x**2 + y**2 ) / z ) )

# Directividade da luz -- [ 0:1 ]
dl(alfa) = ( alfa < alfa1 ) ? 1 : 0

# Densidade do Fluxo Luminoso -- [ cd * sr / m2 ] = [ lm / m2 ] = [ lux ]
dfl(x,y,z) = il(x,y,z) * asa(x,y,z)

# Area a iluminar. -- 20 m de largura por 60 m de comprimento
L = 20
C = 60
set xrange [0:L]
set yrange [0:C]

# Pontos de luz.
ac = 1 # Altura do Chao

x1 = 4
x2 = 8
x3 = 12
x4 = 16

y1 = 5
y2 = 10
y3 = 15
y4 = 20
y5 = 25
y6 = 30
y7 = 35
y8 = 40
y9 = 45
y10 = 50
y11 = 55

## Soma Densidade do Fluxo Luminoso de todos os pontos de luz, apenas luz directa.
#dfld(x,y,z) = 0 \
# + dfl(x-x1,y-y1,z) + dfl(x-x2,y-y1,z) + dfl(x-x3,y-y1,z) + dfl(x-x4,y-y1,z) \
# + dfl(x-x1,y-y2,z) + dfl(x-x2,y-y2,z) + dfl(x-x3,y-y2,z) + dfl(x-x4,y-y2,z) \
# + dfl(x-x1,y-y3,z) + dfl(x-x2,y-y3,z) + dfl(x-x3,y-y3,z) + dfl(x-x4,y-y3,z) \
# + dfl(x-x1,y-y4,z) + dfl(x-x2,y-y4,z) + dfl(x-x3,y-y4,z) + dfl(x-x4,y-y4,z) \
# + dfl(x-x1,y-y5,z) + dfl(x-x2,y-y5,z) + dfl(x-x3,y-y5,z) + dfl(x-x4,y-y5,z) \
# + dfl(x-x1,y-y6,z) + dfl(x-x2,y-y6,z) + dfl(x-x3,y-y6,z) + dfl(x-x4,y-y6,z) \
# + dfl(x-x1,y-y7,z) + dfl(x-x2,y-y7,z) + dfl(x-x3,y-y7,z) + dfl(x-x4,y-y7,z) \
# + dfl(x-x1,y-y8,z) + dfl(x-x2,y-y8,z) + dfl(x-x3,y-y8,z) + dfl(x-x4,y-y8,z) \
# + dfl(x-x1,y-y9,z) + dfl(x-x2,y-y9,z) + dfl(x-x3,y-y9,z) + dfl(x-x4,y-y9,z) \
# + dfl(x-x1,y-y10,z) + dfl(x-x2,y-y10,z) + dfl(x-x3,y-y10,z) + dfl(x-x4,y-y10,z) \
# + dfl(x-x1,y-y11,z) + dfl(x-x2,y-y11,z) + dfl(x-x3,y-y11,z) + dfl(x-x4,y-y11,z) \
# + 0

# Soma Densidade do Fluxo Luminoso de todos os pontos de luz, e reflexoes das paredes.
# Indice de Reflexao das paredes -- [ 0:1 ]
ir = 0.0
dflt(x,y,z) = 0 \
 + dfl(x-x1,y-y1,z)  + dfl(x-x2,y-y1,z)  + dfl(x-x3,y-y1,z)  + dfl(x-x4,y-y1,z) \
 + dfl(x-x1,y-y2,z)  + dfl(x-x2,y-y2,z)  + dfl(x-x3,y-y2,z)  + dfl(x-x4,y-y2,z) \
 + dfl(x-x1,y-y3,z)  + dfl(x-x2,y-y3,z)  + dfl(x-x3,y-y3,z)  + dfl(x-x4,y-y3,z) \
 + dfl(x-x1,y-y4,z)  + dfl(x-x2,y-y4,z)  + dfl(x-x3,y-y4,z)  + dfl(x-x4,y-y4,z) \
 + dfl(x-x1,y-y5,z)  + dfl(x-x2,y-y5,z)  + dfl(x-x3,y-y5,z)  + dfl(x-x4,y-y5,z) \
 + dfl(x-x1,y-y6,z)  + dfl(x-x2,y-y6,z)  + dfl(x-x3,y-y6,z)  + dfl(x-x4,y-y6,z) \
 + dfl(x-x1,y-y7,z)  + dfl(x-x2,y-y7,z)  + dfl(x-x3,y-y7,z)  + dfl(x-x4,y-y7,z) \
 + dfl(x-x1,y-y8,z)  + dfl(x-x2,y-y8,z)  + dfl(x-x3,y-y8,z)  + dfl(x-x4,y-y8,z) \
 + dfl(x-x1,y-y9,z)  + dfl(x-x2,y-y9,z)  + dfl(x-x3,y-y9,z)  + dfl(x-x4,y-y9,z) \
 + dfl(x-x1,y-y10,z) + dfl(x-x2,y-y10,z) + dfl(x-x3,y-y10,z) + dfl(x-x4,y-y10,z) \
 + dfl(x-x1,y-y11,z) + dfl(x-x2,y-y11,z) + dfl(x-x3,y-y11,z) + dfl(x-x4,y-y11,z) \
 + 0 + ir * ( \
 + dfl(x-L-x1,y-y1,z)  + dfl(x-L-x2,y-y1,z)  + dfl(x-L-x3,y-y1,z)  + dfl(x-L-x4,y-y1,z) \
 + dfl(x-L-x1,y-y2,z)  + dfl(x-L-x2,y-y2,z)  + dfl(x-L-x3,y-y2,z)  + dfl(x-L-x4,y-y2,z) \
 + dfl(x-L-x1,y-y3,z)  + dfl(x-L-x2,y-y3,z)  + dfl(x-L-x3,y-y3,z)  + dfl(x-L-x4,y-y3,z) \
 + dfl(x-L-x1,y-y4,z)  + dfl(x-L-x2,y-y4,z)  + dfl(x-L-x3,y-y4,z)  + dfl(x-L-x4,y-y4,z) \
 + dfl(x-L-x1,y-y5,z)  + dfl(x-L-x2,y-y5,z)  + dfl(x-L-x3,y-y5,z)  + dfl(x-L-x4,y-y5,z) \
 + dfl(x-L-x1,y-y6,z)  + dfl(x-L-x2,y-y6,z)  + dfl(x-L-x3,y-y6,z)  + dfl(x-L-x4,y-y6,z) \
 + dfl(x-L-x1,y-y7,z)  + dfl(x-L-x2,y-y7,z)  + dfl(x-L-x3,y-y7,z)  + dfl(x-L-x4,y-y7,z) \
 + dfl(x-L-x1,y-y8,z)  + dfl(x-L-x2,y-y8,z)  + dfl(x-L-x3,y-y8,z)  + dfl(x-L-x4,y-y8,z) \
 + dfl(x-L-x1,y-y9,z)  + dfl(x-L-x2,y-y9,z)  + dfl(x-L-x3,y-y9,z)  + dfl(x-L-x4,y-y9,z) \
 + dfl(x-L-x1,y-y10,z) + dfl(x-L-x2,y-y10,z) + dfl(x-L-x3,y-y10,z) + dfl(x-L-x4,y-y10,z) \
 + dfl(x-L-x1,y-y11,z) + dfl(x-L-x2,y-y11,z) + dfl(x-L-x3,y-y11,z) + dfl(x-L-x4,y-y11,z) \
 + 0 \
 + dfl(x+L-x1,y-y1,z)  + dfl(x+L-x2,y-y1,z)  + dfl(x+L-x3,y-y1,z)  + dfl(x+L-x4,y-y1,z) \
 + dfl(x+L-x1,y-y2,z)  + dfl(x+L-x2,y-y2,z)  + dfl(x+L-x3,y-y2,z)  + dfl(x+L-x4,y-y2,z) \
 + dfl(x+L-x1,y-y3,z)  + dfl(x+L-x2,y-y3,z)  + dfl(x+L-x3,y-y3,z)  + dfl(x+L-x4,y-y3,z) \
 + dfl(x+L-x1,y-y4,z)  + dfl(x+L-x2,y-y4,z)  + dfl(x+L-x3,y-y4,z)  + dfl(x+L-x4,y-y4,z) \
 + dfl(x+L-x1,y-y5,z)  + dfl(x+L-x2,y-y5,z)  + dfl(x+L-x3,y-y5,z)  + dfl(x+L-x4,y-y5,z) \
 + dfl(x+L-x1,y-y6,z)  + dfl(x+L-x2,y-y6,z)  + dfl(x+L-x3,y-y6,z)  + dfl(x+L-x4,y-y6,z) \
 + dfl(x+L-x1,y-y7,z)  + dfl(x+L-x2,y-y7,z)  + dfl(x+L-x3,y-y7,z)  + dfl(x+L-x4,y-y7,z) \
 + dfl(x+L-x1,y-y8,z)  + dfl(x+L-x2,y-y8,z)  + dfl(x+L-x3,y-y8,z)  + dfl(x+L-x4,y-y8,z) \
 + dfl(x+L-x1,y-y9,z)  + dfl(x+L-x2,y-y9,z)  + dfl(x+L-x3,y-y9,z)  + dfl(x+L-x4,y-y9,z) \
 + dfl(x+L-x1,y-y10,z) + dfl(x+L-x2,y-y10,z) + dfl(x+L-x3,y-y10,z) + dfl(x+L-x4,y-y10,z) \
 + dfl(x+L-x1,y-y11,z) + dfl(x+L-x2,y-y11,z) + dfl(x+L-x3,y-y11,z) + dfl(x+L-x4,y-y11,z) \
 + 0 \
 + dfl(x-x1,y-C-y1,z)  + dfl(x-x2,y-C-y1,z)  + dfl(x-x3,y-C-y1,z)  + dfl(x-x4,y-C-y1,z) \
 + dfl(x-x1,y-C-y2,z)  + dfl(x-x2,y-C-y2,z)  + dfl(x-x3,y-C-y2,z)  + dfl(x-x4,y-C-y2,z) \
 + dfl(x-x1,y-C-y3,z)  + dfl(x-x2,y-C-y3,z)  + dfl(x-x3,y-C-y3,z)  + dfl(x-x4,y-C-y3,z) \
 + dfl(x-x1,y-C-y4,z)  + dfl(x-x2,y-C-y4,z)  + dfl(x-x3,y-C-y4,z)  + dfl(x-x4,y-C-y4,z) \
 + dfl(x-x1,y-C-y5,z)  + dfl(x-x2,y-C-y5,z)  + dfl(x-x3,y-C-y5,z)  + dfl(x-x4,y-C-y5,z) \
 + dfl(x-x1,y-C-y6,z)  + dfl(x-x2,y-C-y6,z)  + dfl(x-x3,y-C-y6,z)  + dfl(x-x4,y-C-y6,z) \
 + dfl(x-x1,y-C-y7,z)  + dfl(x-x2,y-C-y7,z)  + dfl(x-x3,y-C-y7,z)  + dfl(x-x4,y-C-y7,z) \
 + dfl(x-x1,y-C-y8,z)  + dfl(x-x2,y-C-y8,z)  + dfl(x-x3,y-C-y8,z)  + dfl(x-x4,y-C-y8,z) \
 + dfl(x-x1,y-C-y9,z)  + dfl(x-x2,y-C-y9,z)  + dfl(x-x3,y-C-y9,z)  + dfl(x-x4,y-C-y9,z) \
 + dfl(x-x1,y-C-y10,z) + dfl(x-x2,y-C-y10,z) + dfl(x-x3,y-C-y10,z) + dfl(x-x4,y-C-y10,z) \
 + dfl(x-x1,y-C-y11,z) + dfl(x-x2,y-C-y11,z) + dfl(x-x3,y-C-y11,z) + dfl(x-x4,y-C-y11,z) \
 + 0 \
 + dfl(x-x1,y+C-y1,z)  + dfl(x-x2,y+C-y1,z)  + dfl(x-x3,y+C-y1,z)  + dfl(x-x4,y+C-y1,z) \
 + dfl(x-x1,y+C-y2,z)  + dfl(x-x2,y+C-y2,z)  + dfl(x-x3,y+C-y2,z)  + dfl(x-x4,y+C-y2,z) \
 + dfl(x-x1,y+C-y3,z)  + dfl(x-x2,y+C-y3,z)  + dfl(x-x3,y+C-y3,z)  + dfl(x-x4,y+C-y3,z) \
 + dfl(x-x1,y+C-y4,z)  + dfl(x-x2,y+C-y4,z)  + dfl(x-x3,y+C-y4,z)  + dfl(x-x4,y+C-y4,z) \
 + dfl(x-x1,y+C-y5,z)  + dfl(x-x2,y+C-y5,z)  + dfl(x-x3,y+C-y5,z)  + dfl(x-x4,y+C-y5,z) \
 + dfl(x-x1,y+C-y6,z)  + dfl(x-x2,y+C-y6,z)  + dfl(x-x3,y+C-y6,z)  + dfl(x-x4,y+C-y6,z) \
 + dfl(x-x1,y+C-y7,z)  + dfl(x-x2,y+C-y7,z)  + dfl(x-x3,y+C-y7,z)  + dfl(x-x4,y+C-y7,z) \
 + dfl(x-x1,y+C-y8,z)  + dfl(x-x2,y+C-y8,z)  + dfl(x-x3,y+C-y8,z)  + dfl(x-x4,y+C-y8,z) \
 + dfl(x-x1,y+C-y9,z)  + dfl(x-x2,y+C-y9,z)  + dfl(x-x3,y+C-y9,z)  + dfl(x-x4,y+C-y9,z) \
 + dfl(x-x1,y+C-y10,z) + dfl(x-x2,y+C-y10,z) + dfl(x-x3,y+C-y10,z) + dfl(x-x4,y+C-y10,z) \
 + dfl(x-x1,y+C-y11,z) + dfl(x-x2,y+C-y11,z) + dfl(x-x3,y+C-y11,z) + dfl(x-x4,y+C-y11,z) \
 + 0 )


# Titulo do grafico.
set title "Iluminancia [lux]"
set xlabel "Largura [m]"
set ylabel "Comprimento [m]"

# Resolucao x,y dos pontos.
set sample 100
set isosample 100

# Mostrar grafico 3d com superficie a cores.
#set pm3d
# Mostrar grafico 2d com superficie a cores.
set pm3d map

# Obter uma tabela com os valores calculados.
#set table "resultados.txt"

# Obter uma figura com o grafico.
set term png
set output "res.png"

# Calcular os valores e apresentar o grafico.
splot dflt(x,y,ac)

# EOF
