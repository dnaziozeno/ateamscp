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
extern int      MaxCutGen;
extern int     *cost_col;
extern int     *identity_lin;
extern int     *identity_col;
extern int     *Q;
extern int     *S;
extern int     *primes;
extern Lin     *line;
extern Col     *column;
extern RandInd *set_ind;

void ColsIdentity(int *var_x);
void LinsIdentity();
void ColsCut(int     *nb_cuts,
	     int     *var_x,
	     CutType *cut);
void CalcIdentityCosts();
void RandomizeNbLines();
int TestSort(const void *a,
             const void *b);


/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */

void GenBRCuts(int        *nb_cuts,
	       PrimalType *PrimalSol,
	       CutType    *cut)
{
  int i = 0;

  *nb_cuts = 0;
  memset(cost_col,-1,nb_lin * sizeof(int));
  ColsIdentity(PrimalSol->var_x);
  LinsIdentity();
  for (i = 0; i < MaxCutGen; i++)
    { CalcIdentityCosts();
      ColsCut(nb_cuts,PrimalSol->var_x,cut);
      RandomizeNbLines();
    }
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
  int    j  = 0;
  DList *Ij = NULL;

  set_ind[nb_lin].j = 0;
  for (j = 0; j < identity_col[nb_lin]; j++)
   { Ij = column[identity_col[j]].first;
     while (Ij)
      { if (identity_lin[Ij->lin] == 1)
	 { set_ind[set_ind[nb_lin].j].j    = Ij->lin;
	   set_ind[set_ind[nb_lin].j].sort = lrand48();
	   set_ind[nb_lin].j++;
	   cost_col[Ij->lin] = j;
         }
	Ij = Ij->down;
      }
   }
  memcpy(S,cost_col,nb_lin * sizeof(int));
}

/* ------------------------------------------------------------------------- */
/* Identifica as linhas da matriz identidade e os custos de cada coluna. */

void CalcIdentityCosts()
{
  int i = 0,
      j = 0,
      k = 0;

  memcpy(cost_col,S,nb_lin * sizeof(int));
  for (j = 0; j < set_ind[nb_lin].j; j++)
   { k = set_ind[j].j;
     for (i = 0; i < nb_lin; i++)
       if ((i != k) && (cost_col[k] == cost_col[i]))
	   cost_col[i] = -1;
   }
}

/* ------------------------------------------------------------------------- */
/* Gera uma nova ordenacao para o conjunto de linhas saturadas. */

void RandomizeNbLines()
{
  int i = 0;

  for (i = 0; i < set_ind[nb_lin].j; i++)
    set_ind[i].sort = lrand48();
  qsort((void *)set_ind,set_ind[nb_lin].j,sizeof(RandInd),TestSort);
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
/*
int TestSort(const void *a,
             const void *b)
{
  return(((RandInd *)a)->sort - ((RandInd *)b)->sort);
}
*/
/* ------------------------------------------------------------------------- */

