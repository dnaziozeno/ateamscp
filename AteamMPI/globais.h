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
/* Este arquivo contem as inclusoes de arquivos, definicoes de macros, cons- */
/* tantes e tipos usadas em todos os arquivos do A-Team distribuido  para  o */
/* Set Covering Problem.                                                     */
/* ------------------------------------------------------------------------- */

#include <ctype.h>
//#include <dpsk.hpp>
#include <limits.h>
#include <math.h>
#include <search.h>
#include <stdio.h>
#include <stdlib.h>
//#include <stream.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <values.h>
#include <time.h>
#include <mpi.h>
//HHHH
/*
extern "C" { int             getrusage(int,struct rusage*);
             int             gettimeofday(struct timeval*);
	       long            lrand48(void);
	       double          drand48(void);
	       void            srand48(long);
	       unsigned short *seed48(unsigned short);
	       unsigned        sleep(unsigned);
	     }
*/

/* Definicoes relativas ao A-Team */
#define TRUE             1
#define FALSE            0
#define epsilon          10e-8
#define TimeWaiting      120
#define TotalAgents      11
#define NbDualAgents     4
#define NbPrimalAgents   6
#define LinearIncr       0
#define LinearDecr       1
#define Constant         2

/* Definicao dos metodos do A-Team */
#define REQUEST_SOLUTION      0
#define AGENT_SEND_SOLUTION   1
#define REQUEST_CUTS          2
#define REQUEST_CUTS_OF_SOL   3
#define AGENT_SEND_CUTS       4
#define STOP_ATEAM            5
#define INIT_GLOBAL_VARS      6
#define DUMP_MEMORY           7
#define CHANGE_MEM_INSERTION  8
#define GET_RESULTS_PATH      9
#define STOP_SERVER           10 
typedef enum {agent_dual,
	      agent_3opt,
	      agent_subgrad,
	      agent_gencut,
	      agent_primal,
	      agent_beasley,
	      agent_consensus,
	      agent_adjust,
	      agent_tabu,
	      agent_search,
	      agent_free} Agents;

/* -------------------------------------------------------------------------
   Definicao dos elementos de uma lista "triplamente ligada", que sera usada
   na construcao de linhas e colunas de uma matriz esparsa que  representara
   o problema a ser tratado.
   ------------------------------------------------------------------------- */
typedef struct dlist
{ struct dlist *up,
               *right,
               *down;
  short         coef;
  int           lin,
                col;
} DList;


/* -------------------------------------------------------------------------
   Definicao das colunas da matriz que representara o problema a ser tratado.
   Cada coluna da matriz correponde a um elemento da funcao objetivo e as o-
   corencias desse elemento nas restricoes.
   ------------------------------------------------------------------------- */
typedef struct
{ DList  *first,
         *last;
  char    del;
  int     elem_col,
          var;
  double  coef;
} Col;


/* -------------------------------------------------------------------------
   Definicao das linhas da matriz que representara o problema a ser tratado.
   Cada linha da matriz correponde a uma restricao do problema.
   ------------------------------------------------------------------------- */
typedef struct
{ DList *first,
        *last;
  char   del;
  short  rhs;
  int    elem_lin;
} Lin;


/* -------------------------------------------------------------------------
   Definicao da estrutura de um componente da memoria dual.
   ------------------------------------------------------------------------- */
typedef struct
{ char    improved_sol,
          agent[TotalAgents];
  int     nb_cuts,
         *cut;
  long    proc_time;
  double  value,
         *red_cost,
         *var_u;
  Agents  agent_ID;
} DualType;


/* -------------------------------------------------------------------------
   Definicao da estrutura de um componente da memoria primal.
   ------------------------------------------------------------------------- */
typedef struct
{ char    agent[TotalAgents];
  int    *var_x;
  int     proc_time;
  double  value;
  Agents  agent_ID;
} PrimalType;


/* -------------------------------------------------------------------------
   Definicao da estrutura de um componente da memoria de cortes.
   ------------------------------------------------------------------------- */
typedef struct
{ char   used,
         chosen;
  short  rhs,
        *coef;
  int    col_cut,
         tab_pos,
         nb_sol,
         next;
  long   key;
} CutType;



/* -------------------------------------------------------------------------
   Declaracao dos prototipos de funcoes usadas nos servidores mem. do ATeam.
   ------------------------------------------------------------------------- */

void LocalTime(long *cron_time); /* agentrot.c */
int  ReadAteamParam(int param);
void OpenFileStatistics(char *path);
void WriteStatistics(int pos);
int  ChoosePosition(int probability,
		    int mem_size);
void InsertDualMem(char     *sleep_ag,
		   char     *new_problem,
		   DualType *DualSol);
void InsertPrimalMem(char       *sleep_ag,
		     PrimalType *PrimalSol);
void InitParamHashTable(); /* dualrot.c */
void InitAteamParam();
void ChooseCuts(int     *nb_cuts,
		CutType *cut);
char InsertCutMem(char    *sleep_ag,
		  CutType *cut);



/*
DPSK_CLASS *DefMDServer(char   *ID_DPSK,
			char   *path,
			char    init_serv,
			u_long *ServerNb);
DPSK_CLASS *DefMPServer(char   *ID_DPSK,
			char   *path,
			char    init_serv,
			u_long *ServerNb);
*/



/* ----------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ----------------------------------------------------------------------- */

typedef struct
{ long sort,
       j;
} RandInd;

