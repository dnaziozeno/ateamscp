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
/* Este arquivo contem o codigo necessario para a interface do agente primal */
/* greedy com o servidor de memoria de solucoes primais do ATeam  distribui- */
/* do para o Set Covering Problem.                                           */
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
       *unsat    = NULL;

MPI_Comm interfaceComm;

double *aux_cost = NULL;

void ExecAgPrimal(MPI_Comm communicatorMD, MPI_Comm communicatorMP);

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
       
  MPI_Comm commServerMD, commServerMP;          
  	            				            
  MPI_Init(&argc, &argv);               
  MPI_Comm_get_parent(&interfaceComm);
                                       
  /* Tetativa de conectar com o servidor */
  flagConnectMD = MPI_Lookup_name("ServerMD", MPI_INFO_NULL, portMD);
  if(flagConnectMD) 
  {
      printf("\n\n* Agente Primal Greedy finalizado *");
      printf("\n* ERRO : Servidor de solucoes duais nao iniciado. *\n");
      fflush(stdout);
  }	 
  else
  { 
  	 MPI_Comm_connect(portMD, MPI_INFO_NULL, 0, MPI_COMM_SELF, &commServerMD);
     flagConnectMP = MPI_Lookup_name("ServerMP", MPI_INFO_NULL, portMP);
     if(flagConnectMP) 
     { printf("\n\n* Agente Primal Greedy finalizado *");
	   printf("\n* ERRO : Servidor de solucoes primais nao iniciado. *\n");
     }
     else
     {  
     	MPI_Comm_connect(portMP, MPI_INFO_NULL, 0, MPI_COMM_SELF, &commServerMP);
/*
    TimeSleeping    = (int)  ReadAteamParam(1);
    MaxLenPrimalMem = (int)  ReadAteamParam(3);
    CutsofSol       = (int)  ReadAteamParam(6);
    MaxExeTime      = (int)  ReadAteamParam(11);
    RandomPrimal    = (char) ReadAteamParam(14);
*/

    TimeSleeping    = atoi(argv[2]);
    MaxLenPrimalMem = atoi(argv[3]);
    CutsofSol       = atoi(argv[4]);
    MaxExeTime      = atoi(argv[5]);
    RandomPrimal    = (char) atoi(argv[6]);

	    if (!(finput = fopen(argv[1],"r")))
	    { printf("\n\n* Erro na abertura do arquivo %s. *\n",argv[1]);
	      exit(1);
         }
	    ReadSource();
	    fclose(finput);
	    srand48(time(NULL));

	    ExecAgPrimal(commServerMD, commServerMP);

	    printf("\n\n* Agente Primal Greedy finalizado *");
	    printf("\n* Servidor finalizou processamento. *\n");
      }
   }
   
   int message = 1224;
   MPI_Send(&message, 1, MPI_INT, 0, 1, commServerMD);
   MPI_Send(&message, 1, MPI_INT, 0, 1, commServerMP);
   
   /* Envia mensagem de Finalizacao para interface */
   message = 1;
   MPI_Send(&message, 1, MPI_INT, 0, 1, interfaceComm);
  
   MPI_Comm_disconnect(&commServerMD);
   MPI_Comm_disconnect(&commServerMP);
   MPI_Finalize();
   
   printf("\n ==== ag_primal FINALIZADO \n", message);
}

/* ------------------------------------------------------------------------- */

void ExecAgPrimal(MPI_Comm communicatorMD, MPI_Comm communicatorMP)
{
  char        stop     = FALSE,
              sleep_ag = FALSE;
  int         i        = 0,
              cont     = 0,
             *cut_pos  = NULL,
             *var_x    = NULL;
  double      Z_aux    = 0,
             *var_u    = NULL,
             *red_cost = NULL;
  DualType    DualSol;
  PrimalType  PrimalSol;

  var_u           = (double *) malloc(max_lin * sizeof(double));
  red_cost        = (double *) malloc(nb_col * sizeof(double));
  aux_cost        = (double *) malloc(nb_col * sizeof(double));
  var_x           = (int *) malloc(nb_col * sizeof(int));
  U_x             = (int *) malloc((nb_lin + 1) * sizeof(int));
  unsat           = (int *) malloc(nb_lin * sizeof(int));
  cut_pos         = (int *) malloc(CutsofSol * sizeof(int));
  PrimalSol.var_x = (int *) malloc(nb_col * sizeof(int));
  Reduction(var_x,&Z_aux);

  int * bufferInterface;
  MPI_Request request;
  MPI_Status status;
  int count = 0;
  char change[1];
  int flag = 0, position = 0;
  int aux = -1;

  GetTimes(FALSE);
  do
   {
    MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, interfaceComm, &flag, &status);
    MPI_Get_count(&status, MPI_PACKED, &count);

    if(flag){
      
      bufferInterface = malloc(count);
      MPI_Recv(bufferInterface, count, MPI_PACKED, MPI_ANY_SOURCE, MPI_ANY_TAG, interfaceComm, &status);
      MPI_Unpack(bufferInterface, count, &position, &change[0], 1, MPI_CHAR, interfaceComm);
      
      if(change[0] == 'S'){
        return;
      }
      
      else if(change[0] == 'P'){

	    MPI_Unpack(bufferInterface, count, &position, &aux, 1, MPI_INT, interfaceComm);
	    if(aux != -1)
	      TimeSleeping = aux;
	    
	    aux = -1;
	    MPI_Unpack(bufferInterface, count, &position, &aux, 1, MPI_INT, interfaceComm);  
        if(aux != -1)
           MaxLenPrimalMem = aux;

      aux = -1;
	    MPI_Unpack(bufferInterface, count, &position, &aux, 1, MPI_INT, interfaceComm);  
        if(aux != -1)
          CutsofSol    = aux;
        
      aux = -1;
	    MPI_Unpack(bufferInterface, count, &position, &aux, 1, MPI_INT, interfaceComm);  
        if(aux != -1)
          MaxExeTime   = aux;
      
      aux = -1;
	    MPI_Unpack(bufferInterface, count, &position, &aux, 1, MPI_INT, interfaceComm);  
        if(aux != -1)
          RandomPrimal = (char) aux;
      
	  }
	  
      position = 0;
      flag = 0;
      free(bufferInterface);
    }

    if (sleep_ag)
       sleep(TimeSleeping);
     RequestDualSol(FALSE,&stop,LinearIncr,cut_pos,var_u,red_cost,
		    &DualSol,agent_free,communicatorMD);
     if (!stop)
     { PrimalSol.value = Z_aux;
	   memcpy(PrimalSol.var_x,var_x,nb_col * sizeof(int));
	   PrimalGreedy(RandomPrimal,&DualSol,&PrimalSol);
	   GetTimes(TRUE);
	   PrimalSol.proc_time = total_time_cpu - lost_time_cpu;
	   SendPrimalSolutiontoServer(&stop,&sleep_ag,&PrimalSol,communicatorMP);
       //printf("\n* Primal *");
     }
     
   } while((!stop) && (MaxExeTimeCron()));

  //if (!stop)
    //StopAteam(communicatorMD,communicatorMP);

  free(var_x);
  free(var_u);
  free(red_cost);
  free(aux_cost);
  free(U_x);
  free(unsat);
  free(cut_pos);
  free(PrimalSol.var_x);
}

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */
