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
/* Este arquivo contem o codigo necessario para a interface do agente random */
/* adjust com o servidor de memoria de solucoes primais do ATeam  distribui- */
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

int *aux_x = NULL,
    *cover = NULL;

void ExecAgAdjust(MPI_Comm communicator);

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
	  flagConnectMP = 1;
	  
  char portMP[MPI_MAX_PORT_NAME];
       
  MPI_Comm interfaceComm, commServerMP;          
  	            				            
  MPI_Init(&argc, &argv);               
  MPI_Comm_get_parent(&interfaceComm);

  flagConnectMP = MPI_Lookup_name("ServerMP", MPI_INFO_NULL, portMP);
  if(flagConnectMP) 
  {
    printf("\n\n* Agente Random Adjust finalizado *");
    printf("\n* ERRO : Servidor de solucoes primais nao iniciado. *\n");
  }
  else
  { 
   	 MPI_Comm_connect(portMP, MPI_INFO_NULL, 0, MPI_COMM_SELF, &commServerMP);
   	 TimeSleeping    = ReadAteamParam(1);
     MaxLenPrimalMem = ReadAteamParam(3);
     MaxExeTime      = ReadAteamParam(11);
     if (!(finput = fopen(argv[2],"r")))
      { printf("\n\n* Erro na abertura do arquivo %s. *\n",argv[2]);
	    exit(1);
      }
     ReadSource();
     fclose(finput);
     Reduction(NULL,NULL);
     srand48(time(NULL));
     MinChange = (int) ((ReadAteamParam(8) / 100.) * nb_col);

     ExecAgAdjust(commServerMP);

     printf("\n\n* Agente Random Adjust finalizado *");
     printf("\n* Servidor finalizou processamento. *\n");
  }
   
  /* Finaliza comunicacao com o servidor */
  int message = 1224;
  MPI_Send(&message, 1, MPI_INT, 0, 1, commServerMP);
   
  /* Envia mensagem de Finalizacao para interface */
  //message = 1;
  //MPI_Send(&message, 1, MPI_INT, 0, 1, interfaceComm);
 
  MPI_Comm_disconnect(&commServerMP);
  MPI_Finalize();
   
  printf("\n ==== ag_adjust FINALIZADO!!! \n", message);
}

/* ------------------------------------------------------------------------- */

void ExecAgAdjust(MPI_Comm communicator)
{
  char        stop     = FALSE,
              sleep_ag = FALSE;
  int         cont     = 0,
             *var_x    = NULL;
  PrimalType  PrimalSol;

  var_x = (int *) malloc(nb_col * sizeof(int));
  aux_x = (int *) malloc(nb_col * sizeof(int));
  cover = (int *) malloc(nb_lin * sizeof(int));

  GetTimes(FALSE);
  do
  { if (sleep_ag)
       sleep(TimeSleeping);
     RequestPrimalSol(FALSE,&stop,Constant,var_x,&PrimalSol,communicator);
     if (!stop)
     { RandomAdjust(&PrimalSol);
	   GetTimes(TRUE);
	   PrimalSol.proc_time = total_time_cpu - lost_time_cpu;
	   SendPrimalSolutiontoServer(&stop,&sleep_ag,&PrimalSol,communicator);
       printf("\n adjust");
     }
   } while((!stop) && (MaxExeTimeCron()));

  if (!stop)
    StopAteam(NULL, communicator);

  free(var_x);
  free(aux_x);
  free(cover);
}

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */
