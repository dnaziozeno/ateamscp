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
/* memoria de solucoes duais e as rotinas para impressao de estatisticas da  */
/* evolucao do ATeam distribuido para o Set Covering Problem.                */
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */


#include "globais.h"   /* Inclusao das definicoes de tipos, macros e constan-*/
                       /* tes utilizadas em todos os arquivos do A-Team dis- */
                       /* tribuido para o Set Covering Problem.              */

extern char      DualWorst;
extern int       nb_lin;
extern int       nb_col;
extern int       nb_cuts_mem;
extern int       cuts_equal;
extern int       proposed_cuts;
extern int       excedent_cuts;
extern int       first_free;
extern int       nb_dual_sol_mem;
extern int       dual_worst;
extern int       dual_equal;
extern int       CutsofSol;
extern int       MaxLenDualMem;
extern int       MaxLenCutMem;
extern int       LenHashTab;
extern int       ProposedSolution[NbDualAgents];
extern int       ProposedSucess[NbDualAgents];
extern int       SolutionAccept[NbDualAgents];
extern int       SolutionAgents[NbDualAgents];
extern long      total_time;
extern long      lost_time;
extern float     density;
extern double    BestSolution[NbDualAgents];
extern FILE     *fstts;
extern FILE     *fdump;
extern Agents    work_agent;
extern DualType *DualMem;
extern CutType  *CutMem;

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */

void LocalTime(int *hour,
	       int *min,
	       int *sec)
{
  struct tm      day;
  struct timeval now;

  gettimeofday(&now);
  day   = *localtime(&(now.tv_sec));
  *hour = day.tm_hour;
  *min  = day.tm_min;
  *sec  = day.tm_sec;
}

/* ------------------------------------------------------------------------- */

void OpenFileStatistics(char *path)
{
  char path_dump[100];
  char path_stts[100];

  strcpy(path_dump,path);
  strcpy(path_stts,path);
  strcat(path_dump,"_MD_sol");
  strcat(path_stts,"_MD_est");
  fdump = fopen(path_dump,"w");
  fstts = fopen(path_stts,"w");

  fprintf(fstts,"O arquivo contem ''fotos'' da memoria de solucoes duais");
  fprintf(fstts,"\nTempo    -> Tempo entre o inicio do A-Team e a foto");
  fprintf(fstts,"\nSolDual  -> Valor do melhor solucao dual encontrada");
  fprintf(fstts,"\nDProp    -> Nr de sol. duais propostas a memoria");
  fprintf(fstts,"\nDAcei    -> Nr de sol. duais prop. e aceitas pela mem");
  fprintf(fstts,"\nDIgua    -> Nr de sol. duais prop. nao aceitas iguais");
  fprintf(fstts,"\nDPior    -> Nr de sol. duais prop. pior que pior da mem");
  fprintf(fstts,"\nDualPior -> Taxa de proposicao de sol. duais piores");
  fprintf(fstts,"\nDualAcei -> Taxa de aceitacao das sol. duais propostas");
  fprintf(fstts,"\n\n            Numero de Linhas   =  %5d",nb_lin);
  fprintf(fstts,"\n            Numero de Colunas  =  %5d",nb_col);
  fprintf(fstts,"\n            Densidade          =  %5.2f",density);
  fprintf(fstts,"\n            Tamanho Mem Dual   =  %5d", MaxLenDualMem);
  fprintf(fstts,"\n            Tamanho Mem Cortes = %6d\n", MaxLenCutMem);
  fprintf(fstts,"\n--------------------------------------------------------------------------------------------------------------------------------");
  fprintf(fstts,"\n Tempo Theur   Prop   Acei  Igual   Pior  %%Pior  %%Acei       SolDual  Cut       GDual    %%      Subgrad    %%    3-Optimum    %%");
  fflush(fstts);

  memset(ProposedSolution,0,NbDualAgents * sizeof(int));
  memset(ProposedSucess,0,NbDualAgents * sizeof(int));
  memset(SolutionAccept,0,NbDualAgents * sizeof(int));
  memset(SolutionAgents,0,NbDualAgents * sizeof(int));
}

/* ------------------------------------------------------------------------- */

void WriteStatistics(int pos)
{
  int   dual_accept   = 0,
        dual_proposed = 0,
        hour          = 0,
        min           = 0,
        sec           = 0;
  float dual_rate     = 0,
        opt3_rate     = 0,
        subgrad_rate  = 0,
        worst_rate    = 0,
        accept_rate   = 0;

  LocalTime(&hour,&min,&sec);
  total_time = sec + (60 * min) + (3600 * hour);
  if (total_time < lost_time)
    (total_time += 86400);
  total_time -= lost_time;
  dual_proposed = ProposedSolution[agent_dual] +
                  ProposedSolution[agent_3opt] +
	          ProposedSolution[agent_subgrad];
  dual_accept  = dual_proposed - dual_equal - dual_worst;
  dual_rate    = ((float) SolutionAccept[agent_dual] / MaxLenDualMem) * 100;
  opt3_rate    = ((float) SolutionAccept[agent_3opt] / MaxLenDualMem) * 100;
  subgrad_rate = ((float) SolutionAccept[agent_subgrad] / MaxLenDualMem) * 100;
  worst_rate   = ((float) dual_worst / dual_proposed) * 100;
  accept_rate  = ((float) dual_accept / dual_proposed) * 100;

  fprintf(fstts,"\n%6d %6d %6d %6d %6d %6d  %5.1f  %5.1f %13.3f%4d%13.3f%5.1f%13.3f%5.1f%13.3f%5.1f",total_time, DualMem[pos].proc_time, dual_proposed, dual_accept, dual_equal, dual_worst, worst_rate, accept_rate, DualMem[nb_dual_sol_mem - 1].value, DualMem[nb_dual_sol_mem - 1].nb_cuts, BestSolution[agent_dual], dual_rate, BestSolution[agent_subgrad], subgrad_rate, BestSolution[agent_3opt], opt3_rate);
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
     case LinearDecr : pos = (int)(mem_size * (1 - sqrt(drand48())));
                       break;
     case Constant   : pos = (int)(mem_size * drand48());
   }
  return(pos);
}

/* ------------------------------------------------------------------------- */

int DualRightRealign(int       pos,
		     DualType *DualSol)
{
  while (pos > 0)
    if (DualSol->value < DualMem[pos-1].value)
     { DualMem[pos] = (DualType) DualMem[pos-1];
       pos--;
     }
    else
      break;
  return(pos);
}

/* ------------------------------------------------------------------------- */

int DualLeftRealign(int       pos,
		    DualType *DualSol)
{
  while (pos < nb_dual_sol_mem-1)
    if (DualSol->value > DualMem[pos+1].value)
     { DualMem[pos] = (DualType) DualMem[pos+1];
       pos++;
     }
    else
      break;
  return(pos);
}

/* ------------------------------------------------------------------------- */

void FindPosInsertion(int      *pos,
		      DualType *DualSol)
{
  int i      = 0,
      probab = 0;

  if (nb_dual_sol_mem < MaxLenDualMem)
   { *pos = DualRightRealign(nb_dual_sol_mem,DualSol);
     nb_dual_sol_mem++;
   }
  else
   { switch (DualSol->agent_ID)
      { case agent_dual    : probab = LinearDecr;
	                     break;
        case agent_3opt    : probab = Constant;
	                     break;
        case agent_subgrad : probab = LinearIncr;
      }
     *pos = ChoosePosition(probab,nb_dual_sol_mem - 1);
     for (i = 0; i < DualMem[*pos].nb_cuts; i++)
      { CutMem[DualMem[*pos].cut[i]].nb_sol--;
	if (CutMem[DualMem[*pos].cut[i]].nb_sol == 0)
	  CutMem[DualMem[*pos].cut[i]].used = TRUE;
      }
     free(DualMem[*pos].cut);
     free(DualMem[*pos].var_u);
     free(DualMem[*pos].red_cost);
     SolutionAccept[DualMem[*pos].agent_ID]--;
     if (DualMem[*pos].value > DualSol->value)
       *pos = DualRightRealign(*pos,DualSol);
     else
       *pos = DualLeftRealign(*pos,DualSol);
   }
}

/* ------------------------------------------------------------------------- */

int InsertDualMem(int      *pos,
		  DualType *DualSol)
{
  int i = 0;

  i = nb_dual_sol_mem - 1;
  while ((i >= 0) && (DualSol->value < (DualMem[i].value - epsilon)))
    i--;
  if ((i >= 0) &&
      (DualSol->value > (DualMem[i].value - epsilon)) &&
      (DualSol->value < (DualMem[i].value + epsilon)))
    { dual_equal++;
      free(DualSol->cut);
      free(DualSol->var_u);
      free(DualSol->red_cost);
      return(-1);
    }
  if (!DualWorst)
    if ((nb_dual_sol_mem > 0) && (DualSol->value < DualMem[0].value))
     { dual_worst++;
       free(DualSol->cut);
       free(DualSol->var_u);
       free(DualSol->red_cost);
       return(-1);
     }
  if (DualSol->improved_sol)
   { for (i = 0; i < DualSol->nb_cuts; i++)
       CutMem[DualSol->cut[i]].nb_sol++;
     DualSol->improved_sol = FALSE;
   }
  SolutionAgents[DualSol->agent_ID]++;
  FindPosInsertion(pos,DualSol);
  DualMem[*pos] = (DualType) *DualSol;
  if (DualSol->value > BestSolution[DualSol->agent_ID])
   { BestSolution[DualSol->agent_ID] = DualSol->value;
     return (1);
   }
  else
    return (0);
}

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */

void InitParamHashTable()
{
  int i    = 0,
      hour = 0,
      min  = 0,
      sec  = 0;

  MaxLenDualMem = ReadAteamParam(2);
  MaxLenCutMem  = ReadAteamParam(4);
  LenHashTab    = ReadAteamParam(5);
  CutsofSol     = ReadAteamParam(6);

  DualMem = (DualType *) malloc(MaxLenDualMem * sizeof(DualType));
  CutMem  = (CutType *) malloc(MaxLenCutMem * sizeof(CutType));
  first_free  = LenHashTab;
  for (i = 0; i < LenHashTab; i++)
    CutMem[i] = (CutType) {FALSE,0,NULL,0,i,-1,-1,0};
  for (i = LenHashTab; i < MaxLenCutMem-1; i++)
    CutMem[i] = (CutType) {FALSE,0,NULL,0,i,-1,i+1,0};
  CutMem[i] = (CutType) {FALSE,0,NULL,0,i,-1,-1,0};
  
  LocalTime(&hour,&min,&sec);
  lost_time = sec +(60 * min) + (3600 * hour);
}

/* ------------------------------------------------------------------------- */

void ChooseCuts(int     *nb_cuts,
		CutType *cut)
{
  int i        = 0,
      pos      = 0,
      rand_qtd = 0;

  *nb_cuts = 0;  
  memset(cut,0,CutsofSol * sizeof(CutType));
  do
    rand_qtd = 1 + (lrand48() % CutsofSol);
  while (rand_qtd > nb_cuts_mem);
  while (*nb_cuts < rand_qtd)
   { pos = lrand48() % LenHashTab;
     while ((pos > -1) && (*nb_cuts < rand_qtd))
      { if ((CutMem[pos].nb_sol > -1) && (!CutMem[pos].choosed))
	 { cut[*nb_cuts] = (CutType) CutMem[pos];
	   (*nb_cuts)++;
	   CutMem[pos].choosed = TRUE;
	   CutMem[pos].nb_sol++;
	   pos = CutMem[pos].next;
         }
	else
	  pos = -1;
      }
   }
  for (i = 0; i < *nb_cuts; i++)
    CutMem[cut[i].tab_pos].choosed = FALSE;
}

/* ------------------------------------------------------------------------- */

int DestructCuts()
{
  int i    = 0,
      pos  = 0,
      aux  = 0,
      next = 0;

  for (i = 0; i < LenHashTab; i++)
   { if (CutMem[i].nb_sol == 0)
      { pos = i;
	next = CutMem[pos].next;
	CutMem[pos].nb_sol = -1;
	nb_cuts_mem--;
	while (next > 0)
	 { if (CutMem[next].nb_sol == 0)
	    { CutMem[next].nb_sol = -1;
	      nb_cuts_mem--;
	      aux = CutMem[pos].next;
	      if (next >= LenHashTab)
	       { CutMem[next].next = first_free;
		 first_free = next;
	       }
	    }
	   else
	    { CutMem[pos]         = CutMem[next];
	      CutMem[next].nb_sol = -1;
	      pos = next;
	      aux = CutMem[next].next;
	    }
	   next = aux;
         }
	if (pos > -1)
	  if ((CutMem[pos].nb_sol == -1) && (pos >= LenHashTab))
	   { CutMem[pos].next = first_free;
	     first_free = pos;
	   }
      }
   }
}

/* ------------------------------------------------------------------------- */

int FindCutPosition(int     *pos,
		    CutType *cut)
{
  char find = FALSE;
  int  prev = 0;

  proposed_cuts++;
  *pos = (int) (cut->key % LenHashTab);
  if ((CutMem[*pos].nb_sol == 0) && (CutMem[*pos].used))
    find = TRUE;
  else
    if (CutMem[*pos].nb_sol >= 0)
      { while (*pos >= 0)
	 { if (memcmp(CutMem[*pos].coef,cut->coef,nb_col * sizeof(short)) == 0)
	    { cuts_equal++;
	      break;
	    }
	   if ((CutMem[*pos].nb_sol == 0) && (CutMem[*pos].used))
	     return(TRUE);
	   prev = *pos;
	   *pos = CutMem[*pos].next;
         }
	if ((*pos < 0) && (first_free >= 0))
	 { *pos = first_free;
	   first_free = CutMem[first_free].next;
	   CutMem[prev].next = *pos;
	   find = TRUE;
         }
      }
   }
  return(find);
}

/* ------------------------------------------------------------------------- */

void InsertCutMem(char    *sleep_ag,
		  CutType *cut)
{
  int i   = 0,
      pos = 0;

/*  if (first_free < 0)
   { excedent_cuts++;
     first_free = DestructCuts();
   } */
/*  if (first_free >= 0) */
   { if (FindCutPosition(&pos,cut))
      { cut->tab_pos = CutMem[pos].tab_pos;
	cut->nest    = CutMem[pos].next;
        CutMem[pos]  = (CutType) *cut;
	nb_cuts_mem++;
      }
   }
  *sleep_ag = (first_free < 0);
}

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */
