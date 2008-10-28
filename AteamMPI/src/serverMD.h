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
/* Este arquivo contem as definicoes de variaveis globais ao servidor de me- */
/* moria de solucoes duais.                                                  */
/* ------------------------------------------------------------------------- */

char   DualWorst       = TRUE,
      *path            = NULL;
int    nb_lin          = 0,
       nb_col          = 0,
       nb_cuts_mem     = 0,
       cuts_equal      = 0,
       proposed_cuts   = 0,
       excedent_cuts   = 0,
       first_free      = 0,
       nb_dual_sol_mem = 0,
       dual_worst      = 0,
       dual_equal      = 0,
       CutsofSol       = 0,
       MaxLenDualMem   = 0,
       MaxLenCutMem    = 0,
       LenHashTab      = 0,
       ProposedSolution[NbDualAgents],
       ProposedAgents[NbDualAgents],
       ProposedSucess[NbDualAgents],
       SolutionAccept[NbDualAgents],
       SolutionAgents[NbDualAgents];
long   total_time_cron = 0,  /* Tempo total de processamento em segundos */
       lost_time_cron  = 0;  /* Tempo total perdido com geracao de estatit. */
u_long NbServMD        = 0;
float  density         = 0,
       MaxSolDualAg    = 0;

double BestSolution[NbDualAgents] = {MINDOUBLE,  /* best dual greedy */
				     MINDOUBLE,  /* best 3opt        */
				     MINDOUBLE, /* best subgrad     */
				     MINDOUBLE};
FILE     *fstts   = NULL,
         *fdump   = NULL;
CutType  *CutMem  = NULL;
DualType *DualMem = NULL;

/* ----------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ----------------------------------------------------------------------- */
