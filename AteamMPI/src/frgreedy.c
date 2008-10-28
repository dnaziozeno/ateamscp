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
/* a notacao usada esta de acordo com a notacao usada na referencia-1. A i-  */
/* novacao desta heuristica e' que a escolha de uma coluna, para  compor um  */
/* recobrimento, e' feita dentre aquelas que recobrem um  numero  minimo de  */
/* linhas e nao apenas dentre as que recobrem o maximo de linhas.            */
/*                                                                           */
/* Referencia-1  : Greedy Randomized Adaptive Search Procedure               */
/*                 Thomas A. Feo and Mauricio G. C. Resende                  */
/*                 Journal of Global Optimization, ?, 1-27 (1995)            */
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
extern int *unsat;
extern int *cardin;
extern int *sets;
extern Lin *line;
extern Col *column;

void CalcSetCardinality(int  nb_sets,
			int *max_cardin,
			int *var_x);
void ApplyValueRestriction(int    max_cardin,
			   int   *var_x,
			   float  alpha);
void RecalcSetCardinality(int  pos,
			  int *nb_sets,
			  int *max_cardin);
void SearchRedunColumns(PrimalType *PrimalSol);

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */

void FeoResendeGreedy(float       alpha,
		      PrimalType *PrimalSol)
{
  int j          = 0,
      k          = 0,
      max_cardin = 0,
      nb_sets    = 0;

/* printf("\n(mem) = %f",PrimalSol->value); */
  nb_sets = nb_col - ((int) PrimalSol->value);
  CalcSetCardinality(nb_sets,&max_cardin,PrimalSol->var_x);
  while (nb_sets > 0)
   { ApplyValueRestriction(max_cardin,PrimalSol->var_x,alpha);
     k = sets[lrand48() % sets[nb_col]];
     PrimalSol->var_x[column[k].var] = 1;
     PrimalSol->value += 1;
     RecalcSetCardinality(k,&nb_sets,&max_cardin);
   }
/* printf("\n(grd) = %f\n",PrimalSol->value); */

  PrimalSol->agent[agent_primal]    = TRUE;
  PrimalSol->agent[agent_beasley]   = FALSE;
  PrimalSol->agent[agent_consensus] = FALSE;
  PrimalSol->agent[agent_adjust]    = FALSE;
  PrimalSol->agent[agent_tabu]      = FALSE;
  PrimalSol->agent[agent_search]    = FALSE;
  PrimalSol->agent_ID               = agent_primal;
}

/* ------------------------------------------------------------------------- */
/* Este Procedimento calula o numero de conjunto que estao disponiveis  para
   comporem um recobrimento e a cardinalidade desses conjuntos,  ou seja,  o
   numero de restricoes, ainda nao recobertas, que cada uma pode recobrir.   */

void CalcSetCardinality(int  nb_sets,
			int *max_cardin,
			int *var_x)
{
  int    j  = 0;
  DList *Ji = NULL,
        *Ij = NULL;

  memset(unsat,TRUE,nb_lin * sizeof(int));
  for(j = 0; j < nb_col; j++)
    cardin[j] = column[j].elem_col;
  if (nb_sets < nb_col)
    for (j = 0; j < nb_col; j++)
      if (var_x[column[j].var] == 1)
       { Ij = column[j].first;
	 while (Ij)
	  { if (unsat[Ij->lin])
	     { unsat[Ij->lin] = FALSE;
	       Ji = line[Ij->lin].first;
	       while (Ji)
		{ cardin[Ji->col]--;
		  Ji = Ji->right;
	        }
	     }
	    Ij = Ij->down;
	  }
       }
  for(j = 0; j < nb_col; j++)
    if (cardin[j] > *max_cardin)
      *max_cardin = cardin[j];
}

/* ------------------------------------------------------------------------- */
/* Este procedimento determina os conjuntos (sets) que recobrem um nr. mini-
   mo de restricoes (alpha).                                                 */

void ApplyValueRestriction(int    max_cardin,
			   int   *var_x,
			   float  alpha)
{
  int j        = 0,
      min_size = 0;

  sets[nb_col] = 0;
  min_size = (int) floor(alpha * ((float) max_cardin));
  for (j = 0; j < nb_col; j++)
   { if (cardin[j] > min_size)
      { sets[sets[nb_col]] = j;
	(sets[nb_col])++;
      }
   }
}

/* ------------------------------------------------------------------------- */

void RecalcSetCardinality(int  pos,
			  int *nb_sets,
			  int *max_cardin)
{
  int    j  = 0;
  DList *Ij = NULL,
        *Ji = NULL;

  Ij = column[pos].first;
  while (Ij)
   { if (unsat[Ij->lin])
      { unsat[Ij->lin] = FALSE;
	Ji = line[Ij->lin].first;
	while (Ji)
	 { cardin[Ji->col]--;
	   Ji = Ji->right;
         }
      }
     Ij = Ij->down;
   }
  *max_cardin = 0;
  *nb_sets    = 0;
  for(j = 0; j < nb_col; j++)
    { if (cardin[j] > 0)
	(*nb_sets)++;
      if (cardin[j] > *max_cardin)
	*max_cardin = cardin[j];
    }
}

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */
