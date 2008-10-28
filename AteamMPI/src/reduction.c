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
/* Este arquivo contem o codigo necessario para as rotinas de testes de fac- */
/* tibilidade e reducao de tamanho do problema a ser tratado. Esta rotina e' */
/* chamada por todos os agentes do ATeam distribuido para o Set Covering Pro-*/
/* blem.                                                                     */
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */

#include "globais.h"   /* Inclusao das definicoes de tipos, macros e constan-*/
                       /* tes utilizadas em todos os arquivos do A-Team dis- */
                       /* tribuido para o Set Covering Problem.              */

extern int  nb_lin;
extern int  nb_col;
extern Lin *line;
extern Col *column;

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */

void Reduction(int    *var_x,
	       double *Z_primal)
{
  int    i  = 0;
  DList *Ij = NULL,
        *Ji = NULL;

  if (var_x)
    memset(var_x,0,nb_col * sizeof(int));
  for (i = 0; i < nb_lin; i++)
   { if (line[i].elem_lin == 0)
      { printf("\n\n* Agente encerrado *");
	printf("\n* Problema infactivel (linha %i com 0 elementos) *",i+1);
	exit(1);
      }
     if (line[i].elem_lin == 1)
      { if (var_x)
	  var_x[column[line[i].first->col].var] = 1;
	if (Z_primal)
	  *Z_primal += column[line[i].first->col].coef;
	Ij = column[line[i].first->col].first;
	while (Ij)
	 { line[Ij->lin].del = TRUE;
	   Ji = line[Ij->lin].first;
	   while (Ji)
	    { if (Ji->up)
		Ji->up->down = Ji->down;
	      if (Ji->down)
		Ji->down->up = Ji->up;
	      Ji = Ji->right;
	    }
	   Ij = Ij->down;
	 }
      }
   }
}

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */

