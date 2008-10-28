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
/* Este arquivo contem o codigo necessario para a rotina de leitura do dados */
/* que compoem o problema a ser tratado. Esta rotina e' chamada por todos os */
/* agentes do ATeam distribuido para o Set Covering Problem.                 */
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */

#include "globais.h"   /* Inclusao das definicoes de tipos, macros e constan-*/
                       /* tes utilizadas em todos os arquivos do A-Team dis- */
                       /* tribuido para o Set Covering Problem.              */

extern char   unicost;
extern int    nb_lin;
extern int    nb_col;
extern int    max_lin;
extern int    CutsofSol;
extern long   lost_time_cron;
extern float  density;
extern FILE  *finput;
extern Lin   *line;
extern Col   *column;

int Compare(const void *a,
            const void *b);

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */

void ReadSource()
{
  int    i     = 0,
         j     = 0,
         count = 0;
  DList *Ij    = NULL;

  fscanf(finput,"%d%d",&nb_lin,&nb_col);
  max_lin = nb_lin + CutsofSol;
  line    = (Lin *) malloc(max_lin * sizeof(Lin));
  column  = (Col *) malloc(nb_col * sizeof(Col));
  memset(line,0,max_lin *sizeof(Lin));
  memset(column,0,nb_col*sizeof(Col));

  for (i=0; i < nb_col; i++)
   { column[i].var = i;
     fscanf(finput,"%le %d",&column[i].coef,&column[i].elem_col);
     if (column[i].coef != 1.0)
       unicost = FALSE;
     count += column[i].elem_col;
     for (j=0; j < column[i].elem_col; j++)
      { Ij = (DList *) malloc(sizeof(DList));
	fscanf(finput,"%d",&Ij->lin);
	if (Ij->lin > nb_lin)
	 { printf("\n\n* Agente encerrado *");
	   printf("\n* ERRO: Coluna com nr. de linha maior que %i *\n",nb_lin);
	   exit(1);
	 }
        Ij->lin--;
	Ij->coef = 1;
        Ij->down = NULL;
	if (column[i].first)
	  column[i].last->down = Ij;
	else
	  column[i].first = Ij;
	Ij->up = column[i].last;
        column[i].last = Ij;
      }
   }

  if (!unicost)
    qsort((void *)column,nb_col,sizeof(Col),Compare);

  for (i=0; i < nb_col; i++)
   { Ij = column[i].first;
     while (Ij)
      { Ij->col = i;
	line[Ij->lin].rhs = 1;
	line[Ij->lin].elem_lin++;
        if (line[Ij->lin].first)
	  line[Ij->lin].last->right = Ij;
	else
	  line[Ij->lin].first = Ij;
	Ij->right = NULL;
	line[Ij->lin].last = Ij;
	Ij = Ij->down;
      }
   }
  density = (count * 100) / (nb_lin * nb_col);
  LocalTime(&lost_time_cron);
}

/* ------------------------------------------------------------------------- */

int Compare(const void *a,
            const void *b)
{
  int    result = 0;
  double aux    = 0;
  
  aux = ((Col *)a)->coef - ((Col *)b)->coef;
  if (aux < 0)
    result = -1;
  else
    if (aux > 0)
      result = 1;
  return(result);
}

void FreeSource()
{
  int    i   = 0;
  DList *Ij  = NULL,
        *aux = NULL;

  for (i=0; i < nb_col; i++)
   {
     Ij = column[i].first;
     while (Ij)
      {
        aux = Ij;
        Ij  = Ij->down;
        free((DList *) aux);
      }
   }

  free((Lin *) line);
  free((Col *) column);
}

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */
