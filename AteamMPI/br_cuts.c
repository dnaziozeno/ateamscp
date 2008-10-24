/* ------------------------------------------------------------------------- */
/*                                                                           */
/* DISSERTACAO DE MESTRADO -> DCC-UNICAMP                                    */
/*                                                                           */
/* Titulo    : Resolucao de problemas de Recobrimento  e  Particionamento de */
/*             Conjuntos com o uso de Times Assincronos.                     */
/*                                                                           */
/* Orientador: Marcus Vinicius S. Poggi de Aragao                            */
/* Orientando: Humberto Jose Longo                                           */
/*                                                                           */
/* Alteracoes: 0 -> (10/09/94)                                               */
/*                                                                           */
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/*                                                                           */
/*    Implementacao de um procedimento para gerar cortes  para o Problema de */
/* Recobrimento de Conjuntos. Este procedimento gera cortes  do seguinte ti- */
/* po: sum Xi <= 1. O procedimento foi proposto na referencia abaixo.        */
/*                                                                           */
/* Referencia-1  : Set Covering and Involutary Bases                         */
/*                 M. Bellmore and H. Ratliff                                */
/*                 Management Science 18 (3), pp 194-206, 1971               */
/*                                                                           */
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */

#include "globais.h"   /* Inclusao das definicoes de tipos, macros e constan-*/
                       /* tes utilizadas em todos os arquivos do A-Team dis- */
                       /* tribuido para o Set Covering Problem.              */

extern int      nb_lin;
extern int      nb_col;
extern int     *cost_col;
extern int     *identity_lin;
extern int     *identity_col;
extern int     *Q;
extern int     *primes;
extern Lin     *line;
extern Col     *column;

void ColsIdentity(int *var_x);
void LinsIdentity();
void ColsCut(int     *nb_cuts,
	     int     *var_x,
	     CutType *cut);


/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */

void GenBRCuts(int        *nb_cuts,
	       PrimalType *PrimalSol,
	       CutType    *cut)
{
  *nb_cuts = 0;
  memset(cost_col,-1,nb_lin * sizeof(int));
  ColsIdentity(PrimalSol->var_x);
  LinsIdentity();
  ColsCut(nb_cuts,PrimalSol->var_x,cut);
}

/* ------------------------------------------------------------------------- */
/* Identifica as colunas e as possiveis linhas que pertencem a sub-matriz i-
   dentidade da base B com propriedade de involucao. */

void ColsIdentity(int *var_x)
{
  int    j  = 0;
  DList *Ij = NULL;

  memset(identity_lin,0,nb_lin * sizeof(int));
  identity_col[nb_lin] = 0;
  for (j = 0; j < nb_col; j++)
   { if (var_x[column[j].var] == 0)
       continue;
     identity_col[identity_col[nb_lin]] = j;
     identity_col[nb_lin]++;
     Ij = column[j].first;
     while (Ij)
      { identity_lin[Ij->lin]++;
	Ij = Ij->down;
      }
   }
}

/* ------------------------------------------------------------------------- */
/* Identifica as linhas e o custo de cada uma das colunas associadas  a sub-
   matriz identidade da base B com propriedade de involucao. */

void LinsIdentity()
{
  int    j       = 0,
         lin_sum = 0;
  DList *Ij      = NULL;

  for (j = 0; j < identity_col[nb_lin]; j++)
   { lin_sum = 0;
     Ij = column[identity_col[j]].first;
     while (Ij)
      { if (identity_lin[Ij->lin] == 1)
	 { if (++lin_sum > 1)
	     identity_lin[Ij->lin] = MAXINT;
	   else
	     cost_col[Ij->lin] = j;
         }
	Ij = Ij->down;
      }
   }
}

/* ------------------------------------------------------------------------- */
/* Identifica as colunas que pertencem ao conjunto  Q,  ou seja, aquelas que
   compoem a nova restricao a ser adicionada ao problema. */

void ColsCut(int     *nb_cuts,
	     int     *var_x,
	     CutType *cut)
{
  short  *aux  = NULL;
  int     j    = 0;
  double  cost = 0;
  DList  *Ij   = NULL;


  aux = cut[*nb_cuts].coef;
  cut[*nb_cuts] = (CutType) {FALSE,FALSE,1,aux,0,0,0,-1,0};
  memset(cut[*nb_cuts].coef,0,nb_col * sizeof(short));
  for (j = 0; j < nb_col; j++)
   { if (var_x[column[j].var] == 1)
       continue;
     cost = column[j].coef;;
     Ij = column[j].first;
     while (Ij)
      { if (cost_col[Ij->lin] > -1)
	  cost -= column[cost_col[Ij->lin]].coef;
	Ij = Ij->down;
      }
     if (cost < 0)
      { cut[*nb_cuts].coef[j] = 1;
/*        cut[*nb_cuts].key += (j * 557); */
        cut[*nb_cuts].key += (j * primes[j]);
	cut[*nb_cuts].col_cut++;
      }
   }
  if (cut[*nb_cuts].col_cut > 0)
    (*nb_cuts)++;
}

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */
