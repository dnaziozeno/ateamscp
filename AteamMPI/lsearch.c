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
/*                                                                           */
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */

#include "globais.h"   /* Inclusao das definicoes de tipos, macros e constan-*/
                       /* tes utilizadas em todos os arquivos do A-Team dis- */
                       /* tribuido para o Set Covering Problem.              */

int     trigger_tabu = 0,
        nb_uncover   = 0;
double  penalty      = 0;

extern int      nb_lin;
extern int      nb_col;
extern int      TabuIterations;
extern int     *cover;
extern int     *tabu;
extern int     *aux_x;
extern RandInd *set_ind;
extern Lin     *line;
extern Col     *column;

void   GenCovers(PrimalType *PrimalSol);
double CalcFuncObj(int         i,
		   int         k,
		   PrimalType *PrimalSol);
void   RecalcCovers(int         index,
		    PrimalType *PrimalSol);
void   BestNeighbour(int        *index_j,
                     int        *index_k,
		     double      Z_opt,
		     double     *Z_aux,
		     PrimalType *PrimalSol);
int TestSort(const void *a,
             const void *b);

/* ------------------------------------------------------------------------- */

void LocalSearch(PrimalType *PrimalSol)
{
  char    change  = FALSE;
  int     index_j = 0,
          index_k = 0;
  double  Z_aux   = 0,
          Z_opt   = 0;

  qsort((void *)set_ind,nb_col,sizeof(RandInd),TestSort);
  memset(tabu,0,nb_col * sizeof(int));
  memcpy(aux_x,PrimalSol->var_x,nb_col * sizeof(int));
  penalty = nb_lin * column[nb_col - 1].coef;
  Z_opt   = PrimalSol->value;
  GenCovers(PrimalSol);

  do
   { change = FALSE;
     BestNeighbour(&index_j,&index_k,Z_opt,&Z_aux,PrimalSol);
     if (Z_aux < Z_opt)
      { change = TRUE;
	Z_opt  = Z_aux;
	memcpy(aux_x,PrimalSol->var_x,nb_col * sizeof(int));
	aux_x[column[index_j].var] = 1 - aux_x[column[index_j].var];
	if (index_j != index_k)
	  aux_x[column[index_k].var] = 1 - aux_x[column[index_k].var];
      }
     if (Z_aux >= (PrimalSol->value - epsilon))
      { tabu[index_j] = trigger_tabu + TabuIterations;
	if (index_j != index_k)
	  tabu[index_k] = trigger_tabu + TabuIterations;
      }
     PrimalSol->value = Z_aux;
     PrimalSol->var_x[column[index_j].var] = 1 -
       PrimalSol->var_x[column[index_j].var];
     if (index_j != index_k)
       PrimalSol->var_x[column[index_k].var] = 1 -
	 PrimalSol->var_x[column[index_k].var];
     trigger_tabu++;
   } while (change);

  if (Z_opt < PrimalSol->value)
   { PrimalSol->value = Z_opt;
     memcpy(PrimalSol->var_x,aux_x,nb_col * sizeof(int));

     PrimalSol->agent[agent_primal]    = FALSE;
     PrimalSol->agent[agent_beasley]   = FALSE;
     PrimalSol->agent[agent_consensus] = FALSE;
     PrimalSol->agent[agent_adjust]    = FALSE;
     PrimalSol->agent[agent_tabu]      = FALSE;
     PrimalSol->agent[agent_search]    = TRUE;
     PrimalSol->agent_ID               = agent_search;
   }
/* printf("\n(sch) = %f\n",PrimalSol->value); */
}

/* ------------------------------------------------------------------------- */

void BestNeighbour(int        *index_j,
		   int        *index_k,
		   double      Z_opt,
		   double     *Z_aux,
		   PrimalType *PrimalSol)
{
  int    j = 0,
         k = 0,
         r = 0,
         s = 0;
  double Z = 0;

  *Z_aux = MAXDOUBLE;
  for (r = 0; r < nb_col; r++)
   { j = set_ind[r].j;
     for (s = 0; s < nb_col; s++)
      { k = set_ind[s].j;
	Z = CalcFuncObj(j,k,PrimalSol);
	if ((tabu[j] <= trigger_tabu) && (tabu[k] <= trigger_tabu))
	 { if (Z < (*Z_aux - epsilon))
	    { *index_j = j;
	      *index_k = k;
	      *Z_aux   = Z;
	    }
         }
	else
	 { /* Criterio de aspiracao */
	   if ((Z < (Z_opt - epsilon)) && (Z < (*Z_aux - epsilon)))
	    { *index_j = j;
	      *index_k = k;
	      *Z_aux   = Z;
	    }
         }
      }
   }
  RecalcCovers(*index_j,PrimalSol);
  if (*index_j != *index_k)
    RecalcCovers(*index_k,PrimalSol);
}

/* ------------------------------------------------------------------------- */

double CalcFuncObj(int         j,
                   int         k,
		   PrimalType *PrimalSol)
{
  int     indj   = 0,
          indk   = 0,
          incr_j = 0,
          incr_k = 0;
  double  new_Z  = 0;
  DList  *Ij     = NULL,
         *Ik     = NULL;

  incr_j = 1 - (2 * PrimalSol->var_x[column[j].var]);
  incr_k = 1 - (2 * PrimalSol->var_x[column[k].var]);
  new_Z = PrimalSol->value + (incr_j * column[j].coef);
  if (j != k)
   { new_Z = new_Z + (incr_k * column[k].coef);
     Ik = column[k].first;
   }
  Ij = column[j].first;
  if (PrimalSol->var_x[column[j].var] == 0)
    if (PrimalSol->var_x[column[k].var] == 0)
     { if (nb_uncover > 0)
	 while (Ij || Ik)
	  { indj = (Ij) ? Ij->lin : MAXINT;
	    indk = (Ik) ? Ik->lin : MAXINT;
	    if (indj > indk)
	     { indj = indk;
	       Ik   = Ik->down;
	     }
	    else
	     { Ij = Ij->down;
	       if (indj == indk)
		 Ik = Ik->down;
	     }
	    if (cover[indj] == 0)
	      new_Z -= penalty;
	  }
     }
    else  /* xj = 0, xk = 1 */
      while (Ij || Ik)
       { indj = (Ij) ? Ij->lin : MAXINT;
	 indk = (Ik) ? Ik->lin : MAXINT;
	 if (indj > indk)
	  { Ik = Ik->down;
	    if (cover[indk] == 1)
	      new_Z += penalty;
	  }
	 else
	  { Ij = Ij->down;
	    if (indj < indk)
	     { if (cover[indj] == 0)
		 new_Z -= penalty;
	     }
	    else
	      Ik = Ik->down;
	  }
       }
  else  /* xj = 1 xk = 0 */
    if (PrimalSol->var_x[column[k].var] == 0)
      while (Ij || Ik)
       { indj = (Ij) ? Ij->lin : MAXINT;
	 indk = (Ik) ? Ik->lin : MAXINT;
	 if (indj > indk)
	  { Ik = Ik->down;
	    if (cover[indk] == 0)
	      new_Z -= penalty;
	  }
	 else
	  { Ij = Ij->down;
	    if (indj < indk)
	     { if (cover[indj] == 1)
		 new_Z += penalty;
	     }
	    else
	      Ik = Ik->down;
	  }
        }
    else /* xj = 1 xk = 1 */
      while (Ij || Ik)
       { indj = (Ij) ? Ij->lin : MAXINT;
	 indk = (Ik) ? Ik->lin : MAXINT;
	 if (indj > indk)
	  { Ik = Ik->down;
	    if (cover[indk] == 1)
	      new_Z += penalty;
	  }
	 else
	  { Ij = Ij->down;
	    if (indj < indk)
	     { if (cover[indj] == 1)
		 new_Z += penalty;
	     }
	    else
	     { Ik = Ik->down;
	       if (cover[indj] == 2)
		 new_Z += penalty;
	     }
	  }
       }
  return(new_Z);
}

/* ------------------------------------------------------------------------- */

void RecalcCovers(int         index,
		  PrimalType *PrimalSol)
{
  int    incr = 0;
  DList *Ij   = NULL;

  incr = 1 - (2 * PrimalSol->var_x[column[index].var]);
  Ij   = column[index].first;
  while(Ij)
   { cover[Ij->lin] += incr;
     if (cover[Ij->lin] == 0)
       nb_uncover++;
     else
       if ((cover[Ij->lin] == 1) && (PrimalSol->var_x[column[index].var] == 0))
	 nb_uncover--;
     Ij = Ij->down;
   }
}

/* ------------------------------------------------------------------------- */

void GenCovers(PrimalType *PrimalSol)
{
  int    j  = 0;
  DList *Ij = NULL;

  memset(cover,0,nb_lin * sizeof(int));
  for (j = 0; j < nb_col; j++)
    if (PrimalSol->var_x[column[j].var] == 1)
     { Ij = column[j].first;
       while (Ij)
	{ (cover[Ij->lin])++;
	  Ij = Ij->down;
	}
     }
}

/* ------------------------------------------------------------------------- */

int TestSort(const void *a,
             const void *b)
{
  return(((RandInd *)a)->sort - ((RandInd *)b)->sort);
}

/* ----------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ----------------------------------------------------------------------- */

