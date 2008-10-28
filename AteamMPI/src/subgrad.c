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
/* Este arquivo contem o codigo necessario para o agente subgradiente.  Este */
/* agente requisita aos servidores de memorias dual e primal uma solucao du- */
/* al e uma primal, buscando posteriormente a melhoria da solucao dual atra- */
/* ves do metodo de otimizacao por subgradientes.                            */
/* ------------------------------------------------------------------------- */
 
/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */

#include "globais.h"    /* Inclusao das definicoes de tipos, macros e  cons- */
			/* tantes utilizadas em todos os arquivos  do A-Team */
                        /* distribuido para o Set Covering Problem.          */

extern int     nb_col;
extern int     nb_lin;
extern int    *aux_x;
extern float  *grad;
extern double *aux_u;
extern Lin    *line;
extern Col    *column;

void   AdjustRedCosts(int     lin,
		      double  diff,
		      double *red_cost,
		      double *Zd);
char   StopCriterion(float      *lambda,
		     double     *best_Zd,
		     DualType   *DualSol,
		     PrimalType *PrimalSol);
double CalcStep(float  lambda,
		double Zd,
		double Z_primal);
double ZD_u(double    best_Zd,
	    DualType *DualSol);
void   RecalcReducedCosts(DualType *DualSol);
void   RestoreFeasibility(DualType *DualSol);
int    TestRedCost(const void *a,
		   const void *b);

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */

void SubGradiente(float       lambda,
		  DualType   *DualSol,
		  PrimalType *PrimalSol)
{
  char    exec    = TRUE;
  int     i       = 0;
  double  Zd      = 0,
          best_Zd = 0,
          new_u   = 0,
          max_u   = 0,
          step    = 0;
  DList  *Ji      = NULL;

/*printf("\ncut = %i",DualSol->nb_cuts);
printf("\nmem = %f",DualSol->value); */
  memcpy(aux_u,DualSol->var_u,nb_lin * sizeof(double));
  best_Zd = DualSol->value;

  Zd = ZD_u(best_Zd,DualSol);
  while (exec)
   { step = CalcStep(lambda,Zd,PrimalSol->value);
     DualSol->value = 0;
     for (i = 0; i < nb_lin; i++)
       if (!line[i].del)
	{ new_u = (DualSol->var_u[i] + (step * grad[i]));
	  if (new_u < epsilon)
	    DualSol->value += (DualSol->var_u[i] * line[i].rhs);
	  else
	   { Ji = line[i].first;
	     while (Ji)
	      { max_u = column[Ji->col].coef / Ji->coef;
		if (new_u > max_u)
		  new_u = max_u;
		Ji = Ji->right;
	      }
	     AdjustRedCosts(i,new_u - DualSol->var_u[i],DualSol->red_cost,&Zd);
	     DualSol->var_u[i] = new_u;
	     DualSol->value   += (new_u * line[i].rhs);
	   }
        }
     exec = StopCriterion(&lambda,&best_Zd,DualSol,PrimalSol);
   }
  memcpy(DualSol->var_u,aux_u,nb_lin * sizeof(double));
  DualSol->value = best_Zd;
  RecalcReducedCosts(DualSol);
/*printf("\nsgr = %f",DualSol->value);*/
  RestoreFeasibility(DualSol);
/*printf("\nprj = %f\n",DualSol->value);*/

  DualSol->improved_sol         = TRUE;
  DualSol->agent[agent_dual]    = FALSE;
  DualSol->agent[agent_3opt]    = FALSE;
  DualSol->agent[agent_subgrad] = FALSE;
  DualSol->agent[agent_free]    = FALSE;
  DualSol->agent_ID             = agent_subgrad;
}

/* ------------------------------------------------------------------------- */

double ZD_u(double    best_Zd,
	    DualType *DualSol)
{
  int     j   = 0;
  double  Zd  = 0;
  DList  *Ij  = NULL;

  Zd = best_Zd;
  for (j = 0; j < nb_col; j++)
    if (DualSol->red_cost[j] < 0)
     { aux_x[j] = 1;
       Zd += DualSol->red_cost[j];
     }
    else
      aux_x[j] = 0;
  return(Zd);
}

/* ------------------------------------------------------------------------- */

double CalcStep(float  lambda,
		double Zd,
		double Z_primal)
{
  int     i        = 0;
  double  step     = 0,
          func_var = 0,
          max_var  = 0;
  DList  *Ji       = NULL;

  for (i = 0; i < nb_lin; i++)
    if (!line[i].del)
     { grad[i] = 1.0;
       Ji      = line[i].first;
       while (Ji)
 	{ grad[i] -= (((float) aux_x[Ji->col]) * Ji->coef);
	  Ji       = Ji->right;
        }
       step += (grad[i] * grad[i]);
     }
  func_var = 1.01 * Z_primal - Zd;
  max_var  = 0.04 * Zd;
  if ((func_var > max_var) || (func_var <= 0))
    func_var = max_var;
  step = (step > epsilon) ? ((lambda * func_var) / step) : 0;
  return(step);
}

/* ------------------------------------------------------------------------- */

void AdjustRedCosts(int     lin,
		    double  diff,
		    double *red_cost,
		    double *Zd)
{
  double  delta = 0;
  DList  *Ji    = NULL;

  *Zd += diff;
  Ji = line[lin].first;
  while (Ji)
   { delta = diff * Ji->coef;
     if (red_cost[Ji->col] < delta)
      { *Zd -= delta;
	aux_x[Ji->col] = 1;
      }
     else
       aux_x[Ji->col] = 0;
     red_cost[Ji->col] -= delta;
     Ji = Ji->right;
   }
}

/* ------------------------------------------------------------------------- */

char StopCriterion(float      *lambda,
		   double     *best_Zd,
		   DualType   *DualSol,
		   PrimalType *PrimalSol)
{
  char exec  = TRUE;
  static
  int  iter     = 0,
       max      = 0,
       count    = 0,
       max_iter = 0;

  if (max == 0)
   { max = (nb_lin < nb_col) ? nb_lin : nb_col;
     max_iter = 3 * max;
   }
/*    max = 240;
    max = 2 * nb_lin; */
  if ((DualSol->value - *best_Zd) <= epsilon)
   { if (++count == max)
      { *lambda = (*lambda) * 0.5;
	if (max > 3)
	  max = (int) max / 2;
	count = 0;
      }
   }
  else
    { if (*best_Zd < DualSol->value)
       { *best_Zd = DualSol->value;
	 memcpy(aux_u,DualSol->var_u,nb_lin * sizeof(double));
	 count = 0;
       }
    }
  if ((++iter == max_iter) || (DualSol->value >= PrimalSol->value))
/*  if ((++iter == 500) || (DualSol->value >= PrimalSol->value))
  if ((++iter == (3 * nb_lin)) || (DualSol->value >= PrimalSol->value)) */
   { exec  = FALSE;
     iter  = 0;
     max   = 0;
     count = 0;
   }
  return(exec);
}

/* ------------------------------------------------------------------------- */

void RecalcReducedCosts(DualType *DualSol)
{
  int    j  = 0;
  DList *Ij = NULL;

  for (j = 0; j < nb_col; j++)
   { DualSol->red_cost[j] = column[j].coef;
     Ij                   = column[j].first;
     while (Ij)
      { DualSol->red_cost[j] -= (DualSol->var_u[Ij->lin] * Ij->coef);
	Ij = Ij->down;
      }
   }
}

/* ------------------------------------------------------------------------- */

void RestoreFeasibility(DualType *DualSol)
{
  int    i     = 0,
         j     = 0,
         k     = 0;
  double min   = 0,
         old_u = 0,
         rate  = 0,
         reduc = 0;
  DList *Ij    = NULL,
        *Ji    = NULL,
        *aux   = NULL;

  for (j = 0; j < nb_col; j++)
   { if (DualSol->red_cost[j] >= 0)
       continue;
     rate = column[j].coef / (column[j].coef - DualSol->red_cost[j]);
     Ij   = column[j].first;
     while (Ij)
      { reduc = (1 - rate) * DualSol->var_u[Ij->lin];
	Ji    = line[Ij->lin].first;
	while (Ji)
	 { DualSol->red_cost[Ji->col] += (reduc * Ji->coef);
	   Ji = Ji->right;
         }
	old_u                    = DualSol->var_u[Ij->lin];
	DualSol->var_u[Ij->lin]  = DualSol->var_u[Ij->lin] * rate;
	DualSol->value          += ((DualSol->var_u[Ij->lin] - old_u) *
				    line[Ij->lin].rhs);
	Ij = Ij->down;
      }
   }
  for (i = 0; i < nb_lin; i++)
    if (!line[i].del)
     { Ji  = line[i].first;
       min = DualSol->red_cost[Ji->col];
       aux = Ji;
       while (Ji)
	{ if ((DualSol->red_cost[Ji->col] / Ji->coef) < (min / aux->coef))
	   { min = DualSol->red_cost[Ji->col];
	     aux = Ji;
	   }
	  Ji = Ji->right;
        }
       min = min / aux->coef;
       if (min > epsilon)
	{ DualSol->var_u[i] += min;
	  DualSol->value    += (min * line[i].rhs);
	  Ji = line[i].first;
	  while (Ji)
	   { DualSol->red_cost[Ji->col] -= (min * Ji->coef);
	     Ji = Ji->right;
           }
        }
     }
}

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */
