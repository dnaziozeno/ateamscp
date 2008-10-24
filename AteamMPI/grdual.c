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
/* Este arquivo contem o codigo necessario para o  agente dual greedy.  Este */
/* agente, atraves de uma heuristica gulosa, gera solucoes duais  randomicas */
/* e as envia para o servidor de memoria de solucoes duais.  A notacao usada */
/* nesta implementacao esta de acordo com a usada na referencia abaixo. A di-*/
/* ferenca basica entre esta implementacao e o especificado na referencia, e'*/
/* que originalmente a ordem pela qual as variaveis duais eram escolhidas pa-*/
/* a construcao de uma solucao era deterministica, e foi alterada para que a */
/* escolha seja randomica.                                                   */
/*                                                                           */
/* Referencia: Optimal Solution of Set Covering/partitioning Problems Using  */
/*             Dual Heuristics.                                              */
/*             Marshall L. Fisher and Pradeep Kedia                          */
/*             Management Science 36 (6), June 1990                          */
/*                                                                           */
/* ------------------------------------------------------------------------- */

#include "globais.h"    /* Inclusao das definicoes de tipos, macros e  cons- */
			/* tantes utilizadas em todos os arquivos  do A-Team */
                        /* distribuido para o Set Covering Problem.          */

extern int     nb_lin;
extern int     nb_col;
extern int    *inact;
extern int    *I_u;
extern double *delta_u;
extern Lin    *line;
extern Col    *column;

void RandomDualGreedy(float     reduc,
		      DualType *DualSol);
void OriginalDualGreedy(float     reduc,
			DualType *DualSol);
void CalcDeltaU(DualType *DualSol);
void RecalcDeltaU(int       best_i,
		  float     reduc,
		  DualType *DualSol);
void RecalcReducedCosts(DualType *DualSol);

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */

void DualGreedy(char      RandomGreedy,
		float     reduc,
		DualType *DualSol)
{
  static
  int tot_heur = 6;
  int heur     = 1;

  DualSol->value = 0;
  memset(DualSol->var_u,0,nb_lin*sizeof(double));
  CalcDeltaU(DualSol);
  if (RandomGreedy)
   { heur = lrand48() % tot_heur;
     if (((tot_heur == 6) && (heur == 5)) || ((tot_heur == 5) && (heur == 4)))
       tot_heur--;
   }
  switch (heur)
   { case 0 : reduc = 0.0;
              while (reduc == 0.0)
		reduc = drand48();
              RandomDualGreedy(reduc,DualSol);
              break;
     case 1 : RandomDualGreedy(reduc,DualSol);
              break;
     case 2 : RandomDualGreedy(1.0,DualSol);
              break;
     case 3 : do
                reduc = drand48();
              while (reduc == 0.0);
              OriginalDualGreedy(reduc,DualSol);
              break;
     case 4 : OriginalDualGreedy(reduc,DualSol);
              break;
     case 5 : OriginalDualGreedy(1.0,DualSol);
   }
/*printf("\n%i (%f) => %f",heur,reduc,DualSol->value);*/
  RecalcReducedCosts(DualSol);
/*printf("\n%i (%f) => %f\n",heur,reduc,DualSol->value);*/

  DualSol->improved_sol         = FALSE;
  DualSol->agent[agent_dual]    = TRUE;
  DualSol->agent[agent_3opt]    = FALSE;
  DualSol->agent[agent_subgrad] = FALSE;
  DualSol->agent[agent_free]    = FALSE;
  DualSol->agent_ID             = agent_dual;
}

/* ------------------------------------------------------------------------- */

void RandomDualGreedy(float     reduc,
		      DualType *DualSol)
{
  int    best_i = 0,
         cont   = 0;
  double delta  = 0;

  while (I_u[nb_lin] > 0)
   { best_i = lrand48() % I_u[nb_lin];
     delta = delta_u[I_u[best_i]] * reduc;
     DualSol->var_u[I_u[best_i]] += delta;
     DualSol->value              += (delta * line[I_u[best_i]].rhs);
     RecalcDeltaU(I_u[best_i],reduc,DualSol);
   }
}

/* ------------------------------------------------------------------------- */

void OriginalDualGreedy(float     reduc,
			DualType *DualSol)
{
  int    i      = 0,
         best_i = 0,
         min_Ji = 0;
  double delta  = 0;

  while (I_u[nb_lin] > 0)
   { min_Ji = MAXINT;
     while (i < I_u[nb_lin])
      { if (!line[I_u[i]].del)
	 { if ((line[I_u[i]].elem_lin < min_Ji) ||
	      ((line[I_u[i]].elem_lin == min_Ji) &&
	      (delta_u[I_u[i]] > delta_u[I_u[best_i]])))
	    { min_Ji = line[i].elem_lin;
	      best_i = i;
	    }
         }
	i++;
      }
     delta = delta_u[I_u[best_i]] * reduc;
     DualSol->var_u[I_u[best_i]] += delta;
     DualSol->value              += (delta * line[I_u[best_i]].rhs);
     RecalcDeltaU(I_u[best_i],reduc,DualSol);
     i = 0;
   }
}

/* ------------------------------------------------------------------------- */

void CalcDeltaU(DualType *DualSol)
{
  int     i         = 0,
          k         = 0;
  double  aux_delta = 0;
  DList  *Ji        = NULL;

  for (i = 0; i < nb_col; i++)
    DualSol->red_cost[i] = column[i].coef;
  for (i = 0; i < nb_lin; i++)
   { if (line[i].del)
      { inact[i] = -1;
	delta_u[i] = 0;
      }
     else
      { I_u[k]     = i;
	inact[i]   = k;
	k++;
/*	delta_u[i] = column[line[i].first->col].coef / line[i].first->coef; */
	delta_u[i] = MAXDOUBLE;
	Ji = line[i].first;
	while (Ji)
	 { aux_delta = column[Ji->col].coef / Ji->coef;
	   if (aux_delta < delta_u[i])
	     delta_u[i] = aux_delta;
	   Ji = Ji->right;
         }
      }
   }
  I_u[nb_lin] = k;
}

/* ------------------------------------------------------------------------- */

void RecalcDeltaU(int       best_i,
		  float     reduc,
		  DualType *DualSol)
{
  int     i         = 0;
  double  delta     = 0,
          aux_delta = 0;
  DList  *Ij        = NULL,
         *Ji        = NULL;

  delta = delta_u[best_i] * reduc;
  Ji    = line[best_i].first;
  while (Ji)
   { DualSol->red_cost[Ji->col] -= (delta * Ji->coef);
     Ij = column[Ji->col].first;
     while (Ij)
      { aux_delta = DualSol->red_cost[Ji->col] / Ji->coef;
        if (aux_delta < delta_u[Ij->lin])
	 { delta_u[Ij->lin] = aux_delta;
	   if (delta_u[Ij->lin] <= epsilon)
	    { i = inact[Ij->lin];
	      if(inact[Ij->lin] >= 0)
	      { 
	        (I_u[nb_lin])--;
	        I_u[i] = I_u[I_u[nb_lin]];
	        if (i < I_u[nb_lin])
		      inact[I_u[i]] = i;
		   }
	    }
     }
	 Ij = Ij->down;
      }
     Ji = Ji->right;
   }
}

/* ------------------------------------------------------------------------- */

void RecalcReducedCosts(DualType *DualSol)
{
  int    i   = 0,
         j   = 0;
  double min = 0;
  DList *Ij  = NULL,
        *Ji  = NULL,
        *aux = NULL;

  for (j = 0; j < nb_col; j++)
   { DualSol->red_cost[j] = column[j].coef;
     Ij                   = column[j].first;
     while (Ij)
      { DualSol->red_cost[j] -= (DualSol->var_u[Ij->lin] * Ij->coef);
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
