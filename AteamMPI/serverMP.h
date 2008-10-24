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
/* moria de solucoes primais.                                                */
/* ------------------------------------------------------------------------- */

char   PrimalWorst       = TRUE,
       EqualCostSol      = FALSE,
      *path              = NULL;
int    nb_lin            = 0,
       nb_col            = 0,
       nb_primal_sol_mem = 0,
       primal_worst      = 0,
       primal_equal      = 0,
       MaxLenPrimalMem   = 0,
       ProposedSolution[NbPrimalAgents],
       ProposedAgents[NbPrimalAgents],
       ProposedSucess[NbPrimalAgents],
       SolutionAccept[NbPrimalAgents],
       SolutionAgents[NbPrimalAgents];
long   total_time_cron   = 0,  /* Tempo total de processamento em segundos */
       lost_time_cron    = 0;  /* Tempo total perdido com geracao de estat. */
u_long NbServMP          = 0;
float  density           = 0,
       MaxSolPrimalAg    = 0;

double BestSolution[NbPrimalAgents] = {MAXDOUBLE,  /* best primal greedy */
				       MAXDOUBLE,  /* best beasley       */
				       MAXDOUBLE,  /* best consensus     */
				       MAXDOUBLE,  /* best adjust        */
				       MAXDOUBLE,  /* best tabu          */
				       MAXDOUBLE}; /* best search        */

FILE       *fstts     = NULL,
           *fdump     = NULL;
PrimalType *PrimalMem = NULL;

/* ----------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ----------------------------------------------------------------------- */
