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
/* Este arquivo contem as rotinas que fazem o gerenciamento do  conteudo da  */
/* memoria de solucoes primais e as rotinas para impressao de  estatisticas  */
/* da evolucao do ATeam distribuido para o Set Covering Problem.             */
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */


#include "globais.h"   /* Inclusao das definicoes de tipos, macros e constan-*/
                       /* tes utilizadas em todos os arquivos do A-Team dis- */
                       /* tribuido para o Set Covering Problem.              */

extern char        PrimalWorst;
extern char        EqualCostSol;
extern int         nb_lin;
extern int         nb_col;
extern int         nb_primal_sol_mem;
extern int         primal_worst;
extern int         primal_equal;
extern int         MaxLenPrimalMem;
extern int         ProposedSolution[NbPrimalAgents];
extern int         ProposedSucess[NbPrimalAgents];
extern int         SolutionAccept[NbPrimalAgents];
extern int         SolutionAgents[NbPrimalAgents];
extern long        total_time_cron;
extern long        lost_time_cron;
extern float       density;
extern float       MaxSolPrimalAg;
extern double      BestSolution[NbPrimalAgents];
extern FILE       *fstts;
extern FILE       *fdump;
extern PrimalType *PrimalMem;

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */

void LocalTime(long *cron_time)
{
  struct tm      day;
  struct timeval now;

//HHHH  gettimeofday(&now);
  gettimeofday(&now,NULL);
  day        = *localtime(&(now.tv_sec));
  *cron_time = (long) (day.tm_sec + (60 * day.tm_min) + (3600 * day.tm_hour));
}

/* ------------------------------------------------------------------------- */

void InitAteamParam(int MaxLenPrimalMem_i, int MaxSolPrimalAg_i, int EqualCostSol_i)
{
  int hour = 0,
      min  = 0,
      sec  = 0;

  MaxLenPrimalMem = (int)    MaxLenPrimalMem_i;
  MaxSolPrimalAg  = ((float) MaxSolPrimalAg_i / 100);
  EqualCostSol    = (char)   EqualCostSol_i;

  PrimalMem = (PrimalType *) malloc(MaxLenPrimalMem * sizeof(PrimalType));
  LocalTime(&lost_time_cron);
}

/* ------------------------------------------------------------------------- */

void OpenFileStatistics(char *path)
{
  char path_dump[100];
  char path_stts[100];

  strcpy(path_dump,path);
  strcpy(path_stts,path);
  strcat(path_dump,"_MP_sol");
  strcat(path_stts,"_MP_est");
  fdump = fopen(path_dump,"w");
  fstts = fopen(path_stts,"w");

  fprintf(fstts,"O arquivo contem ''fotos'' da memoria de solucoes primais");
  fprintf(fstts,"\nTempo    -> Tempo entre o inicio do A-Team e a foto");
  fprintf(fstts,"\nTHeur    -> Tempo de execucao de cada uma das heuristica");
  fprintf(fstts,"\nProp     -> Nr de sol. primais propostas a memoria");
  fprintf(fstts,"\nAcei     -> Nr de sol. primais prop. e aceitas pela mem");
  fprintf(fstts,"\nIgual    -> Nr de sol. primais prop. nao aceitas iguais");
  fprintf(fstts,"\nPior     -> Nr de sol. primais prop. pior que pior da mem");
  fprintf(fstts,"\n%%Pior    -> Taxa de proposicao de sol. duais piores");
  fprintf(fstts,"\n%%Acei    -> Taxa de aceitacao das sol. duais propostas");
  fprintf(fstts,"\nSolPrimal-> Valor do melhor solucao primal encontrada");
  fprintf(fstts,"\n\n            Numero de Linhas   = %5d ",nb_lin);
  fprintf(fstts,"\n            Numero de Colunas  = %5d ",nb_col);
  fprintf(fstts,"\n            Densidade          = %5.2f ",density);
  fprintf(fstts,"\n            Tamanho Mem Primal = %5d \n", MaxLenPrimalMem);
  fprintf(fstts,"\n--------------------------------------------------------------------------------------------------------------------------------------------");
  fprintf(fstts,"\n Tempo  Theur   Prop   Acei  Igual   Pior  %%Pior  %%Acei    SolPrimal      GPrimal    %%   RandAdjust    %%    LocSearch    %%     Consenso    %%");
  fflush(fstts);

  memset(ProposedSolution,0,NbPrimalAgents * sizeof(int));
  memset(ProposedSucess,0,NbPrimalAgents * sizeof(int));
  memset(SolutionAccept,0,NbPrimalAgents * sizeof(int));
  memset(SolutionAgents,0,NbPrimalAgents * sizeof(int));
}

/* ------------------------------------------------------------------------- */

void WriteStatistics(int pos)
{
  int    i               = 0,
         hour            = 0,
         min             = 0,
         sec             = 0,
         primal_accept   = 0,
         primal_proposed = 0;
  float  primal_rate     = 0,
         adjust_rate     = 0,
         search_rate     = 0,
         consen_rate     = 0,
         worst_rate      = 0,
         accept_rate     = 0;
  double best_sol[NbPrimalAgents];

  LocalTime(&total_time_cron);
  if (total_time_cron < lost_time_cron)
    (total_time_cron += 86400);
  total_time_cron -= lost_time_cron;
  for (i = 0; i < NbPrimalAgents; i++)
    best_sol[i] = (BestSolution[i] == MAXDOUBLE) ? 0 : BestSolution[i];

  primal_proposed  = ProposedSolution[agent_primal - NbDualAgents] +
                     ProposedSolution[agent_adjust - NbDualAgents] +
                     ProposedSolution[agent_search - NbDualAgents] +
                     ProposedSolution[agent_consensus - NbDualAgents];
  primal_accept    = primal_proposed - primal_equal - primal_worst;
  primal_rate = ((float) SolutionAccept[agent_primal-NbDualAgents] / MaxLenPrimalMem) * 100;
  adjust_rate = ((float) SolutionAccept[agent_adjust-NbDualAgents] / MaxLenPrimalMem) * 100;
  search_rate = ((float) SolutionAccept[agent_search-NbDualAgents] / MaxLenPrimalMem) * 100;
  consen_rate = ((float) SolutionAccept[agent_consensus-NbDualAgents] / MaxLenPrimalMem) * 100;
  worst_rate  = ((float) primal_worst / primal_proposed) * 100;
  accept_rate = ((float) primal_accept / primal_proposed) * 100;

  fprintf(fstts,"\n%6d %6d %6d %6d %6d %6d  %5.1f  %5.1f%13.3f%13.3f%5.1f%13.3f%5.1f%13.3f%5.1f%13.3f%5.1f",total_time_cron, PrimalMem[pos].proc_time, primal_proposed, primal_accept, primal_equal, primal_worst, worst_rate, accept_rate, PrimalMem[0].value, best_sol[agent_primal - NbDualAgents], primal_rate, best_sol[agent_adjust - NbDualAgents], adjust_rate, best_sol[agent_search - NbDualAgents], search_rate, best_sol[agent_consensus - NbDualAgents], consen_rate);
  fflush(fstts);
}

/* ------------------------------------------------------------------------- */

int ChoosePosition(int probability,
		   int mem_size)
{
  int pos = 0;

  switch (probability)
   { case LinearIncr : pos = (int)(mem_size * sqrt(drand48()));
                       break;
     case LinearDecr : pos = (int)((mem_size - 1) * (1 - sqrt(drand48())));
                       break;
     case Constant   : pos = (int)(mem_size * drand48());
   }
  return(pos);
}

/* ------------------------------------------------------------------------- */

int PrimalLeftRealign(int         pos,
		      PrimalType *PrimalSol)
{
  while (pos > 0)
    if (PrimalSol->value < PrimalMem[pos-1].value)
     { PrimalMem[pos] = (PrimalType) PrimalMem[pos-1];
       pos--;
     }
    else
      break;
  return(pos);
}

/* ------------------------------------------------------------------------- */

int PrimalRightRealign(int         pos,
		       PrimalType *PrimalSol)
{
  while (pos < nb_primal_sol_mem-1)
    if (PrimalSol->value > PrimalMem[pos+1].value)
     { PrimalMem[pos] = (PrimalType) PrimalMem[pos+1];
       pos++;
     }
    else
      return(pos);
}

/* ------------------------------------------------------------------------- */

void FindPosInsertion(int        *pos,
		      PrimalType *PrimalSol)
{
  if (nb_primal_sol_mem < MaxLenPrimalMem)
   { *pos = PrimalLeftRealign(nb_primal_sol_mem,PrimalSol);
     nb_primal_sol_mem++;
   }
  else
   { *pos = 1 + ChoosePosition(LinearIncr,nb_primal_sol_mem - 1);
     free(PrimalMem[*pos].var_x);
     SolutionAccept[PrimalMem[*pos].agent_ID - NbDualAgents]--;
     if (PrimalMem[*pos].value > PrimalSol->value)
       *pos = PrimalLeftRealign(*pos,PrimalSol);
     else
       *pos = PrimalRightRealign(*pos,PrimalSol);
   }
}

/* ------------------------------------------------------------------------- */

char RefuseInsertion(PrimalType *PrimalSol)
{
  char refuse = FALSE;
  int  i      = 0;

  i = 0;
  while ((i < nb_primal_sol_mem) &&
	 (PrimalSol->value > (PrimalMem[i].value + epsilon)))
    i++;
  if (i < nb_primal_sol_mem)
   { if (EqualCostSol)
      { while ((i < nb_primal_sol_mem) &&
	       (PrimalSol->value == PrimalMem[i].value))
	 { if (!memcmp((char *) PrimalSol->var_x,
			(char *) PrimalMem[i].var_x,nb_col * sizeof(int)))
	    { primal_equal++;
	      refuse = TRUE;
	      i      = nb_primal_sol_mem;
	    }
	   i++;
         }
      }
     else
       if ((PrimalSol->value > (PrimalMem[i].value - epsilon)) &&
	   (PrimalSol->value < (PrimalMem[i].value + epsilon)))
	{ primal_equal++;
	  refuse = TRUE;
        }
   }
  else
    if ((!PrimalWorst) && (nb_primal_sol_mem > 0) &&
	(PrimalSol->value > PrimalMem[nb_primal_sol_mem-1].value))
     { primal_worst++;
       refuse = TRUE;
     }
  if (refuse)
    free(PrimalSol->var_x);
  return(refuse);
}

/* ------------------------------------------------------------------------- */

void InsertPrimalMem(char       *sleep_ag,
		     PrimalType *PrimalSol)
{
  int pos = 0;

  ProposedSolution[PrimalSol->agent_ID - NbDualAgents]++;
  if (!RefuseInsertion(PrimalSol))
   { SolutionAgents[PrimalSol->agent_ID - NbDualAgents]++;
     SolutionAccept[PrimalSol->agent_ID - NbDualAgents]++;
     FindPosInsertion(&pos,PrimalSol);
     PrimalMem[pos] = (PrimalType) *PrimalSol;
     if (PrimalSol->value < BestSolution[PrimalSol->agent_ID - NbDualAgents])
       { BestSolution[PrimalSol->agent_ID - NbDualAgents] = PrimalSol->value;
	 ProposedSucess[PrimalSol->agent_ID - NbDualAgents]++;
	 WriteStatistics(pos);
       }
     pos       = PrimalSol->agent_ID - NbDualAgents;
     *sleep_ag = (SolutionAccept[pos] > (MaxLenPrimalMem * MaxSolPrimalAg));
   }
}

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */
