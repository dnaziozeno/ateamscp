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

extern int  nb_col;
extern int  nb_lin;
extern int  MinChange;
extern int *cover;
extern int *aux_x;
extern Col *column;

void GenSolutions(PrimalType *PrimalSol);
void CalcCovers(PrimalType *PrimalSol);
void AdjustCols(PrimalType *PrimalSol);

/* ----------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ----------------------------------------------------------------------- */

void RandomAdjust(PrimalType *PrimalSol)
{
  int    i      = 0,
         iter   = 0;
  double Z_aux  = 0,
         Z_orig = 0;

  memcpy(aux_x,PrimalSol->var_x,nb_col * sizeof(int));
  Z_orig = Z_aux = PrimalSol->value;
  iter = (nb_col < nb_lin) ? nb_col : nb_lin;
  iter = (int) (iter / 2);
  for (i = 0; i < iter; i++)
   { CalcCovers(PrimalSol);
     GenSolutions(PrimalSol);
     AdjustCols(PrimalSol);
     if (PrimalSol->value < Z_aux)
      { Z_aux = PrimalSol->value;
	memcpy(aux_x,PrimalSol->var_x,nb_col * sizeof(int));
      }
     else
       if (PrimalSol->value > Z_aux)
	{ PrimalSol->value = Z_aux;
	  memcpy(PrimalSol->var_x,aux_x,nb_col * sizeof(int));
        }
   }

  if (PrimalSol->value < Z_orig)
   { PrimalSol->agent[agent_primal]    = FALSE;
     PrimalSol->agent[agent_beasley]   = FALSE;
     PrimalSol->agent[agent_consensus] = FALSE;
     PrimalSol->agent[agent_adjust]    = TRUE;
     PrimalSol->agent[agent_tabu]      = FALSE;
     PrimalSol->agent[agent_search]    = FALSE;
     PrimalSol->agent_ID               = agent_adjust;
   }
}

/* ------------------------------------------------------------------------- */

void GenSolutions(PrimalType *PrimalSol)
{
  int    i      = 0,
         j      = 0,
         iter   = 0,
         max    = 0,
         nb_var = 0;
  DList *Ij     = NULL;

  max = (nb_lin < nb_col) ? nb_col : nb_lin;
  nb_var = 2 * nb_col;
  if (nb_var < nb_lin)
    nb_var = MinChange + (lrand48() % ((int)(max / nb_lin)));
  else
    nb_var = MinChange + (lrand48() % ((int)(nb_var / nb_lin)));
  while ((i < nb_var) && (++iter < max))
/*   { j = (int)((nb_col - 1) * (1 - sqrt(1 - drand48()))) + 1; */
   { j = lrand48() % nb_col;
     if (PrimalSol->var_x[column[j].var] == 0)
      { PrimalSol->var_x[column[j].var] = 1;
	PrimalSol->value += column[j].coef;
	Ij = column[j].first;
	while (Ij)
	 { (cover[Ij->lin])++;
	   Ij = Ij->down;
	 }
	i++;
      }
   }
}

/* ------------------------------------------------------------------------- */

void CalcCovers(PrimalType *PrimalSol)
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

/* ----------------------------------------------------------------------- */

void AdjustCols(PrimalType *PrimalSol)
{
  char   redun_col = TRUE;
  int    j         = 0;
  DList *Ij        = NULL;

  for (j = nb_col-1; j >= 0; j--)
   { if (PrimalSol->var_x[column[j].var] == 0)
       continue;
     Ij = column[j].first;
     while ((Ij) && (redun_col))
      { if (cover[Ij->lin] == 1)
	  redun_col = FALSE;
	Ij = Ij->down;
      }
     if (redun_col)
      { PrimalSol->value -= column[j].coef;
        PrimalSol->var_x[column[j].var] = 0;
	Ij = column[j].first;
	while (Ij)
	 { (cover[Ij->lin])--;
	   Ij = Ij->down;
	 }
      }
     else
       redun_col = TRUE;
   }
}

/* ----------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ----------------------------------------------------------------------- */
