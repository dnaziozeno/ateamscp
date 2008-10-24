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
/*  Algoritmo heuristico para o problema de recobrimento de conjuntos com    */
/*  custo nao unitario baseado na relaxacao Lagrangeana e otimizacao  por    */
/*  subgradiente.                                                            */
/*                                                                           */
/*  Referencia: A Lagrangian Heuristic for Set-Covering Problems             */
/*              J. E. Beasley                                                */
/*              Naval Research Logistics, Vol 37, pp 151-164 (1990)          */
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
extern int    *S;
extern int    *G;
extern int    *X;
extern double *C;
extern double *P;
extern double *t;
extern Lin    *line;
extern Col    *column;

void Step_01(double   *Z_lb,
	     DualType *DualSol);
void Step_02(int      *iter,
	     double   *S_cost,
	     double   *Z_lb,
	     DualType *DualSol);
void Step_03(double     *S_cost,
	     PrimalType *PrimalSol);
void Step_05(double      Z_lb,
	     PrimalType *PrimalSol);
char Step_06_07(double *sum_Gi);
char Step_08_09(int    *iter,
		double *f,
		double *T,
		double *Z_ub,
		double *Z_lb,
		double *sum_Gi);
void Step_10(double  T,
	     double *Z_lb);


/* ----------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ----------------------------------------------------------------------- */

void BeasleyHeur(DualType   *DualSol,
		 PrimalType *PrimalSol)
/*void BeasleyHeur(double      lower_bound,
		 PrimalType *PrimalSol) */
{
  int    iter   = 0;
  double T      = 0,
         new_t  = 0,
         f      = 2,
         S_cost = 0,
         sum_Gi = 0,
         Z_lb   = 0;

printf("\n%f -> %f\n",DualSol->value,PrimalSol->value);
  Step_01(&Z_lb,DualSol);
printf("\n%f\n",Z_lb);
  while (TRUE)
   { Step_02(&iter,&S_cost,&Z_lb,DualSol);
printf("(%f,%f) -> ",PrimalSol->value,Z_lb);
     Step_03(&S_cost,PrimalSol);
printf("(%f) : ",PrimalSol->value);
     if ((PrimalSol->value - DualSol->value) <= epsilon); /* Step 04 */
{ printf("SAIU 1");
       break;
}     Step_05(Z_lb,PrimalSol);
     if (Step_06_07(&sum_Gi))
{ printf("SAIU 2");
       break;
}     if (Step_08_09(&iter,&f,&T,&PrimalSol->value,&Z_lb,&sum_Gi))
{ printf("SAIU 3");
       break;
}     Step_10(T,&Z_lb);
   }
printf("\n%f\n",PrimalSol->value);

  PrimalSol->agent[agent_primal]    = FALSE;
  PrimalSol->agent[agent_beasley]   = TRUE;
  PrimalSol->agent[agent_consensus] = FALSE;
  PrimalSol->agent[agent_adjust]    = FALSE;
  PrimalSol->agent[agent_tabu]      = FALSE;
  PrimalSol->agent[agent_search]    = FALSE;
  PrimalSol->agent_ID               = agent_beasley;
}

/* ------------------------------------------------------------------------- */
/* PASSO 1 : Inicializacao das estruturas de dados do problema.              */
void Step_01(double   *Z_lb,
	     DualType *DualSol)
{
  int i = 0;

  for (i = 0; i < nb_col; i++)
    P[i] = column[i].coef;
  for (i = 0; i < nb_lin; i++)
   { t[i]  = DualSol->var_u[i];
     *Z_lb = DualSol->value;
   }
}


/* ------------------------------------------------------------------------- */
/* PASSO 2 : Resolucao do Lagrangian Lower Bound Problem.                    */
void Step_02(int      *iter,
	     double   *S_cost,
	     double   *Z_lb,
	     DualType *DualSol)
{
  int    j  = 0;
  DList *Ij = NULL;

  memset(X,0,nb_col * sizeof(int));
  memset(S,0,nb_col * sizeof(int));
  *S_cost;
  for (j = 0; j < nb_col; j++)
   { C[j] = column[j].coef;
     Ij = column[j].first;
     while (Ij)
      { C[j] -= t[Ij->lin];
	Ij    = Ij->down;
      }
     if (C[j] <= 0)
      { S[column[j].var] = X[column[j].var] = 1;
	*S_cost += column[j].coef;
	*Z_lb   += C[j];
      }
   }
  if (DualSol->value < *Z_lb)
   { DualSol->value = *Z_lb;
     memcpy(DualSol->var_u,t,nb_lin * sizeof(double));
     *iter = 0;
   }
}


/* ------------------------------------------------------------------------- */
/* PASSO 3 : Construcao de uma solucao factivel S=var_x para o problema SCP. */
void Step_03(double     *S_cost,
	     PrimalType *PrimalSol)
{
  char   line_covered = FALSE,
         column_cover = FALSE;
  int    i            = 0,
         j            = 0;
  DList *Ji           = NULL,
        *Ij           = NULL;

  for (i = 0; i < nb_lin; i++)
   { line_covered = FALSE;
     Ji = line[i].first;
     while (Ji)
      { if (S[column[Ji->col].var] == 1)
	 { line_covered = TRUE;
	   break;
         }
	Ji = Ji->right;
      }
     if (!line_covered)
      { S[column[line[i].first->col].var] = 1;
	*S_cost += column[line[i].first->col].coef;
      }
   }
  for (j = nb_col-1; j >= 0; j--)
   { if (S[column[j].var] == 0)
       continue;
     column_cover = FALSE;
     Ij = column[j].first;
     while (Ij)
      { line_covered = FALSE;
	Ji = line[Ij->lin].first;
	while (Ji)
	 { if ((S[column[Ji->col].var] == 1) && (Ji->col != j))
	    { line_covered = TRUE;
	      break;
	    }
	   Ji = Ji->right;
         }
	if (!line_covered)
	 { column_cover = TRUE;
	   break;
         }
	Ij = Ij->down;
      }
     if (!column_cover)
      { S[column[j].var] = 0;
	*S_cost -= column[j].coef;
      }
   }
  if (PrimalSol->value > *S_cost)
   { PrimalSol->value = *S_cost;
     memcpy(PrimalSol->var_x,S,nb_col * sizeof(int));
   }
}


/* ------------------------------------------------------------------------- */
/* PASSO 5 : Atualizacao dos valores de P e exclusao de colunas.             */
void Step_05(double      Z_lb,
	     PrimalType *PrimalSol)
{
  int j = 0;

  for (j = 0; j < nb_col; j++)
   { if (column[j].coef == MAXDOUBLE)
       continue;
     if (X[column[j].var] == 1)
      { if (P[j] < Z_lb)
	  P[j] = Z_lb;
      }
     else
       if (P[j] < (Z_lb + C[j]))
	 P[j] = (Z_lb + C[j]);
     if (P[j] > PrimalSol->value)
      { column[j].coef = MAXDOUBLE;
	X[column[j].var] = 0;
      }
   }
}

/* ------------------------------------------------------------------------- */
/* PASSO 6 : Calculo dos subgradientes Gi.                                   */
/* PASSO 7 : Teste da segunda condicao de parada da execucao.                */
char Step_06_07(double *sum_Gi)
{
  int    i  = 0;
  DList *Ji = NULL;

  *sum_Gi = 0;
  for (i = 0; i < nb_lin; i++)
   { G[i] = 1;
     Ji = line[i].first;
     while (Ji)
      { G[i] -= X[column[Ji->col].var];
	Ji    = Ji->right;
      }
     if ((t[i] == 0) && (G[i] < 0))
       G[i] = 0;
     *sum_Gi += G[i] * G[i];
   }
  return (sum_Gi == 0);
}


/* ------------------------------------------------------------------------- */
/* PASSO 8 : Calculo do tamanho do passo da otimizacao por subgradiente.     */
/* PASSO 9 : Teste da terceira condicao de parada da execucao.               */
char Step_08_09(int    *iter,
		double *f,
		double *T,
		double *Z_ub,
		double *Z_lb,
		double *sum_Gi)
{
  if (++(*iter) == 30)
   { *iter = 0;
     *f = *f / 2;
   }
  *T = (*f) * ((1.05 * (*Z_ub)) - (*Z_lb)) / *sum_Gi;
  return (*f <= 0.005);
}


/* ------------------------------------------------------------------------- */
/* PASSO 10 : Atualizacao dos multiplicadores de Lagrange t.                 */
void Step_10(double  T,
	     double *Z_lb)
{
  int    i     = 0;
  double new_t = 0;

  *Z_lb = 0;
  for (i = 0; i < nb_lin; i++)
   { new_t = t[i] + (T * G[i]);
     t[i]  = (new_t > 0) ? new_t : 0;
     *Z_lb += t[i];
   }
}

/* ------------------------------------------------------------------------- */
