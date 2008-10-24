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
/* Este arquivo contem o codigo necessario para a interface do  agente  sub- */
/* gradiente com o servidor de memoria de solucoes duais do ATeam distribui- */
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

int    *aux_x = NULL;
float  *grad  = NULL;
double *aux_u = NULL;

MPI_Comm interfaceComm;

void ExecAgSubgr(MPI_Comm communicatorMD, MPI_Comm communicatorMP);
void WriteProblem(char     *path,
		  		  int       nb_prob,
		          int      *lin_count,
		          DualType *DualSol);

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
      printf("\n\n* Agente Subgr finalizado *");
      printf("\n* ERRO : Servidor de solucoes DUAIS nao iniciado. *\n");
      fflush(stdout);
  }	 
  else
  { 
  	 MPI_Comm_connect(portMD, MPI_INFO_NULL, 0, MPI_COMM_SELF, &commServerMD);
     flagConnectMP = MPI_Lookup_name("ServerMP", MPI_INFO_NULL, portMP);
     if(flagConnectMP) 
     { printf("\n\n* Agente Subgradiente finalizado *");
	   printf("\n* ERRO : Servidor de solucoes duais nao iniciado. *\n");
     }
     else
     {  
      MPI_Comm_connect(portMP, MPI_INFO_NULL, 0, MPI_COMM_SELF, &commServerMP);

/*
      CutsofSol  = ReadAteamParam(6);
      MaxExeTime = ReadAteamParam(11);
*/

        CutsofSol       = atoi(argv[2]);
        MaxExeTime      = atoi(argv[3]);

	    if (!(finput = fopen(argv[1],"r")))
	    { printf("\n\n* Erro na abertura do arquivo %s. *\n",argv[1]);
	      exit(1);
         }
	    ReadSource();
	    fclose(finput);
	    Reduction(NULL,NULL);
	    srand48(time(NULL));

	    ExecAgSubgr(commServerMD, commServerMP);

	    printf("\n\n* Agente Subgradient finalizado *");
	    printf("\n* Servidor finalizou processamento. *\n");
      }
   }

   /* Finaliza comunicacao com o servidor */
   int message = 1224;
   MPI_Send(&message, 1, MPI_INT, 0, 1, commServerMD);
   MPI_Send(&message, 1, MPI_INT, 0, 1, commServerMP);
   
   /* Envia mensagem de Finalizacao para interface */
   message = 1;
   MPI_Send(&message, 1, MPI_INT, 0, 1, interfaceComm);
  
   MPI_Comm_disconnect(&commServerMD);
   MPI_Comm_disconnect(&commServerMP);
   MPI_Finalize();
   
   printf("\n ==== ag_subgr FINALIZADO \n", message);
}

/* ------------------------------------------------------------------------- */

void ExecAgSubgr(MPI_Comm communicatorMD, MPI_Comm communicatorMP)
{
  char        stop        = FALSE,
              sleep_ag    = FALSE,
              new_problem = FALSE,
              path[200];
  int         i           = 0,
              cont        = 0,
              nb_cuts     = 0,
              nb_write    = 0,
             *cut_pos     = NULL,
             *var_x       = NULL,
             *lin_count   = NULL;
  float       lambda      = 1.6,
              gap         = 0;
  double     *var_u       = NULL,
             *red_cost    = NULL;
  DualType    DualSol;
  PrimalType  PrimalSol;
  CutType    *cut         = NULL;

  lin_count = (int *) malloc(nb_col * sizeof(int));
  cut_pos   = (int *) malloc(CutsofSol * sizeof(int));
  var_x     = (int *) malloc(nb_col * sizeof(int));
  var_u     = (double *) malloc(max_lin * sizeof(double));
  red_cost  = (double *) malloc(nb_col * sizeof(double));
  aux_x     = (int *) malloc(nb_col * sizeof(int));
  aux_u     = (double *) malloc(max_lin * sizeof(double));
  grad      = (float *) malloc(max_lin * sizeof(float));
  cut       = (CutType *) malloc(CutsofSol * sizeof(CutType));
  for (i = 0; i < CutsofSol; i++)
    cut[i].coef = (short *) malloc(nb_col * sizeof(short));
  for (i = 0; i < nb_col; i++)
    lin_count[i] = column[i].elem_col;

  GetTimes(FALSE);
  GetResultsPath(path, communicatorMD);

  int * bufferInterface;
  MPI_Request request;
  MPI_Status status;
  int count = 0;
  char change[1];
  int flag = 0, position = 0;
  int aux = -1;

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
	      CutsofSol = aux;
	    
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
/*   { RequestDualSol(&stop,LinearDecr,cut_pos,var_u,red_cost, */
		    &DualSol,agent_subgrad,communicatorMD);
		    //printf("\n subgr dual %f", DualSol.value);
    if (!stop)
      { RequestPrimalSol(TRUE,&stop,0,var_x,&PrimalSol,communicatorMP);
	    //printf("\n subgr primal %f", PrimalSol.value);
        
        if (!stop)
	    { gap = (PrimalSol.value - DualSol.value) / PrimalSol.value;
	      if (gap <= epsilon)
	      { stop = TRUE;
	        StopAteam(communicatorMD,communicatorMP);
	      }
	      else
	      { if (DualSol.nb_cuts > 0)
	        { RequestCutsofSoltoServer(&stop,cut,&DualSol,communicatorMD);
		      if (!stop)
		      AddCutstoProblem(0,cut,&DualSol);
	        }
	        else
	        { RequestNCutstoServer(&stop,&nb_cuts,cut,communicatorMD);
		      if ((!stop) && (nb_cuts > 0))
		      { DualSol.cut = cut_pos;
		        AddCutstoProblem(nb_cuts,cut,&DualSol);
		      }
	        }
	        if (!stop)
	        { if ((gap >= 0.05) && (gap <= 0.1))
		       lambda = 0.75;
		      if (gap < 0.05)
		       lambda = 0.5;
		     SubGradiente(lambda,&DualSol,&PrimalSol);	
		     //printf("\n subgr SubGradiente dual %f", DualSol.value);
		     //printf("\n subgr SubGradiente primal %f", PrimalSol.value);
		     gap = (PrimalSol.value - DualSol.value) / PrimalSol.value;
		     if (gap <= epsilon)
		     { stop = TRUE;
		       StopAteam(NULL,communicatorMP);
		     }
		     GetTimes(TRUE);
		     DualSol.proc_time = total_time_cpu - lost_time_cpu;
		     SendDualSolutiontoServer(&stop,&sleep_ag,&new_problem,
					  &DualSol,communicatorMD);
		     if ((new_problem) && (DualSol.nb_cuts > 0))
		     { if (++nb_write < 50)
		       WriteProblem(path,nb_write,lin_count,&DualSol);
		     }
		     if (DualSol.nb_cuts > 0)
		      ExcludeCutsofProblem(DualSol.nb_cuts);
	       }
	     }
       }
      
      //printf("\n subgr");
      
      }
      
      
      
   } while((!stop) && (MaxExeTimeCron()));

 
  //StopAteam(communicatorMD, communicatorMP);
  free(cut_pos);
  free(var_u);
  free(red_cost);
  free(var_x);
  free(aux_x);
  free(aux_u);
  free(grad);
  for (i = 0; i < CutsofSol; i++)
    free(cut[i].coef);
  free(cut);
}

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */

void WriteProblem(char     *path,
		  int       nb_prob,
		  int      *lin_count,
		  DualType *DualSol)
{
  char   path_out[200],
         nb_out[20];
  int    j        = 0,
         nb_elem  = 0,
         orig_lin = 0;
  FILE  *foutput  = NULL;
  DList *Ij       = NULL;

  strcpy(path_out,path);
  sprintf(nb_out,"_%02i_%02i",nb_prob,DualSol->nb_cuts);
  strcat(path_out,nb_out);
  foutput = fopen(path_out,"w");
  fprintf(foutput,"%5d\n%5d\n",nb_lin,nb_col);

  orig_lin = nb_lin - DualSol->nb_cuts;
  for (j=0; j < nb_col; j++)
   { nb_elem = column[j].elem_col - lin_count[j];
     if (nb_elem > 0)
      { fprintf(foutput,"%5d %5d\n",j,nb_elem);
	Ij = column[j].first;
	while(Ij)
	 { if (Ij->lin >= orig_lin)
	     fprintf(foutput,"%5d\n",Ij->lin);
	    Ij = Ij->down;
	 }
      }
   }
  fclose(foutput);
}



/*
void WriteProblem(char     *path,
		  int       nb_prob,
		  int      *lin_count,
		  DualType *DualSol)
{
  char   path_out[200],
         nb_out[20];
  int    j        = 0;
  FILE  *foutput  = NULL;
  DList *Ij       = NULL;

  strcpy(path_out,path);
  sprintf(nb_out,"_%02i_%02i",nb_prob,DualSol->nb_cuts);
  strcat(path_out,nb_out);
  foutput = fopen(path_out,"w");
  fprintf(foutput,"%10ld\n%10ld\n",nb_lin,nb_col);

  for (j=0; j < nb_col; j++)
   { fprintf(foutput," %.14e %9ld\n",column[j].coef,column[j].elem_col);
     Ij = column[j].first;
     while(Ij)
      { fprintf(foutput,"%10ld    %.14e\n",Ij->lin+1,(float) Ij->coef);
	Ij = Ij->down;
      }
   }
  fclose(foutput);
}
*/
