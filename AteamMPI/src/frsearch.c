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
/* Este procedimento de busca local esta de  acordo com a referencia abaixo. */
/* O procedimento testa todas as k-tuplas de um recobrimento,  buscando  uma */
/* p-tupla (p < k), a qual nao esteja no recobrimento, de tal modo que ao se */
/* efetuar a troca das tuplas chega-se a um recobrimento de menor custo.  Os */
/* valores dos parametros k e p, nesta implementacao, sao 2 e 1, respectiva- */
/* mente.                                                                    */
/*                                                                           */
/* Referencia : Greedy Randomized Adaptive Search Procedures                 */
/*              Thomas A. Feo and Mauricio G. C. Resende                     */
/*              Journal of Global Optimization, ?, 1-27, (1995)              */
/*                                                                           */
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */

#include "globais.h"   /* Inclusao das definicoes de tipos, macros e constan-*/
                       /* tes utilizadas em todos os arquivos do A-Team dis- */
                       /* tribuido para o Set Covering Problem.              */

extern int  nb_lin;
extern int  nb_col;
extern int *cover;
extern int *uncover;
extern int *aux_x;
extern Lin *line;
extern Col *column;

void FindSolutionCols(int *tot_col,
		      int *var_x);
void SearchNewSolution(char *redun_cols,
		       int   tot_col,
		       int  *i,
		       int  *j,
		       int  *k,
		       int  *var_x);
char SearchChangeColumn(int  nb_uncov,
			int *k,
			int *var_x);

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */

void FeoResendeSearch(PrimalType *PrimalSol)
{
  char redun_cols = FALSE;
  int  i          = 0,
       j          = 0,
       k          = 0,
       tot_col    = 0;

/* printf("\n\n(mem) -> %f",PrimalSol->value); */
  FindSolutionCols(&tot_col,PrimalSol->var_x);
  k = nb_col;
  SearchNewSolution(&redun_cols,tot_col,&i,&j,&k,PrimalSol->var_x);
  if ((k < nb_col) || (i == j))
   { PrimalSol->var_x[column[aux_x[i]].var] = 0;
     if (i != j)
       PrimalSol->var_x[column[aux_x[j]].var] = 0;
     if (redun_cols)
       PrimalSol->value -= 2;
     else
      { if (k < nb_col)
	  PrimalSol->var_x[column[k].var] = 1;
	PrimalSol->value -= 1;
      }
/* printf("\n(sch) => %f\n",PrimalSol->value); */

     PrimalSol->agent[agent_primal]    = FALSE;
     PrimalSol->agent[agent_beasley]   = FALSE;
     PrimalSol->agent[agent_consensus] = FALSE;
     PrimalSol->agent[agent_adjust]    = FALSE;
     PrimalSol->agent[agent_tabu]      = FALSE;
     PrimalSol->agent[agent_search]    = TRUE;
     PrimalSol->agent_ID               = agent_search;
   }
}

/* ------------------------------------------------------------------------- */
/* Este procedimento procura quais sao as colunas que compoem o recobrimento
   (aux_x) e determina quantas vezes cada uma das restricoes  foi  recoberta
   (cover).                                                                  */

void FindSolutionCols(int *tot_col,
		      int *var_x)
{
  int    j  = 0;
  DList *Ij = NULL;

  memset(cover,0,nb_lin * sizeof(int));
  for (j = 0; j < nb_col; j++)
    if (var_x[column[j].var] == 1)
      { Ij = column[j].first;
	while (Ij)
	 { cover[Ij->lin]++;
	   Ij = Ij->down;
	 }
	aux_x[*tot_col] = j;
	(*tot_col)++;
      }
}

/* ------------------------------------------------------------------------- */
/* Este procedimento busca uma k-tupla (k = 2), guardando as linhas  que fi-
   carao descobertas (uncover) e que deverao  ser  recobertas  pela  p-tupla
   (p = 1).                                                                  */

void SearchNewSolution(char *redun_cols,
		       int   tot_col,
		       int  *i,
		       int  *j,
		       int  *k,
		       int  *var_x)
{
  char   i_redun  = TRUE,
         j_redun  = TRUE,
         exec     = TRUE;
  int    lin_i    = 0,
         lin_j    = 0,
         nb_uncov = 0;
  DList *I_i      = NULL,
        *I_j      = NULL;

  *i = -1;
  while ((++(*i) < (tot_col -1)) && (exec))
   { *j = *i;
     while ((++(*j) < tot_col) && (exec))
      { I_i = column[aux_x[*i]].first;
	I_j = column[aux_x[*j]].first;
	while (I_i || I_j)
	 { lin_i = (I_i) ? I_i->lin : nb_lin;
	   lin_j = (I_j) ? I_j->lin : nb_lin;
	   if (lin_i < lin_j)
	    { if (cover[lin_i] == 1)
	       { lin_j = lin_i;
		 i_redun = FALSE;
	       }
	      I_i = I_i->down;
	    }
	   else
	     if (lin_i > lin_j)
	       { if (cover[lin_j] == 1)
		  { lin_i = lin_j;
		    j_redun = FALSE;
		  }
		 I_j = I_j->down;
	       }
	     else
	      { if (cover[lin_i] == 2)
		  i_redun = j_redun = FALSE;
		else
		  lin_i = -1;
		I_i = I_i->down;
		I_j = I_j->down;
	      }
	   if (lin_i == lin_j)
	    { uncover[nb_uncov] = lin_i;
	      nb_uncov++;
	    }
         }
	if (nb_uncov == 0)
	 { *redun_cols = TRUE;
	   exec = FALSE;
	   *k = 0;
         }
	else
	  if (i_redun || j_redun)
	    exec = FALSE;
	  else
	    exec = SearchChangeColumn(nb_uncov,k,var_x);
	nb_uncov = 0;
      }
   }
  --(*i);
  --(*j);
  if (!(*redun_cols))
   { if (i_redun)
       *j = *i;
     if (j_redun)
       *i = *j;
   }
}

/* ------------------------------------------------------------------------- */
/* Este procedimento busca uma coluna que ao ser colocada  no  recobrimento,
   suporte a exclusao de duas outras colunas, ou seja, esta  coluna  recobre
   as linhas que ficaram descobertas ao se excluir duas outras colunas.      */

char SearchChangeColumn(int  nb_uncov,
			int *k,
			int *var_x)
{
  int    i    = 0,
         find = 0;
  DList *Ij   = NULL;

  *k = -1;
  while ((++(*k) < nb_col) && (find < nb_uncov))
    if (var_x[column[*k].var] == 0)
     { find = 0;
       Ij = column[*k].first;
       while ((Ij) && (i < nb_uncov))
	{ if (uncover[i] < Ij->lin)
	    i++;
	  else
	    if (uncover[i] > Ij->lin)
	      Ij = Ij->down;
	    else
	     { find++;
	       i++;
	       Ij = Ij->down;
	     }
        }
     }
  return(*k == nb_col);
}

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */
