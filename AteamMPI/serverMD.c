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
/* Este arquivo contem as definicoes do servidor MD, que controla a  memoria */
/* de solucoes duais do A-Team distribuido para o Set Covering Problem.      */
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */

#include "globais.h"   /* Inclusao das definicoes de tipos, macros e constan-*/
                       /* tes utilizadas em todos os arquivos do A-Team dis- */
                       /* tribuido para o Set Covering Problem.              */
#include "serverMD.h"  /* Inclusao das variaveis globais usadas pelo servidor*/
		               /* para controlar a memoria, os dados e arquivos de   */
		               /* estatisticas.                                      */

#include <pthread.h>
/*
void RequestDualSolution(char         stop_ateam,
			 DPSK_OBJECT *MethodParam,
			 DPSK_SHARED *call_ID);
void RequestBestDualSolution(char         stop_ateam,
			     DPSK_OBJECT *MethodParam,
			     DPSK_SHARED *call_ID);
void AgentSendDualSolution(char        *stop_ateam,
			   DPSK_OBJECT *MethodParam,
			   DPSK_SHARED *call_ID);
*/
void RequestEmptyMemory(char        *stop_ateam,
			            MPI_Comm    communicator);
			
/*void RequestCuts(char         stop_ateam,
		 DPSK_OBJECT *MethodParam,
		 DPSK_SHARED *call_ID);
*/ 
void RequestCutsofSol(char      stop_ateam,
		              char      * buffer, 
		              int       sizeOfBuffer, 
		              int       position, 
		              MPI_Comm  communicator);
/*
void AgentSendCuts(char         stop_ateam,
		   DPSK_OBJECT *MethodParam,
		   DPSK_SHARED *call_ID);
*/
void RequestDualSolution(char         stop_ateam,
                         char         * buffer,
                         int          sizeBuffer, 
                         int          position, 
				 	     MPI_Comm     communicator);			 
void AgentSendDualSolution(char *stop_ateam, char * buffer, int sizeBuffer,
                           int position, MPI_Comm communicator);
void InitGlobalVars(char *buffer, int position, MPI_Comm communicator);
	    
void ReturnResultsPath(char *path, MPI_Comm communicator);
void DumpMemory();
void WriteLowerBound();


 pthread_mutex_t mutex;
 pthread_mutexattr_t attr_mutex;


/* Argumentos passados para as threads de execucao */
typedef struct
{ MPI_Comm communicator;
  char * path1;
  char * path2;
  char * port;	
} ThreadParam;

/* Variaveis globais para o serverMD */
char stop_ateam = FALSE;
double time_out = TimeWaiting;

/* Thread de execução do serverMD - Trata das requisicoes dos agentes */
void *threadServerMD (void * argsStruct) {
  
  char * buffer;
  char * arg1;
  char * arg2;
  char *portMD;
  int insize          = 0;
  int flagAgentAction = 1;
  int method          = 0;
  int position        = 0;
  MPI_Status status;   
  ThreadParam  * threadParamStruct;
  
  /* Obtendo os parametros recebidos */    
  threadParamStruct = (ThreadParam *) argsStruct;
  MPI_Comm newCommClient = (MPI_Comm ) (threadParamStruct->communicator);
  //arg1 = (char *)threadParamStruct->path1;
  arg2 = (char *)threadParamStruct->path2;
  portMD = (char *)threadParamStruct->port;
  
  
  //printf("argumento %s\n", arg1);
  //printf("argumento %s\n", arg2);
  
  /* Laco para tratar a requisicao do agentes */   
  while(flagAgentAction){
    
    /* Recebendo os dados empacotados */
	MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, newCommClient, &status);
	MPI_Get_count(&status, MPI_PACKED, &insize);
	
	//printf("\n server md size %d", insize);	 
	buffer = malloc(insize);
	MPI_Recv(buffer, insize, MPI_PACKED, MPI_ANY_SOURCE, MPI_ANY_TAG, 
		                                         newCommClient, &status);
	MPI_Unpack(buffer, insize, &position, &method, 1, MPI_INT, 
		                                                  newCommClient);
		 
    if(method != 1224){
	  /* Tratamento dos metodos chamados */
	  switch (method)
	  { case REQUEST_SOLUTION :
		 { if (nb_dual_sol_mem > 0)
		     RequestDualSolution(stop_ateam, buffer, insize, position, newCommClient);
		   else
			 RequestEmptyMemory(&stop_ateam, newCommClient);
		   break;
		  }
		  case AGENT_SEND_SOLUTION :
		   {   
			 AgentSendDualSolution(&stop_ateam, buffer, insize, position, newCommClient);
			 break;
		   }
		  case REQUEST_CUTS :
		  { //RequestCuts(stop_ateam, MethodParam, newCommClient);
		    break;
		  }
		  case REQUEST_CUTS_OF_SOL :
		   {  
		  	 if (nb_cuts_mem > 0)
			   RequestCutsofSol(stop_ateam, buffer, insize, position, newCommClient);
			 else
		       RequestEmptyMemory(&stop_ateam, newCommClient);
			 break;
		   }
		  case AGENT_SEND_CUTS :
		   { //AgentSendCuts(stop_ateam,MethodParam,call_ID);
		     break;
		   }
		  case STOP_ATEAM :
		   { stop_ateam = TRUE;
			 break;
		   }
		  case INIT_GLOBAL_VARS :
		   { InitGlobalVars(buffer, position, newCommClient);
			 OpenFileStatistics(arg2);
			 break;
		   }
		  case DUMP_MEMORY :
		   { DumpMemory();
			 break;
		   }
		  case CHANGE_MEM_INSERTION :
		   { //DualWorst = (char) ReadAteamParam(9);
				   //call_ID -> ReturnMethod(NULL);
		     break;
		   }
		  case GET_RESULTS_PATH :
		   { ReturnResultsPath(arg2, newCommClient);
			 break;
		   }
		  case STOP_SERVER :
		   {
			 flagAgentAction = 0;
			 time_out = 0;
			 break;	 	
		   }	 
	  }
			  
    }//fim do if 1224
    else{
	 flagAgentAction = 0;
	}
	free(buffer);
	insize = 0;
	position = 0;

  }// fim do laço flag
   
  MPI_Close_port(portMD);
  MPI_Comm_disconnect(&newCommClient);
  free(threadParamStruct);
  printf ("\n\n === FIM THREAD!!!\n\n");
  pthread_exit(NULL);

} //Fim da funcao threadServerMD
/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
   O primeiro parametro que e' passado para o servidor e' o numero do proce-
   ssador em que o servidor vai ser executado e o segundo e' o diretorio  em
   que serao gerados os arquivos estatisticas.
   ------------------------------------------------------------------------- */
main(int argc, char *argv[])
{
 /* Variáveis */	
 int size, rank;
 int provided;
 
 
 
 MPI_Init_thread(NULL, NULL, MPI_THREAD_MULTIPLE, &provided );
 MPI_Comm_size(MPI_COMM_WORLD, &size);
 MPI_Comm_rank(MPI_COMM_WORLD, &rank);	
  										
 char server_name[256];             
 char portMD[MPI_MAX_PORT_NAME];    
 
 /* Iniciando o servidor ServerMD */
 MPI_Open_port(MPI_INFO_NULL, portMD);
 strcpy(server_name, "ServerMD");
 MPI_Publish_name(server_name, MPI_INFO_NULL, portMD);
  
 /* ------------------------------------------------------------------------- */
 /* Enviando mensagem para avisar q o servidor está liberado */ 
  	
 MPI_Comm parentcomm;
 char message[1];
 MPI_Comm_get_parent( &parentcomm );
 strcpy(message, "");
 MPI_Send(message , strlen(message), MPI_CHAR, 0, 1, parentcomm);
  
 /* ------------------------------------------------------------------------- */
 
 srand48(time(0)); /* Dúvida: saber exatamente o que faz */
  
 /* Le os valores dos parametros do ATeam e inicia a estrutura da tabela de
    "hash" que sera a memoria de cortes.                                      */
 InitParamHashTable();                             /* Localizada em dualrot.c */

 pthread_t threads[100];
 int t =0;
 int rc;

 ThreadParam * listParam = NULL;
 MPI_Comm * newCommClient;
 pthread_attr_t attr;

 pthread_attr_init(&attr);
 pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
 
 

 
 pthread_mutexattr_init(&attr_mutex);
 pthread_mutexattr_setprotocol(&attr_mutex, 0);
 pthread_mutex_init(&mutex, &attr_mutex);
 /* Inicio do loop "infinito" de execucao do servidor */

 while (time_out > 10)
 {  
   printf ("\n Servidor MD a espera de Conexão \n");
   
   newCommClient = malloc(sizeof(MPI_Comm));
   /* A espera de uma conexao */
   MPI_Comm_accept(portMD, MPI_INFO_NULL, 0, MPI_COMM_SELF, newCommClient);
            
   listParam = malloc(sizeof(ThreadParam));       
   listParam->communicator = *newCommClient;
   listParam->path2 = argv[1];
   //listParam->path1 = argv[2];
   //listParam->path2 = argv[4];
   listParam->port = portMD;
    
   /* Criando a thread para tratar a requisicao do agente conectado */
   rc = pthread_create(&threads[t], &attr, threadServerMD, (void *) listParam);
   if (rc){
     printf("ERROR; return code from pthread_create() is %d\n", rc);
     exit(-1);
   }
   t++;
  
  } /* Final do LOOP "infinito" do servidor */
  
  WriteStatistics(nb_dual_sol_mem - 1);
  DumpMemory();
  WriteLowerBound();
  fclose(fstts);
  fclose(fdump);

  
  
     
  MPI_Unpublish_name(server_name, MPI_INFO_NULL, portMD);
  MPI_Finalize();
  if(pthread_mutex_destroy(&mutex) != 0)
     perror("mutex destroy");
  printf("\n\n * Servidor de memoria de solucoes duais finalizado ... * \n\n");
  printf("* Melhor limite inferior obtido = %f. *\n", DualMem[nb_dual_sol_mem-1].value);
}

/* ------------------------------------------------------------------------- */

void RequestDualSolution(char         stop_ateam,
                         char         * buffer, 
                         int          sizeBuffer,
                         int          position, 
				 	     MPI_Comm     communicator)
{
  char         best_sol    = FALSE,
               * bufferSend;              
  int          pos         = 0,
               probab      = 0,
               positionSend = 0,
               sizeMessage = 0;
  Agents       requester;
  
  /* Desempacotamento da requisicao enviada pelo agente */
  MPI_Unpack(buffer, sizeBuffer, &position, &best_sol, 1, MPI_CHAR, communicator); 
  if (!best_sol)
   { 
     MPI_Unpack(buffer, sizeBuffer, &position, &probab, 1, MPI_INT, communicator); 
     MPI_Unpack(buffer, sizeBuffer, &position, &requester, 1, MPI_UNSIGNED, communicator);
     pos    = ChoosePosition(probab, nb_dual_sol_mem); 
     do{
       pos  = ChoosePosition(probab, nb_dual_sol_mem);
     }while (DualMem[pos].agent[requester]);
   }
   
  /* Empacotamento da solucao a ser enviada ao agente que a requisitou */
 if (DualMem[pos].nb_cuts > 0)
    DualMem[pos].agent[requester] =  (requester == agent_3opt);
 
 sizeMessage = (2 * sizeof(char)) + (TotalAgents * sizeof(char)) + 	
               sizeof(int) + sizeof(long) + sizeof(double) + (nb_col * sizeof(double)) +
               ((nb_lin + CutsofSol) * sizeof(double)) + 
               sizeof(unsigned int);  			    
 
 if(DualMem[pos].nb_cuts > 0)
 	//sizeMessage = sizeMessage + (DualMem[pos].nb_cuts * sizeof(int));
 	sizeMessage = sizeMessage + ( CutsofSol * sizeof(int));
 bufferSend = malloc(sizeMessage);
   
 MPI_Pack(&stop_ateam, 1, MPI_CHAR, bufferSend, sizeMessage, &positionSend, communicator);
 MPI_Pack(&DualMem[pos].improved_sol, 1, MPI_CHAR, bufferSend, sizeMessage, &positionSend, communicator);
 MPI_Pack(DualMem[pos].agent, TotalAgents, MPI_CHAR, bufferSend, sizeMessage, &positionSend, communicator);
 MPI_Pack(&DualMem[pos].nb_cuts, 1, MPI_INT, bufferSend, sizeMessage, &positionSend, communicator);
 
 if (DualMem[pos].nb_cuts > 0){
      MPI_Pack(DualMem[pos].cut, CutsofSol, MPI_INT, bufferSend, sizeMessage, &positionSend, communicator);
 }
 
 MPI_Pack(&DualMem[pos].proc_time, 1, MPI_LONG, bufferSend, sizeMessage, &positionSend, communicator); 
 MPI_Pack(&DualMem[pos].value, 1, MPI_DOUBLE, bufferSend, sizeMessage, &positionSend, communicator);
 
 MPI_Pack(DualMem[pos].red_cost, nb_col, MPI_DOUBLE, bufferSend, sizeMessage, &positionSend, communicator);
 
 
 int sizeVar_u = 0;
 //sizeVar_u = nb_lin + DualMem[pos].nb_cuts;
 sizeVar_u = nb_lin + CutsofSol;
 
 MPI_Pack(DualMem[pos].var_u, sizeVar_u, MPI_DOUBLE, bufferSend, sizeMessage, &positionSend, communicator);
 MPI_Pack(&DualMem[pos].agent_ID, 1, MPI_UNSIGNED, bufferSend, sizeMessage, &positionSend, communicator);
 
 MPI_Send(bufferSend, positionSend, MPI_PACKED, 0, 1, communicator); 

 free(bufferSend);
}

/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */

void AgentSendDualSolution(char *stop_ateam, char * buffer, int sizeBuffer,
                           int position, MPI_Comm communicator)	
{
  char         stop_aux    = FALSE,
               sleep_ag    = FALSE,
               new_problem = FALSE;
  int          size        = 0;             
  DualType     DualSol;
  void         *values      = NULL;
  
  
  size = nb_lin + CutsofSol;
  
  DualSol.var_u    = (double *) malloc(size * sizeof(double));
  DualSol.red_cost = (double *) malloc(nb_col * sizeof(double));
  DualSol.cut = (int *) malloc(CutsofSol * sizeof(int));
  
  
  
  MPI_Unpack(buffer, sizeBuffer, &position, &stop_aux, 1, MPI_CHAR, communicator);
  *stop_ateam = (*stop_ateam || stop_aux);
  
  MPI_Unpack(buffer, sizeBuffer, &position, &DualSol.value, 1, MPI_DOUBLE, communicator);
  MPI_Unpack(buffer, sizeBuffer, &position, DualSol.red_cost, nb_col, MPI_DOUBLE, communicator);
  MPI_Unpack(buffer, sizeBuffer, &position, DualSol.var_u, nb_lin, MPI_DOUBLE, communicator);
  MPI_Unpack(buffer, sizeBuffer, &position, &DualSol.agent_ID, 1, MPI_UNSIGNED, communicator);
  MPI_Unpack(buffer, sizeBuffer, &position, &DualSol.improved_sol, 1, MPI_CHAR, communicator);
  MPI_Unpack(buffer, sizeBuffer, &position, DualSol.agent, TotalAgents, MPI_CHAR, communicator);
  MPI_Unpack(buffer, sizeBuffer, &position, &DualSol.nb_cuts, 1, MPI_INT, communicator);
  
  //printf("\n cut %d", DualSol.nb_cuts);
  if (DualSol.nb_cuts > 0)
   { 
       MPI_Unpack(buffer, sizeBuffer, &position, DualSol.cut, CutsofSol, MPI_INT, communicator);
   } 
  
  MPI_Unpack(buffer, sizeBuffer, &position, &DualSol.proc_time, 1, MPI_LONG, communicator);
   
  //  Insercao na memoria da solucao enviada pelo agente 
  pthread_mutex_lock(&mutex);
  InsertDualMem(&sleep_ag,&new_problem,&DualSol); //localizada em dualRot.c 
  pthread_mutex_unlock(&mutex);

  // Servidor retorna ao agente indicativo de finalizacao ou nao do agente 
  char * bufferConfirm;
  int positionConfirm = 0;
  
  bufferConfirm = malloc(3 * sizeof(char));
  

  MPI_Pack(stop_ateam, 1, MPI_CHAR, bufferConfirm, 3 * sizeof(char), &positionConfirm, communicator);
  MPI_Pack(&sleep_ag, 1, MPI_CHAR, bufferConfirm, 3 * sizeof(char), &positionConfirm, communicator);
  MPI_Pack(&new_problem, 1, MPI_CHAR, bufferConfirm, 3 * sizeof(char), &positionConfirm, communicator);

  MPI_Send(bufferConfirm, positionConfirm, MPI_PACKED, 0, 1, communicator);

  free(bufferConfirm);
  
}

/* ------------------------------------------------------------------------- */
/*
void RequestCuts(char         stop_ateam,
		 DPSK_OBJECT *MethodParam,
		 DPSK_SHARED *call_ID)
{
  short       *cut_set     = NULL;
  int          i           = 0,
               nb_cuts     = 0;
  CutType     *cut         = NULL;
  DPSK_SLOT   *Rec0        = NULL,
              *Rec1        = NULL,
              *Rec2        = NULL,
              *Rec3        = NULL;
  DPSK_OBJECT *ReturnParam = NULL;

  // Empacotamento dos cortes a serem enviados ao agente
  if (nb_cuts_mem >= (2 * CutsofSol))
   { cut = (CutType *) malloc(CutsofSol * sizeof(CutType));
     ChooseCuts(&nb_cuts,cut);
   }
  if (nb_cuts > 0)
   { cut_set = (short *) malloc(CutsofSol * nb_col * sizeof(short));
     for (i = 0; i < nb_cuts; i++)
       memcpy(&cut_set[i * nb_col],cut[i].coef,nb_col * sizeof(short));
     Rec0 = new DPSK_SLOT("Stop",(void *)(&stop_ateam),sizeof(char));
     Rec1 = new DPSK_SLOT("NCuts",(void *)(&nb_cuts),sizeof(int));
     Rec2 = new DPSK_SLOT("Cut",(void *)(cut),CutsofSol * sizeof(CutType));
     Rec3 = new DPSK_SLOT("Coef",(void *)(cut_set),
			  CutsofSol * nb_col * sizeof(short));
     ReturnParam = new DPSK_OBJECT("ReturnParam",Rec0,Rec1,Rec2,Rec3,NULL);
     call_ID -> ReturnMethod(ReturnParam);
     delete(ReturnParam);
     delete(Rec0);
     delete(Rec1);
     delete(Rec2);
     delete(Rec3);
     free(cut_set);
   }
  else
    call_ID->ReturnMethod(NULL);
  free(cut);
}
*/
/* ------------------------------------------------------------------------- */
void RequestCutsofSol(char      stop_ateam,
		              char      * buffer, 
		              int       sizeOfBuffer, 
		              int       position, 
		              MPI_Comm  communicator)
{ /*
  short       *cut_set     = NULL;
  int          i           = 0,
               nb_cuts     = 0,
              *cut         = NULL;
  CutType     *cut_of_sol  = NULL;

  MPI_Unpack(buffer, sizeOfBuffer, &position, &nb_cuts, 1, MPI_INT, communicator); 
  MPI_Unpack(buffer, sizeOfBuffer, &position, cut, 1, MPI_INT, communicator); 
  
  // Escolha dos cortes a serem enviados ao agente requisitante 
  cut_of_sol = (CutType *) malloc(CutsofSol * sizeof(CutType));
  for (i = 0; i < nb_cuts; i++)
    cut_of_sol[i] = (CutType) CutMem[cut[i]];

  // Empacotamento dos cortes a serem enviados ao agente
  cut_set = (short *) malloc(CutsofSol * nb_col * sizeof(short));
  for (i = 0; i < nb_cuts; i++)
    memcpy(&cut_set[i * nb_col],cut_of_sol[i].coef,nb_col * sizeof(short));
  
  char * bufferSend;
  int  sizeMessage = 0;
  int  positionSend = 0;
  
  sizeMessage = 3 * sizeof(char) + 2 * sizeof(short) + 4 * sizeof(int) +
                sizeof(long) + (CutsofSol * nb_col * sizeof(short)); 
  
  bufferSend = malloc(sizeMessage);
  
  MPI_Pack(&stop_ateam, 1, MPI_CHAR, bufferSend, sizeMessage, &positionSend, communicator);
  MPI_Pack(&cut_of_sol->used, 1, MPI_CHAR, bufferSend, sizeMessage, &positionSend, communicator);
  MPI_Pack(&cut_of_sol->chosen, 1, MPI_CHAR, bufferSend, sizeMessage, &positionSend, communicator);
  MPI_Pack(&cut_of_sol->rhs, 1, MPI_SHORT, bufferSend, sizeMessage, &positionSend, communicator);
  MPI_Pack(cut_of_sol->coef, 1, MPI_SHORT, bufferSend, sizeMessage, &positionSend, communicator);
  MPI_Pack(&cut_of_sol->col_cut, 1, MPI_INT, bufferSend, sizeMessage, &positionSend, communicator);
  MPI_Pack(&cut_of_sol->tab_pos, 1, MPI_INT, bufferSend, sizeMessage, &positionSend, communicator);
  MPI_Pack(&cut_of_sol->nb_sol, 1, MPI_INT, bufferSend, sizeMessage, &positionSend, communicator);
  MPI_Pack(&cut_of_sol->next, 1, MPI_INT, bufferSend, sizeMessage, &positionSend, communicator);
  MPI_Pack(&cut_of_sol->key, 1, MPI_LONG, bufferSend, sizeMessage, &positionSend, communicator);
  MPI_Pack(cut_set, CutsofSol * nb_col, MPI_SHORT, bufferSend, sizeMessage, &positionSend, communicator);
  
  MPI_Send(bufferSend, positionSend, MPI_PACKED, 0, 1, communicator); 
  
  free(bufferSend);
  free(cut_set);
  free(cut_of_sol);
  */ 
}

/* ------------------------------------------------------------------------- */
/*
void AgentSendCuts(char         stop_ateam,
		   DPSK_OBJECT *MethodParam,
		   DPSK_SHARED *call_ID)
{
  char         sleep_ag    = FALSE,
               stop_aux    = FALSE;
  short       *cut_set     = NULL;
  int          i           = 0,
               nb_cuts     = 0;
  CutType     *cut         = NULL,
              *new_cuts    = NULL;
  DPSK_SLOT    Rec1("NCuts"),
               Rec2("Cut"),
               Rec3("Coef"),
              *aux         = NULL,
              *ReturnStop  = NULL,
              *ReturnSleep = NULL;
  DPSK_OBJECT *ReturnParam = NULL;

  // Desempacotamento da solucao enviada pelo Agente 
  aux      = MethodParam -> GetSlot(&Rec1);
  nb_cuts  = *(int *)(aux -> GetValue());
  delete(aux);

  aux      = MethodParam -> GetSlot(&Rec2);
  new_cuts = (CutType *)(aux -> GetValue());
  delete(aux);

  aux      = MethodParam -> GetSlot(&Rec3);
  cut_set  = (short *)(aux -> GetValue());
  delete(aux);

  while ((i < nb_cuts) && (!sleep_ag))
   { cut       = (CutType *) malloc(sizeof(CutType));
     *cut      = new_cuts[i];
     cut->coef = (short *) malloc(nb_col * sizeof(short));
     memcpy(cut->coef,&cut_set[i * nb_col],nb_col * sizeof(short));
     if (!InsertCutMem(&sleep_ag,cut))
      { free(cut->coef);
	free(cut);
      }
     i++;
   }

  stop_aux = stop_ateam;
  if (nb_cuts_mem == MaxLenCutMem)
    stop_aux = TRUE;
  // Servidor retorna ao agente indicativo de finalizacao ou nao do agente
  ReturnStop  = new DPSK_SLOT("Stop",(void *)(&stop_aux),sizeof(char));
  ReturnSleep = new DPSK_SLOT("Sleep",(void *)(&sleep_ag),sizeof(char));
  ReturnParam = new DPSK_OBJECT("ReturnParam",ReturnStop,ReturnSleep,NULL);
  call_ID -> ReturnMethod(ReturnParam);
  delete(ReturnParam);
  delete(ReturnStop);
  delete(ReturnSleep);
}
*/
/* ------------------------------------------------------------------------- */

void RequestEmptyMemory(char *stop_ateam, MPI_Comm communicator)
{
  // Servidor sinaliza ao agente para finalizar, pois a memoria esta vazia
  *stop_ateam = TRUE;
    
  char * buffer;
  int position = 0;
  buffer = malloc(sizeof(char));
 
  MPI_Pack(&stop_ateam, 1, MPI_CHAR, buffer, sizeof(char), &position, communicator);
  MPI_Send(buffer, position, MPI_PACKED, 0, 1, communicator); 
  
  free(buffer);

  
}
/* ------------------------------------------------------------------------- */

void InitGlobalVars(char *buffer, int position, MPI_Comm communicator)
{
 
  /* Desempacotamento dos valores das variaveis globais */
  MPI_Unpack(buffer, 3 * sizeof(int) + sizeof(float) + sizeof(unsigned long), 
  	&position, &nb_lin, 1, MPI_INT, communicator);
  MPI_Unpack(buffer, 3 * sizeof(int) + sizeof(float) + sizeof(unsigned long), 
  	&position, &nb_col, 1, MPI_INT, communicator);
  MPI_Unpack(buffer, 3 * sizeof(int) + sizeof(float) + sizeof(unsigned long), 
  	&position, &density, 1, MPI_FLOAT, communicator);
  MPI_Unpack(buffer, 3 * sizeof(int) + sizeof(float) + sizeof(unsigned long), 
  	&position, &NbServMD, 1, MPI_UNSIGNED_LONG, communicator);
  
  
  return;
}

/* ------------------------------------------------------------------------- */
void ReturnResultsPath(char *path, MPI_Comm communicator)
{
  int  path_len    = 0;
  char * message;
  int  position = 0;
  
  path_len    = (int) strlen(path);
  message = malloc((path_len * sizeof(char)) + sizeof(int));
  
  
  MPI_Pack(&path_len, 1, MPI_INT, message, ((path_len * sizeof(char)) + sizeof(int)), &position, communicator);
  MPI_Pack(path, path_len, MPI_CHAR, message, ((path_len * sizeof(char)) + sizeof(int)), &position, communicator);
  
  
  MPI_Send(message, position, MPI_PACKED, 0, 1, communicator);
  
  free(message);
 
}
/* ------------------------------------------------------------------------- */

void DumpMemory()
{
  int    i = 0;
  struct tm      day;
  struct timeval now;

  gettimeofday(&now,NULL);
  day = *localtime(&(now.tv_sec));

  WriteStatistics(nb_dual_sol_mem - 1);
  fprintf(fdump,"\n\n%02i:%02i:%02i\n\n",day.tm_hour,day.tm_min,day.tm_sec);
  fprintf(fdump,"\n\nNr. de cortes na memoria = %i\n\n",nb_cuts_mem);
  fprintf(fdump,"\n\nNr. de cortes propostos  = %i\n\n",proposed_cuts);
  for (i = 0; i < nb_dual_sol_mem; i++)
    fprintf(fdump,"%3i = %f\n",i,DualMem[i].value);
  fflush(fdump);
}

/* ------------------------------------------------------------------------- */

void WriteLowerBound()
{
  int i   = 0,
      lin = 0,
      pos = 0;

  pos = nb_dual_sol_mem - 1;
  fprintf(fdump,"\n\n\nZlb = %f\n\n",DualMem[pos].value);
  fprintf(fdump,"Nr. cortes = %i\n\n",DualMem[pos].nb_cuts); 
  lin = nb_lin + DualMem[pos].nb_cuts;
  for (i = 0; i < lin; i++)
    fprintf(fdump,"%4i = %f\n",i,DualMem[pos].var_u[i]);
}

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */
