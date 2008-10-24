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
/* Este arquivo contem o codigo necessario para a interface do agente three- */
/* opt com o servidor de memoria de solucoes duais do ATeam distribuido para */
/* o Set Covering Problem.                                                   */
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */

#include "globais.h"    /* Inclusao das definicoes de tipos, macros e  cons- */
			/* tantes utilizadas em todos os arquivos  do A-Team */
                        /* distribuido para o Set Covering Problem.          */
#include "agentes.h"    /* Inclusao das definicoes de variaveis usadas em to-*/
                        /* os agentes do ATeam distribuido.                  */


char *exclude = NULL;
int  *disj    = NULL,
     *set1    = NULL,
     *set2    = NULL;
MPI_Comm interfaceComm;

void ExecAg3Opt(MPI_Comm communicator);

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
      printf("\n\n* Agente ThreeOpt finalizado *");
      printf("\n* ERRO : Servidor de solucoes duais nao iniciado. *\n");
      fflush(stdout);
  }	 
  else
   { TimeSleeping = ReadAteamParam(1);
     CutsofSol    = ReadAteamParam(6);
     MaxExeTime   = ReadAteamParam(11);
     if (!(finput = fopen(argv[2],"r")))
      { printf("\n\n* Erro na abertura do arquivo %s. *\n",argv[2]);
	    exit(1);
      }
     ReadSource();
     fclose(finput);
     Reduction(NULL,NULL);
     srand48(time(NULL));

     MPI_Comm_connect(port, MPI_INFO_NULL, 0, MPI_COMM_SELF, &commServerMD);

     ExecAg3Opt(commServerMD);

     printf("\n\n* Agente ThreeOpt finalizado *");
     printf("\n* Servidor finalizou processamento. *\n");
   }
   /* Finaliza comunicacao com o servidor */
   int message = 1224;
   MPI_Send(&message, 1, MPI_INT, 0, 1, commServerMD);
   
   /* Envia mensagem de Finalizacao para interface */
   message = 1;
   MPI_Send(&message, 1, MPI_INT, 0, 1, interfaceComm);
   
  
   MPI_Comm_disconnect(&commServerMD);
   MPI_Finalize();
   printf("\n ==== ag_3opt FINALIZADO \n", message);
}

/* ------------------------------------------------------------------------- */

void ExecAg3Opt(MPI_Comm communicator)
{
  char      stop        = FALSE,
            sleep_ag    = FALSE,
            new_problem = FALSE;
  int       i           = 0,
            nb_cuts     = 0,
           *cut_pos     = NULL;
  double    aux_sol     = 0,
           *var_u       = NULL,
           *red_cost    = NULL;
  DualType  DualSol;
  CutType  *cut         = NULL;

  exclude  = (char *) malloc(nb_col * sizeof(char));
  cut_pos  = (int *) malloc(CutsofSol * sizeof(int));
  disj     = (int *) malloc(max_lin * sizeof(int));
  set1     = (int *) malloc(nb_col * sizeof(int));
  set2     = (int *) malloc(nb_col * sizeof(int));
  var_u    = (double *) malloc(max_lin * sizeof(double));
  red_cost = (double *) malloc(nb_col * sizeof(double));
  cut      = (CutType *) malloc(CutsofSol * sizeof(CutType));
  for (i = 0; i < CutsofSol; i++)
    cut[i].coef = (short *) malloc(nb_col * sizeof(short));


  int * bufferInterface;
  MPI_Request request;
  MPI_Status status;
  int count = 0;
  char change[1];
  int flag = 0, position = 0, *ptr = NULL;
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
          CutsofSol    = aux;
        
        aux = -1;
	    MPI_Unpack(bufferInterface, count, &position, &aux, 1, MPI_INT, interfaceComm);  
        if(aux != -1)
          MaxExeTime   = aux;
      
	  }
	  
      position = 0;
      flag = 0;
      free(bufferInterface);
    }
   	
   	RequestDualSol(FALSE,&stop,Constant,cut_pos,var_u,red_cost,
		    	   &DualSol, agent_3opt, communicator);  
     if (!stop)
     { if (DualSol.nb_cuts > 0)
	 	{   RequestCutsofSoltoServer(&stop, cut, &DualSol, communicator);
	   		if (!stop)
	     	AddCutstoProblem(0,cut,&DualSol);
         }
		if (!stop)
	 	{ 	aux_sol = DualSol.value;
	   		ThreeOpt(&DualSol);
	   		if (DualSol.value > aux_sol)
	    	{ GetTimes(TRUE);
	      	  DualSol.proc_time = total_time_cpu - lost_time_cpu;
	      	  SendDualSolutiontoServer(&stop,&sleep_ag,&new_problem,
				       &DualSol,communicator);
	    	}
	   		if (DualSol.nb_cuts > 0)
	     		ExcludeCutsofProblem(DualSol.nb_cuts);
        }
     }
     //return;
   } while((!stop) && (MaxExeTimeCron()));
   
  if (!stop)
    StopAteam(communicator,NULL);
  
  free(exclude);
  free(cut_pos);
  free(disj);
  free(set1);
  free(set2);
  free(var_u);
  free(red_cost);
  for (i = 0; i < CutsofSol; i++)
    free(cut[i].coef);
  free(cut);
}

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */
