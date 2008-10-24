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
/* Este arquivo contem as declaracoes de variaveis globais para  todos os a- */
/* agentes do ATeam distribuido para o Set Covering Problem.                 */
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */

char   RandomInitMP    = FALSE,
       RandomPrimal    = FALSE,
       RandomConsensus = FALSE,
       RandomInitMD    = FALSE,
       RandomDual      = FALSE,
       BalasNgCut      = FALSE,
       unicost         = TRUE;
int    nb_lin          = 0,
       nb_col          = 0,
       max_lin         = 0,
       CutsofSol       = 0,
       TimeSleeping    = 0,
       MaxCutGen       = 0,
       MaxLenDualMem   = 0,
       MaxLenPrimalMem = 0,
       MinChange       = 0,
       MaxExeTime      = 0,
       ReducPerc       = 0,
       TabuIterations  = 0;
long   total_time_cpu  = 0,
       lost_time_cpu   = 0,
       total_time_cron = 0,
       lost_time_cron  = 0;
float  density         = 0,
       RestrValue      = 0;
FILE  *finput          = NULL;
Lin   *line            = NULL;
Col   *column          = NULL;


/* -------------------------------------------------------------------------
   Declaracao dos prototipos das funcoes usadas nos agentes do ATeam.      
   ------------------------------------------------------------------------- */

void ReadSource();                                               /* readsc.c */
void FreeSource();
void Reduction(int    *var_x,                                    
	       double *Z_primal);                                 /* reduction.c */
void DualGreedy(char      RandomGreedy,
		float     reduc,
		DualType *DualSol);                                      /* grdual.c */
void ThreeOpt(DualType *DualSol);                              /* threeopt.c */
void SubGradiente(float       lambda,
		  DualType   *DualSol,                                  /* subgrad.c */
		  PrimalType *PrimalSol);
void PrimalGreedy(char        RandomGreedy,
		  DualType   *DualSol,
		  PrimalType *Sol);
void FeoResendeGreedy(float       alpha,
		      PrimalType *PrimalSol);
void RandomAdjust(PrimalType *PrimalSol);
void LocalSearch(PrimalType *PrimalSol);
void FeoResendeSearch(PrimalType *PrimalSol);
void PrimalConsensus(int        *var_x1,
		     int        *var_x2,
		     DualType   *DualSol,
		     PrimalType *PrimalSol);
void BeasleyHeur(DualType   *DualSol,
		 PrimalType *PrimalSol);
void GenBNCuts(int        *nb_cuts,
	       PrimalType *PrimalSol,
	       CutType    *cut);
void GenBRCuts(int        *nb_cuts,
	       PrimalType *PrimalSol,
	       CutType    *cut);
void GetTimes(char AddTime);
char MaxExeTimeCron();

void RequestDualSol(char        best_solution,
		    char       *stop,
		    int         probab,
		    int        *cut,
		    double     *var_u,
		    double     *red_cost,
		    DualType   *DualSol,
		    Agents      requester,
		    MPI_Comm   communicator);
		      
void SendDualSolutiontoServer(char       *stop,
			      			  char       *sleep_ag,
			                  char       *new_problem,
			                  DualType   *DualSol,
			                  MPI_Comm   communicator);			      

void RequestPrimalSol(char        best_solution,
				      char       *stop,
				      int         probab,
				      int        *var_x,
				      PrimalType *PrimalSol,
				      MPI_Comm   communicator);
		      
void SendPrimalSolutiontoServer(char       *stop,
				                char       *sleep,
				                PrimalType *PrimalSol,
				                MPI_Comm communicator);

void RequestNCutstoServer(char       *stop,
			              int        *nb_cuts,
			              CutType    *cut,
			              MPI_Comm   communicator);
 
void RequestCutsofSoltoServer(char       *stop,
			      CutType    *cut,
			      DualType   *DualSol,
			      MPI_Comm   communicator);
/*
void SendCutstoServer(char       *stop,
		      char       *sleep_ag,
		      int         nb_cuts,
		      CutType    *cut,
		      DPSK_CLASS *ServMDClass);
*/ 
void AddCutstoProblem(int       nb_cuts,
		      CutType  *cut,
		      DualType *DualSol);
void ExcludeCutsofProblem(int nb_cuts);

//void StopAteam(MPI_Comm commMD, MPI_Comm commMP);
/*
void FinishInitServer(DPSK_CLASS *ServMemClass);
*/
void GetResultsPath(char *path, MPI_Comm communicator);
/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */
