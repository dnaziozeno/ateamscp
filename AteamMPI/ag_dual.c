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
/* Este arquivo contem o codigo necessario para a interface do  agente  dual */
/* greedy com o servidor de memoria de solucoes duais do  ATeam  distribuido */
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

int    *I_u     = NULL,
       *inact   = NULL;
double *delta_u = NULL;

MPI_Comm interfaceComm;

void ExecAgDual(MPI_Comm communicator);

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
  
  int flagConnect = 1;		  
  char port[MPI_MAX_PORT_NAME];   
  MPI_Comm commServerMD;          
  	            				            
  MPI_Init(&argc, &argv);               
  MPI_Comm_get_parent(&interfaceComm);
                                       
  /* Tetativa de conectar com o servidor */
  flagConnect = MPI_Lookup_name("ServerMD", MPI_INFO_NULL, port);
  if(flagConnect) 
  {
     printf("\n\n* Termino do Agente Dual Greedy *");
     printf("\n* ERRO : A memoria de solucoes duais nao foi iniciada. *");
     fflush(stdout);
  }	 
  else
   { 
/*
     TimeSleeping  = (int)  ReadAteamParam(1);
     MaxLenDualMem = (int)  ReadAteamParam(2);
     CutsofSol     = (int)  ReadAteamParam(6);
     MaxExeTime    = (int)  ReadAteamParam(11);
     ReducPerc     = (int)  ReadAteamParam(12);
     RandomDual    = (char) ReadAteamParam(17);
*/

     TimeSleeping  = atoi(argv[2]);
     MaxLenDualMem = atoi(argv[3]);
     CutsofSol     = atoi(argv[4]);
     MaxExeTime    = atoi(argv[5]);
     ReducPerc     = atoi(argv[6]);
     RandomDual    = (char) atoi(argv[7]);

     if (!(finput = fopen(argv[1],"r")))
     { printf("\n\n* Erro na abertura do arquivo %s. *\n",argv[1]);
	   exit(1);
     }
     ReadSource();
     fclose(finput);
     Reduction(NULL,NULL);
     srand48(time(NULL));

     MPI_Comm_connect(port, MPI_INFO_NULL, 0, MPI_COMM_SELF, &commServerMD);

     ExecAgDual(commServerMD);
     
     printf("\n\n* Agente Dual Greedy finalizado *");
     printf("\n* Servidor finalizou processamento. *\n");
   }

  /* Finaliza comunicacao com o servidor */
   int message = 1224;
   MPI_Send(&message, 1, MPI_INT, 0, 1, commServerMD);
   
   /* Envia mensagem de Finalizacao para interface */
   //message = 1;
   //MPI_Send(&message, 1, MPI_INT, 0, 1, interfaceComm);
   
  
   MPI_Comm_disconnect(&commServerMD);
   MPI_Finalize();
   printf("\n ==== ag_dual FINALIZADO \n", message);

}

/* ------------------------------------------------------------------------- */

void ExecAgDual(MPI_Comm communicator)
{
  char      stop        = FALSE,
            sleep_ag    = FALSE,
            new_problem = FALSE;
  int       i           = 0,
            nb_cuts     = 0;
  CutType  *cut         = NULL;
  DualType  DualSol;

  DualSol.var_u    = (double *) malloc(max_lin * sizeof(double));
  DualSol.red_cost = (double *) malloc(nb_col * sizeof(double));
  DualSol.cut      = (int *) malloc(CutsofSol * sizeof(int));
  delta_u          = (double *) malloc(max_lin * sizeof(double));
  I_u              = (int *) malloc((max_lin + 1) * sizeof(int));
  inact            = (int *) malloc(max_lin * sizeof(int));
  cut              = (CutType *) malloc(CutsofSol * sizeof(CutType));
  
  int * bufferInterface;
  MPI_Request request;
  MPI_Status status;
  int count = 0;
  char change[1];
  int flag = 0, position = 0;
  int aux = -1;

  for (i = 0; i < CutsofSol; i++)
    cut[i].coef = (short *) malloc(nb_col * sizeof(short));

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
           MaxLenDualMem = aux;

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
          ReducPerc = aux;

      aux = -1;
	    MPI_Unpack(bufferInterface, count, &position, &aux, 1, MPI_INT, interfaceComm);  
        if(aux != -1)
          RandomDual = (char) aux;
      
	   }
	  
      position = 0;
      flag = 0;
      free(bufferInterface);
     } 
     /* if (sleep_ag)
       sleep(TimeSleeping); */
     RequestNCutstoServer(&stop,&nb_cuts,cut,communicator);
     if (!stop)
     { if (nb_cuts > 0)
	     AddCutstoProblem(nb_cuts,cut,&DualSol);
       else
	     DualSol.nb_cuts = 0;
	   DualGreedy(RandomDual,((float) ReducPerc / 100.0),&DualSol);
	   GetTimes(TRUE);

	   DualSol.proc_time = total_time_cpu - lost_time_cpu;
	   //printf("droga");
	   SendDualSolutiontoServer(&stop,&sleep_ag,&new_problem,
				                &DualSol,communicator);
	   if (nb_cuts > 0)
	     ExcludeCutsofProblem(nb_cuts);
     }
     //printf("\n* Dual *");
   } while((!stop) && (MaxExeTimeCron()));

  //if (!stop)
    //StopAteam(communicator,NULL);
  
  free(DualSol.var_u);
  free(DualSol.red_cost);
  free(DualSol.cut);
  free(delta_u);
  free(I_u);
  free(inact);
  for (i = 0; i < CutsofSol; i++)
    free(cut[i].coef);
  free(cut);
}

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */
