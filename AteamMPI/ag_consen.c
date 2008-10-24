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
/* Este arquivo contem o codigo necessario para a interface do  agente  con- */
/* senso com o servidor de memoria de solucoes primais do  ATeam distribuido */
/* para o Set Covering Problem.                                              */
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */

#include "globais.h"    /* Inclusao das definicoes de tipos, macros e  cons- */
			/* tantes utilizadas em todos os arquivos  do A-Team */
                        /* distribuido para o Set Covering Problem.          */
#include "agentes.h"    /* Inclusao das definicoes de variaveis usadas em to-*/
                        /* os agentes do ATeam distribuido.                  */

int    *U_x      = NULL,
       *unsat    = NULL,
       *cardin   = NULL,
       *sets     = NULL;
double *aux_cost = NULL;

void ExecAgConsen(MPI_Comm communicatorMD, MPI_Comm communicatorMP);

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
   O primeiro parametro a ser passado para esse agente e' o numero do proce-
   ssador em que esta executando o 'ns'  e o segundo parametro e' o nome  do
   arquivo que contem a instancia do SCP.
   ------------------------------------------------------------------------- */
main(int argc, char *argv[])
{
  int size,          
      rank,	
	  flagConnectMP = 1,
	  flagConnectMD = 1;		  
  char portMD[MPI_MAX_PORT_NAME],
       portMP[MPI_MAX_PORT_NAME];
       
  MPI_Comm commServerMD, interfaceComm, commServerMP;          
  	            				            
  MPI_Init(&argc, &argv);               
  MPI_Comm_size(MPI_COMM_WORLD, &size); 
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
                           
  MPI_Comm_get_parent(&interfaceComm);
                                       
  /* Tetativa de conectar com o servidor */
  flagConnectMD = MPI_Lookup_name("ServerMD", MPI_INFO_NULL, portMD);
  if(flagConnectMD) 
  {
      printf("\n\n* Agente Consen finalizado *");
      printf("\n* ERRO : Servidor de solucoes duais nao iniciado. *\n");
      fflush(stdout);
  }	 
  else
  { 
  	MPI_Comm_connect(portMD, MPI_INFO_NULL, 0, MPI_COMM_SELF, &commServerMD);
  	flagConnectMP = MPI_Lookup_name("ServerMP", MPI_INFO_NULL, portMP);
    if(flagConnectMP) 
    {
      printf("\n\n* Agente Consen finalizado *");
      printf("\n* ERRO : Servidor de solucoes primais nao iniciado. *\n");
      fflush(stdout);
    }
  	else
  	{
		TimeSleeping    = (int)   ReadAteamParam(1);
		CutsofSol       = (int)   ReadAteamParam(6);
		MaxExeTime      = (int)   ReadAteamParam(11);
		RandomConsensus = (char)  ReadAteamParam(15);
		RestrValue      = (float) (ReadAteamParam(18) / 100.0);
		if (!(finput = fopen(argv[2],"r")))
		{ printf("\n\n* Erro na abertura do arquivo %s. *\n",argv[2]);
		   exit(1);
		}
		ReadSource();
		fclose(finput);
		Reduction(NULL,NULL);
		srand48(time(NULL));

		MPI_Comm_connect(portMP, MPI_INFO_NULL, 0, MPI_COMM_SELF, &commServerMP);
		
		ExecAgConsen(commServerMD, commServerMP);

		printf("\n\n* Agente Consenso finalizado *");
		printf("\n* Servidor finalizou processamento. *\n");
	}
 }
   /* Finaliza comunicacao com o servidor */
   int message = 1224;
   MPI_Send(&message, 1, MPI_INT, 0, 1, commServerMD);
   MPI_Send(&message, 1, MPI_INT, 0, 1, commServerMP);
   
   /* Envia mensagem de Finalizacao para interface */
   //message = 1;
   //MPI_Send(&message, 1, MPI_INT, 0, 1, interfaceComm);
  
   MPI_Comm_disconnect(&commServerMD);
   MPI_Comm_disconnect(&commServerMP);
   MPI_Finalize();
   
   printf("\n ==== ag_consen FINALIZADO \n", message);
}

/* ------------------------------------------------------------------------- */

void ExecAgConsen(MPI_Comm communicatorMD, MPI_Comm communicatorMP)
{
  char        stop     = FALSE,
              sleep_ag = FALSE;
  int         cont     = 0,
             *var_x1   = NULL,
             *var_x2   = NULL,
             *var_x3   = NULL,
             *cut_pos  = NULL;
  double      value1   = 0,
              value2   = 0,
             *var_u    = NULL,
             *red_cost = NULL;
  DualType    DualSol;
  PrimalType  PrimalSol;

  var_u    = (double *) malloc(max_lin * sizeof(double));
  red_cost = (double *) malloc(nb_col * sizeof(double));
  aux_cost = (double *) malloc(nb_col * sizeof(double));
  cut_pos  = (int *) malloc(CutsofSol * sizeof(int));
  U_x      = (int *) malloc((nb_lin + 1) * sizeof(int));
  unsat    = (int *) malloc(nb_lin * sizeof(int));
  cardin   = (int *) malloc(nb_col * sizeof(int));
  sets     = (int *) malloc((nb_col + 1) * sizeof(int));
  var_x1   = (int *) malloc(nb_col * sizeof(int));
  var_x2   = (int *) malloc(nb_col * sizeof(int));
  var_x3   = (int *) malloc(nb_col * sizeof(int));

  GetTimes(FALSE);
  do
   { if (sleep_ag)
       sleep(TimeSleeping);
     if (!unicost)
       RequestDualSol(FALSE,&stop,LinearIncr,cut_pos,var_u,red_cost,
		      &DualSol,agent_free, communicatorMD);
     if (!stop)
     {  
     	RequestPrimalSol(TRUE,&stop,LinearDecr,var_x1,&PrimalSol, communicatorMP);
	    if (!stop)
	    { value1 = PrimalSol.value;
	      do
	         RequestPrimalSol(FALSE,&stop,LinearDecr,var_x2,
			      &PrimalSol, communicatorMP);
	      while ((value1 == PrimalSol.value) && (!stop));
	      if (!stop)
	      { value2 = PrimalSol.value;
	        do
		      RequestPrimalSol(FALSE,&stop,LinearDecr,var_x3,
				 &PrimalSol, communicatorMP);
	        while (((PrimalSol.value == value1) || (PrimalSol.value == value2)) && (!stop));
	        if (!stop)
	        { PrimalSol.var_x = var_x1;
		      PrimalSol.value = value1;
		      PrimalConsensus(var_x2,var_x3,&DualSol,&PrimalSol);
		      GetTimes(TRUE);
		      PrimalSol.proc_time = total_time_cpu - lost_time_cpu;
		      SendPrimalSolutiontoServer(&stop,&sleep_ag, &PrimalSol, communicatorMP);
	        }
	      }
       }
      printf("\n consen");
     }
   } while((!stop) && (MaxExeTimeCron()));

  if (!stop)
    StopAteam(NULL, communicatorMP);

  free(aux_cost);
  free(var_u);
  free(red_cost);
  free(U_x);
  free(unsat);
  free(cardin);
  free(sets);
  free(cut_pos);
  free(var_x1);
  free(var_x2);
  free(var_x3);
}

/* ------------------------------------------------------------------------- */

void PrimalConsensus(int        *var_x1,
		     int        *var_x2,
		     DualType   *DualSol,
		     PrimalType *PrimalSol)
{
  int   j     = 0,
        k     = 0;

  for (j = 0; j < nb_col; j++)
   { k = column[j].var;
     if ((PrimalSol->var_x[k] == 1) && (var_x1[k] == 0) && (var_x2[k] == 0))
      { PrimalSol->var_x[k] = 0;
        PrimalSol->value   -= column[j].coef;
      }
   }
  if (unicost)
    FeoResendeGreedy(RestrValue,PrimalSol);
  else
    PrimalGreedy(RandomConsensus,DualSol,PrimalSol);

  PrimalSol->agent[agent_primal]    = FALSE;
  PrimalSol->agent[agent_beasley]   = FALSE;
  PrimalSol->agent[agent_consensus] = TRUE;
  PrimalSol->agent[agent_adjust]    = FALSE;
  PrimalSol->agent[agent_tabu]      = FALSE;
  PrimalSol->agent[agent_search]    = FALSE;
  PrimalSol->agent_ID               = agent_consensus;
}

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */
