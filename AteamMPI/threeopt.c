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
/* Este arquivo contem o codigo necessario para o agente threeopt.  Este  a- */
/* gente requisita ao servidor de memoria dual uma solucao, e busca a melho- */
/* ria dessa solucao atraves da busca de tres variaveis, que permitam que  o */
/* valor da funcao objetivo cresca ao se somar/subtrair um certo valor delta */
/* a essas variaveis. Esta heuristica foi proposta na referencia abaixo:     */
/*                                                                           */
/* Referencia : Optimal Solution of Set Covering/Partitioning Problems Using */
/*              Dual Heuristics                                              */
/*              Marshall L. Fisher and Pradeep Kedia                         */
/*              Management Science 36 (6), June 1990                         */
/*                                                                           */
/* ------------------------------------------------------------------------- */
 
/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */

#include "globais.h"    /* Inclusao das definicoes de tipos, macros e  cons- */
			/* tantes utilizadas em todos os arquivos  do A-Team */
                        /* distribuido para o Set Covering Problem.          */

extern char *exclude;
extern int   nb_col;
extern int   nb_lin;
extern int  *disj;
extern int  *set1;
extern int  *set2;
extern Lin  *line;
extern Col  *column;

char Condition_ii(int       i1,
		  int       i2,
		  int       i3,
		  DualType *DualSol);
char Condition_iii();
void IncreaseVarU(int       i1,
		  int       i2,
		  int       i3,
		  double   *delta,
		  DualType *DualSol);
void ActiveInactConstr(int       i1,
		       int       i2,
		       int       i3,
		       double    delta,
		       DualType *DualSol);
void CalcDisjunctionSize(DualType *DualSol);

/* ----------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ----------------------------------------------------------------------- */

void ThreeOpt(DualType *DualSol)
{
  char   exec  = TRUE,
         loop  = TRUE;
  int    i1    = 0,
         i2    = 0,
         i3    = 0;
  double delta = 0;

printf("\n\ncut = %i",DualSol->nb_cuts);
printf("\ngrd = %f",DualSol->value);
  CalcDisjunctionSize(DualSol);
  while (exec)
   { exec = FALSE;
     i1   = -1;
     while (++i1 < nb_lin)
      { if (DualSol->var_u[i1] <= epsilon)   /* Condition (i) */
	  continue;
	i2 = -1;
	while ((++i2 < (nb_lin - 1)) && (loop))
	 { if ((i2 == i1) || (line[i2].del))
	     continue;
/*	   i3 = i2; */
	   i3 = -1;
	   while ((++i3 < nb_lin) && (loop))
/*	    { if (i3 == i1) */
	    { if ((i3 == i1) || (i3 == i2) || (line[i3].del))
		continue;
	      if ((disj[i2] + disj[i3]) <= disj[i1])
		if (Condition_ii(i1,i2,i3,DualSol))
		  if (Condition_iii())
		   { IncreaseVarU(i1,i2,i3,&delta,DualSol);
		     ActiveInactConstr(i1,i2,i3,delta,DualSol);
		     exec = TRUE;
		     loop = (DualSol->var_u[i1] > epsilon);
		   }
	    }
         }
	loop = TRUE;
      }
   }
printf("\n3op = %f\n",DualSol->value);

  DualSol->improved_sol         = TRUE;
  DualSol->agent[agent_dual]    = FALSE;
  DualSol->agent[agent_3opt]    = FALSE;
  DualSol->agent[agent_subgrad] = FALSE;
  DualSol->agent[agent_free]    = FALSE;
  DualSol->agent_ID             = agent_3opt;
}

/* ----------------------------------------------------------------------- */

char Condition_ii(int       i1,
		  int       i2,
		  int       i3,
		  DualType *DualSol)
{
  typedef struct 
   { int   *set;
     DList *J;
   } Actions;
  
  int      i    = -1;
  DList   *J_i1 = NULL;
  Actions  x[2];

  set1[nb_col] = set2[nb_col] = 0;
  x[0]         = (Actions) {set1,line[i2].first};
  x[1]         = (Actions) {set2,line[i3].first};
  J_i1         = line[i1].first;
  while (J_i1)
   { while (++i < 2)
       while (x[i].J)
	{ if (DualSol->red_cost[x[i].J->col] <= epsilon)
	    if (x[i].J->col < J_i1->col)
	      return (FALSE);
	    else
	     { if (x[i].J->col >= J_i1->col)
		 break;
	       x[i].set[x[i].set[nb_col]] = x[i].J->col;
	       (x[i].set[nb_col])++;
	     }
	  x[i].J = x[i].J->right;
        }
     if ((!x[0].J) && (!x[1].J))
       return (TRUE);
     i    = -1;
     J_i1 = J_i1->right;
   }
  while (++i < 2)
    while (x[i].J)
     { if (DualSol->red_cost[x[i].J->col] <= epsilon)
	 return (FALSE);
       x[i].J = x[i].J->right;
     }
  return(TRUE);
}

/* ----------------------------------------------------------------------- */

char Condition_iii()
{
  int i = 0,
      j = 0;

  while ((i < set1[nb_col]) && (j < set2[nb_col]))
   { if (set1[i] == set2[j])
       return (FALSE);
     else
       if (set1[i] < set2[j])
	 ++i;
       else
	 ++j;
   }
  return(TRUE);
}

/* ----------------------------------------------------------------------- */

void IncreaseVarU(int       i1,
		  int       i2,
		  int       i3,
		  double   *delta,
		  DualType *DualSol)
{
  int     a   = 0,
          j   = 0;
  double  min = MAXDOUBLE;
  DList  *Ij  = NULL;

  for (j = 0; j < nb_col; j++)
   { if (DualSol->red_cost[j] <= epsilon)
       continue;
     a = 0;
     Ij = column[j].first;
     while (Ij)
      { if ((Ij->lin < i1) && (Ij->lin < i2) && (Ij->lin < i3))
	 { Ij = Ij->down;
	   continue;
	 }
	if ((Ij->lin > i1) && (Ij->lin > i2) && (Ij->lin > i3))
	  break;
	if (Ij->lin == i1)
	  a--;
	else
	  if ((Ij->lin == i2) || (Ij->lin == i3))
	    a++;
	Ij = Ij->down;
      }
     if (a > 0)
      { *delta = DualSol->red_cost[j] / a;
        if (*delta < min)
          min = *delta;
      }
   }
  *delta = (min < DualSol->var_u[i1]) ? min : DualSol->var_u[i1];
  if (*delta > epsilon)
   { DualSol->var_u[i1] -= *delta;
     DualSol->var_u[i2] += *delta;
     DualSol->var_u[i3] += *delta;
     DualSol->value     += *delta;
   }
}

/* ----------------------------------------------------------------------- */

void ActiveInactConstr(int       i1,
		       int       i2,
		       int       i3,
		       double    delta,
		       DualType *DualSol)
{
  int    k    = -1,
         i[2] = {0,0};
  DList *Ij   = NULL,
        *Ji   = NULL;

  Ji = line[i1].first;
  while (Ji)
   { if (DualSol->red_cost[Ji->col] <= epsilon)
      { exclude[Ji->col] = FALSE;
	Ij = column[Ji->col].first;
	while (Ij)
	 { (disj[Ij->lin])--;
	   Ij = Ij->down;
         }
      }
     DualSol->red_cost[Ji->col] += delta;
     Ji = Ji->right;
   }
  i[0] = i2;
  i[1] = i3;
  while (++k < 2)
   { Ji = line[i[k]].first;
     while (Ji)
      { DualSol->red_cost[Ji->col] -= delta;
        if ((DualSol->red_cost[Ji->col] <= epsilon) && (!exclude[Ji->col]))
	 { exclude[Ji->col] = TRUE;
	   Ij = column[Ji->col].first;
	   while (Ij)
	    { (disj[Ij->lin])++;
	      Ij = Ij->down;
	    }
         }
        Ji = Ji->right;
      }
   }
}

/* ----------------------------------------------------------------------- */

void CalcDisjunctionSize(DualType *DualSol)
{
  int    j  = 0;
  DList *Ij = NULL;
  
  memset(disj,0,nb_lin * sizeof(int));
  for (j = 0; j < nb_col; j++)
    if (DualSol->red_cost[j] > epsilon)
      exclude[j] = FALSE;
    else
     { exclude[j] = TRUE;
       Ij = column[j].first;
       while (Ij)
        { (disj[Ij->lin])++;
	  Ij = Ij->down;
        }
     }
}

/* ----------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ----------------------------------------------------------------------- */
