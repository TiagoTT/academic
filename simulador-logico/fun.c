/*
 * Ficheiro: fun.c
 */

#include "fun.h"

/* 1 input gates: */
int tabela1[N_TIPOS][N_VALORES] = {
/*         {    0       1       x       d       dbar    }       */
/* and  */ {    ZERO,   ONE,    X,      D,      DBAR    },
/* nand */ {    ONE,    ZERO,   X,      DBAR,   D       },
/* or   */ {    ZERO,   ONE,    X,      D,      DBAR    },
/* nor  */ {    ONE,    ZERO,   X,      DBAR,   D       },
/* xor  */ {    ZERO,   ONE,    X,      D,      DBAR    },
/* xnor */ {    ONE,    ZERO,   X,      DBAR,   D       }};

/* 2 input gates */
int tabela2[N_TIPOS][N_VALORES][N_VALORES] = {
/*         {    0       1       x       d       dbar    }       */
/*and  */ {{    ZERO,   ZERO,   ZERO,   ZERO,   ZERO    },      /* 0 */
           {    ZERO,   ONE,    X,      D,      DBAR    },      /* 1 */
           {    ZERO,   X,      X,      X,      X       },      /* x */
           {    ZERO,   D,      X,      D,      ZERO    },      /* d */
           {    ZERO,   DBAR,   X,      ZERO,   DBAR    }},     /* dbar */
/*nand */ {{    ONE,    ONE,    ONE,    ONE,    ONE     },      /* 0 */
           {    ONE,    ZERO,   X,      DBAR,   D       },      /* 1 */
           {    ONE,    X,      X,      X,      X       },      /* x */
           {    ONE,    DBAR,   X,      DBAR,   ONE     },      /* d */
           {    ONE,    D,      X,      ONE,    D       }},     /* dbar */
/*or   */ {{    ZERO,   ONE,    X,      D,      DBAR    },      /* 0 */
           {    ONE,    ONE,    ONE,    ONE,    ONE     },      /* 1 */
           {    X,      ONE,    X,      X,      X       },      /* x */
           {    D,      ONE,    X,      D,      ONE     },      /* d */
           {    DBAR,   ONE,    X,      ONE,    DBAR    }},     /* dbar */
/*nor  */ {{    ONE,    ZERO,   X,      DBAR,   D       },      /* 0 */
           {    ZERO,   ZERO,   ZERO,   ZERO,   ZERO    },      /* 1 */
           {    X,      ZERO,   X,      X,      X       },      /* x */
           {    DBAR,   ZERO,   X,      DBAR,   ZERO    },      /* d */
           {    D,      ZERO,   X,      ZERO,   D       }},     /* dbar */
/*xor  */ {{    ZERO,   ONE,    X,      D,      DBAR    },      /* 0 */
           {    ONE,    ZERO,   X,      DBAR,   D       },      /* 1 */
           {    X,      X,      X,      X,      X       },      /* x */
           {    D,      DBAR,   X,      ZERO,   ONE     },      /* d */
           {    DBAR,   D,      X,      ONE,    ZERO    }},     /* dbar */
/*xnor */ {{    ONE,    ZERO,   X,      DBAR,   D       },      /* 0 */
           {    ZERO,   ONE,    X,      D,      DBAR    },      /* 1 */
           {    X,      X,      X,      X,      X       },      /* x */
           {    DBAR,   D,      X,      ONE,    ZERO    },      /* d */
           {    D,      DBAR,   X,      ZERO,   ONE     }}};    /* dbar */


void calculaValor(Circuito* circuito, int node_index)
{
  int i;
  Node* node;
  
  node = circuito->vectorNodes + node_index ;
  
  /* Os calculos sao efectuados como se NAND(1), NOR(3) e XNOR(5) fossem 
   * AND(0), OR(2) e XOR(4) respectivamente.
   * No final realiza-se o complemento do valor.
   */
  node->valor = tabela1[ ( node->tipo / 2 ) * 2 ]
    [ circuito->vectorNodes[ node->inputs[ 0 ] ].valor ];
  
  for( i = 1 ; i < node->fanin ; i++ )
    {
      node->valor = tabela2[ ( node->tipo / 2 ) * 2 ]
	[ node->valor ][ circuito->vectorNodes[ node->inputs[ i ] ].valor ];
    }
  
  if( node->tipo % 2 )
    {
      node->valor = tabela1[ NOR ]
	[ node->valor ];
    }
  
  return;
}
