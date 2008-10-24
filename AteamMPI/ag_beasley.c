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
/* Este arquivo contem o codigo necessario para a interface do agente Beasley*/
/* Heuristic com o servidor de memoria de solucoes primais do ATeam  distri- */
/* buido para o Set Covering Problem.                                        */
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */

#include "globais.h"   /* Inclusao das definicoes de tipos, macros e constan-*/
                       /* tes utilizadas em todos os arquivos do A-Team dis- */
                       /* tribuido para o Set Covering Problem.              */
#include "agentes.h"    /* Inclusao das definicoes de variaveis usadas em to-*/
                        /* os agentes do ATeam distribuido.                  */

int    *S = NULL,
       *G = NULL,
       *X = NULL;
double *C = NULL,
       *P = NULL,
       *t = NULL;

void ExecAgBeasley(MPI_Comm communicatorMD, MPI_Comm communicatorMP);

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
  int flagConnectMP = 1,
	  flagConnectMD = 1;		  
  char portMD[MPI_MAX_PORT_NAME],
       portMP[MPI_MAX_PORT_NAME];
       
  MPI_Comm commServerMD, interfaceComm, commServerMP;          
  	            				            
  MPI_Init(&argc, &argv);               
  MPI_Comm_get_parent(&interfaceComm);
                                       
  /* Tetativa de conectar com o servidor */
  flagConnectMD = MPI_Lookup_name("ServerMD", MPI_INFO_NULL, portMD);
  if(flagConnectMD) 
  {
      printf("\n\n* Agente Beasley finalizado *");
      printf("\n* ERRO : Servidor de solucoes duais nao iniciado. *\n");
      fflush(stdout);
  }	 
  else
  { 
  	 MPI_Comm_connect(portMD, MPI_INFO_NULL, 0, MPI_COMM_SELF, &commServerMD);
     flagConnectMP = MPI_Lookup_name("ServerMP", MPI_INFO_NULL, portMP);
     if(flagConnectMP) 
     { printf("\n\n* Agente Beasley finalizado *");
	   printf("\n* ERRO : Servidor de solucoes primais nao iniciado. *\n");
     }
     else
     {  
     	MPI_Comm_connect(portMP, MPI_INFO_NULL, 0, MPI_COMM_SELF, &commServerMP);
     	
     	CutsofSol  = (int)  ReadAteamParam(6);
	    MaxExeTime = (int)  ReadAteamParam(11);
	    if (!(finput = fopen(argv[2],"r")))
	    { printf("\n\n* Erro na abertura do arquivo %s. *\n",argv[2]);
	      exit(1);
        }
	    ReadSource();
 	    fclose(finput);
	    Reduction(NULL,NULL);
	    srand48(time(NULL));

	    ExecAgBeasley(commServerMD, commServerMP);
	    
	    printf("\n\n* Agente Beasley Heur. finalizado *");
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
   
   printf("\n ==== ag_beasley FINALIZADO \n", message);	
}

/* ------------------------------------------------------------------------- */

void ExecAgBeasley(MPI_Comm communicatorMD, MPI_Comm communicatorMP)
{
  char        stop        = FALSE,
              stop2       = FALSE,
              sleep_ag    = FALSE,
              sleep2      = FALSE,
              new_problem = FALSE;
  int         j           = 0,
             *cut_pos     = NULL,
             *var_x       = NULL;
  double     *var_u       = NULL,
             *red_cost    = NULL,
             *aux_cost    = NULL;
  DualType    DualSol;
  PrimalType  PrimalSol;

  var_u    = (double *) malloc(max_lin * sizeof(double));
  red_cost = (double *) malloc(nb_col * sizeof(double));
  aux_cost = (double *) malloc(nb_col * sizeof(double));
  C        = (double *) malloc(nb_col * sizeof(double));
  P        = (double *) malloc(nb_col * sizeof(double));
  t        = (double *) malloc(nb_lin * sizeof(double));
  X        = (int *) malloc(nb_col * sizeof(int));
  S        = (int *) malloc(nb_col * sizeof(int));
  G        = (int *) malloc(nb_lin * sizeof(int));
  var_x    = (int *) malloc(nb_col * sizeof(int));
  cut_pos  = (int *) malloc(CutsofSol * sizeof(int));

  for (j = 0; j < nb_col; j++)
    aux_cost[j] = column[j].coef;
  
  GetTimes(FALSE);
  do
  { RequestDualSol(FALSE,&stop,LinearIncr,cut_pos,var_u,red_cost,
		           &DualSol,agent_free,communicatorMD);
    if (!stop)
    { RequestPrimalSol(TRUE,&stop,0,var_x,&PrimalSol,communicatorMP);
	  
	  if (!stop)
	  { BeasleyHeur(&DualSol,&PrimalSol);
	    GetTimes(TRUE);
	    DualSol.proc_time = total_time_cpu - lost_time_cpu;
	    SendDualSolutiontoServer(&stop,&sleep_ag,&new_problem,
				                 &DualSol, communicatorMD);
	    PrimalSol.proc_time = total_time_cpu - lost_time_cpu;
	    SendPrimalSolutiontoServer(&stop2,&sleep2,&PrimalSol,communicatorMP);
	    stop     = (stop || stop2);
	    sleep_ag = (sleep_ag || sleep2);
	    if (!stop)
	      for (j = 0; j < nb_col; j++)
	        column[j].coef = aux_cost[j];
      }
    }
  } while((!stop) && (MaxExeTimeCron()));

  if (!stop)
    StopAteam(communicatorMD, communicatorMP);
  
  free(var_x);
  free(var_u);
  free(red_cost);
  free(cut_pos);
  free(S);
  free(G);
  free(C);
  free(aux_cost);
  free(P);
  free(t);
}

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */


