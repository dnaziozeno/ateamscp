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
/*    Implementacao de uma heuristica gulosa para gerar recobrimentos. Toda  */
/* a notacao usada esta de acordo com a notacao usada na referencia-1. O a-  */
/* juste das colunas, com a exclusao de colunas redundates ao recobrimento   */
/* foi construido a partir de esquema proposta na referencia-2.  O uso  de   */
/* mais de uma funcao na escolha de uma variavel esta de acordo com a refe-  */
/* rencia-3.                                                                 */
/*                                                                           */
/* Referencia-1  : Optimal Solution of Set Covering/Partitioning Problems    */
/*                 Using Dual Heuristics                                     */
/*                 Marshall L. Fisher and Pradeep Kedia                      */
/*                 Management Science 36 (6), June 1990                      */
/*                                                                           */
/* Referencia-2  : A Lagrangian Heuristic for Set-Covering Problems          */
/*                 J. E. Beasley                                             */
/*                 Naval Research Logistics 37, pp. 151-164, 1990            */
/*                                                                           */
/* Referencia-3  : Set Covering Algorithms Using Cutting Planes, Heuristics  */
/*                 and Subgradient Optimization: A Computational Study       */
/*                 Egon Balas and Andrew Ho                                  */
/*                 Mathematical Programming 12, pp. 37-60, 1980              */
/*                                                                           */
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */

#include "globais.h"   /* Inclusao das definicoes de tipos, macros e constan-*/
                       /* tes utilizadas em todos os arquivos do A-Team dis- */
                       /* tribuido para o Set Covering Problem.              */

extern int     nb_lin;
extern int     nb_col;
extern int    *U_x;
extern int    *unsat;
extern double *aux_cost;
extern Lin    *line;
extern Col    *column;

void BestLine(int    *best,
	      double *var_u);
void BestColumn(int     heur,
		int    *best,
		int    *var_x,
		double *var_u);
void CalcCosts(int     k,
	       int    *size_Ik,
	       double *Ck_reduc,
	       double *var_u);
void CalcUnsatis(int *var_x);
void RecalcUnsatis(int  best,
		   int *var_x);
void AdjustColumns(PrimalType *PrimalSol);
void CalcReducedCostsAux(DualType *DualSol);

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */

void PrimalGreedy(char        RandomGreedy,
		  DualType   *DualSol,
		  PrimalType *PrimalSol)
{
  int heur = 0,
      best = 0;

  if (RandomGreedy)
    heur = lrand48() % 7;
  if (DualSol->nb_cuts > 0)
    CalcReducedCostsAux(DualSol);
  else
    memset(aux_cost,0,nb_col * sizeof(double));
  CalcUnsatis(PrimalSol->var_x);
  while (U_x[nb_lin] > 0)
   { BestLine(&best,DualSol->var_u);
     BestColumn(heur,&best,PrimalSol->var_x,DualSol->var_u);
     PrimalSol->var_x[column[best].var] = 1;
     PrimalSol->value += column[best].coef;
     RecalcUnsatis(best,PrimalSol->var_x);
   }
  AdjustColumns(PrimalSol);

  PrimalSol->agent[agent_primal]    = TRUE;
  PrimalSol->agent[agent_beasley]   = FALSE;
  PrimalSol->agent[agent_consensus] = FALSE;
  PrimalSol->agent[agent_adjust]    = FALSE;
  PrimalSol->agent[agent_tabu]      = FALSE;
  PrimalSol->agent[agent_search]    = FALSE;
  PrimalSol->agent_ID               = agent_primal;
}

/* ------------------------------------------------------------------------- */

void BestLine(int    *best,
	      double *var_u)
{
  int    i         = 0;
  double delta     = 0,
         max_delta = -MAXDOUBLE;

  *best = 0;
  for (i = 0; i < U_x[nb_lin]; i++)
   { delta = var_u[U_x[i]] * ((double) line[U_x[i]].elem_lin);
     if ((delta > max_delta) || ((delta == max_delta) &&
	 (line[U_x[i]].elem_lin > line[*best].elem_lin)))
      { max_delta = delta;
	*best     = U_x[i];
      }
   }
}

/* ------------------------------------------------------------------------- */

void BestColumn(int     heur_type,
		int    *best,
		int    *var_x,
		double *var_u)
{
  int    j          = 0,
         heur       = 0,
         size_Ik    = 0;
  double Ck_reduc   = 0,
         tie_broken = 0,
         min_Ck     = MAXDOUBLE,
         min_tie    = MAXDOUBLE;
  DList *Ji         = NULL;

  Ji = line[*best].first;
  while (Ji)
   { CalcCosts(Ji->col,&size_Ik,&Ck_reduc,var_u);
     tie_broken = column[Ji->col].coef;
     if (size_Ik > 1)
       tie_broken = tie_broken / size_Ik;
     if (heur_type == 6)
       heur = lrand48() % 6;
     else
       heur = heur_type;
     switch (heur)
      { case 0 : if (size_Ik > 1)
		   Ck_reduc = Ck_reduc / size_Ik;
 	         break;
        case 1 : Ck_reduc = column[Ji->col].coef;
	         break;
        case 2 : if (size_Ik > 1)
	           Ck_reduc = Ck_reduc / (size_Ik * size_Ik);
	         break;
        case 3 : if ((Ck_reduc > 1) && (size_Ik > 1))
	           Ck_reduc = sqrt(Ck_reduc) / (size_Ik * size_Ik);
	         break;
        case 4 : if (size_Ik > 1)
	           Ck_reduc = column[Ji->col].coef / (size_Ik * size_Ik);
	         break;
        case 5 : if ((Ck_reduc > 0) && (size_Ik > 1))
	           Ck_reduc = sqrt(column[Ji->col].coef) / (size_Ik * size_Ik);
      }
     if ((Ck_reduc < min_Ck) ||
	 ((Ck_reduc == min_Ck) && (tie_broken < min_tie)))
      { min_Ck  = Ck_reduc;
	min_tie = tie_broken;
	*best   = Ji->col;
      }
     Ji = Ji->right;
   }
}

/* ------------------------------------------------------------------------- */

void CalcCosts(int     k,
	       int    *size_Ik,
	       double *Ck_reduc,
	       double *var_u)
{
  DList *Ik = NULL;

  *size_Ik  = 0;
  *Ck_reduc = column[k].coef;
  Ik        = column[k].first;
  while (Ik)
   { if (unsat[Ik->lin] != -1)
      { *Ck_reduc -= var_u[Ik->lin];
	(*size_Ik)++;
      }
     Ik = Ik->down;
   }
  *Ck_reduc -= aux_cost[k];
}

/* ------------------------------------------------------------------------- */

void CalcUnsatis(int *var_x)
{
  int i = 0,
      k = 0;

  for (i = 0; i < nb_lin; i++)
    if (line[i].del)
      unsat[i] = -1;
    else
     { U_x[k]   = i;
       unsat[i] = k;
       k++;
     }
  U_x[nb_lin] = k;
  for (i = 0; i < nb_col; i++)
    if (var_x[column[i].var] == 1)
      RecalcUnsatis(i,var_x);
}

/* ------------------------------------------------------------------------- */

void RecalcUnsatis(int  best,
		   int *var_x)
{
  int    i  = 0;
  DList *Ij = NULL;

  Ij = column[best].first;
  while (Ij)
   { /* Se a linha i = Ij->lin ainda nao foi satisfeita, e se a variavel  Xj
	multiplicada pelo coeficiente Cij for suficiente para  satisfazer  a
	linha i, entao ela e' excluida do conjunto de nao satisfeitas */
     if (unsat[Ij->lin] != -1)
/*     if ((unsat[Ij->lin] != -1) && ((var_x[column[i].var] * Ij->coef) >= 1)) */
      { i = unsat[Ij->lin];
	unsat[Ij->lin] = -1;
	(U_x[nb_lin])--;
	U_x[i] = U_x[U_x[nb_lin]];
	if (i < U_x[nb_lin])
	  unsat[U_x[i]] = i;
      }
     Ij = Ij->down;
   }
}

/* ------------------------------------------------------------------------- */

void AdjustColumns(PrimalType *PrimalSol)
{
  char   column_cover = FALSE,
         line_covered = FALSE;
  int    j            = 0;
  DList *Ij           = NULL,
        *Ji           = NULL;

  for (j = nb_col-1; j >= 0; j--)
   { if (PrimalSol->var_x[column[j].var] == 0)
       continue;
     column_cover = FALSE;
     Ij = column[j].first;
     while ((Ij) && (!column_cover))
      { line_covered = FALSE;
	if (!line[Ij->lin].del)
	 { Ji = line[Ij->lin].first;
	   while ((Ji) && (!line_covered))
	    { if ((PrimalSol->var_x[column[Ji->col].var] == 1) &&
		  (Ji->col != j))
		line_covered = TRUE;
	      Ji = Ji->right;
	    }
	   if (!line_covered)
	     column_cover = TRUE;
         }
	Ij = Ij->down;
      }
     if (!column_cover)
      { PrimalSol->var_x[column[j].var] = 0;
	PrimalSol->value -= column[j].coef;
      }
   }
}

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */

void CalcReducedCostsAux(DualType *DualSol)
{
  int    j  = 0;
  DList *Ij = NULL;

  for (j = 0; j < nb_col; j++)
   { aux_cost[j] = 0;
     Ij          = column[j].first;
     while (Ij)
      { aux_cost[j] += DualSol->var_u[Ij->lin];
	Ij = Ij->down;
      }
     aux_cost[j] = DualSol->red_cost[j] - (column[j].coef - aux_cost[j]);
   }
}

/* ------------------------------------------------------------------------- */

