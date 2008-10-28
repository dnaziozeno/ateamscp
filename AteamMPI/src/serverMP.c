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
/* Este arquivo contem as definicoes do servidor MP, que controla a  memoria */
/* de solucoes primais do A-Team distribuido para o Set Covering Problem.    */
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */

#include "globais.h"   /* Inclusao das definicoes de tipos, macros e constan-*/
                       /* tes utilizadas em todos os arquivos do A-Team dis- */
                       /* tribuido para o Set Covering Problem.              */
#include "serverMP.h"  /* Inclusao das variaveis globais usadas pelo servidor*/
		               /* para controlar a memoria, os dados e arquivos de   */
		               /* estatisticas.                                      */
#include <pthread.h>
/*
void RequestBestPrimalSolution(char         stop_ateam,
			       DPSK_OBJECT *MethodParam,
			       DPSK_SHARED *call_ID);
*/
void RequestEmptyPrimalMemory(char *stop_ateam, MPI_Comm communicator);
void RequestPrimalSolution(char         stop_ateam,
                           char         * buffer,
                           int          sizeBuffer, 
                           int          position, 
				 	       MPI_Comm     communicator);
void AgentSendPrimalSolution(char stop_ateam, char * buffer, int sizeBuffer,
                                       int position, MPI_Comm communicator);
void InitGlobalVars(char *buffer, int position, MPI_Comm communicator);
void DumpMemory();
void WriteUpperBound();

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */


 pthread_mutex_t mutex;
 pthread_mutexattr_t attr_mutex;




/* Argumentos passados para as threads de execucao */
typedef struct
{ MPI_Comm communicator;
  char * path1;
  char * path2;
  char * port;	
} ThreadParam;

char stop_ateam = FALSE;
double time_out   = TimeWaiting;

/* Thread de execução do serverMP - Trata das requisicoes dos agentes */
void *threadServerMP (void * argsStruct) {
  
  char * buffer;
  char * arg1;
  char * arg2;
  char *portMP;
  int insize          = 0;
  int flagAgentAction = 1;
  int method          = 0;
  int position        = 0;
  MPI_Status status;   
  ThreadParam  * threadParamStruct;
  MPI_Datatype type = MPI_PACKED;
  int sizePacked = 0;
  
  /* Obtendo os parametros recebidos */    
  threadParamStruct = (ThreadParam *) argsStruct;
  MPI_Comm newCommClient = (MPI_Comm ) (threadParamStruct->communicator);
  //arg1 = (char *)threadParamStruct->path1;
  arg2 = (char *)threadParamStruct->path1;
  portMP = (char *)threadParamStruct->port;
  
  /* Laco para tratar a requisicao do agentes */   
  while(flagAgentAction){
    
    /* Recebendo os dados empacotados */
	MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, newCommClient, &status);
	MPI_Get_count(&status, MPI_PACKED, &insize);
	
	MPI_Type_size(type, &sizePacked);
	//printf("\n ==== sizePacked %d", sizePacked);	 
	//printf("\n ==== size %d", insize * sizePacked);	 
	buffer = malloc(insize * sizePacked);
	MPI_Recv(buffer, insize, MPI_PACKED, MPI_ANY_SOURCE, MPI_ANY_TAG, 
		                                         newCommClient, &status);
	MPI_Unpack(buffer, insize, &position, &method, 1, MPI_INT, 
		                                                  newCommClient);
		 
    if(method != 1224){
    /* Tratamento dos metodos chamados */
     switch (method)
     { case REQUEST_SOLUTION :
	   { if (nb_primal_sol_mem > 0)
	     RequestPrimalSolution(stop_ateam, buffer, insize, position, newCommClient);
	     else
	       RequestEmptyPrimalMemory(&stop_ateam, newCommClient);
	     break;
       }
       case AGENT_SEND_SOLUTION :
	   { AgentSendPrimalSolution(stop_ateam, buffer, insize, position, newCommClient);
	     break;
       }
       case STOP_ATEAM :
	   { stop_ateam = TRUE;
	     //call_ID -> ReturnMethod(NULL);
	     break;
       }
       case INIT_GLOBAL_VARS :
	   { InitGlobalVars(buffer, position, newCommClient);
	     OpenFileStatistics(arg2);
	     break;
       }
       case DUMP_MEMORY :
	   { DumpMemory();
	     //call_ID -> ReturnMethod(NULL);
	     break;
       }
       case CHANGE_MEM_INSERTION :
	   { PrimalWorst = (char) ReadAteamParam(10);
	     //call_ID -> ReturnMethod(NULL);
	     break;
       }
       case STOP_SERVER :
	   {
	     flagAgentAction = 0;
		 time_out = 0;
		 break;	 	
	   }
     }/* Fim switch */
     
   }/* Fim if 1224 */ 
   else
   {  
     flagAgentAction = 0;
   }
   free(buffer);
   insize = 0;
   position = 0;
  }/* Fim while */
  
  MPI_Close_port(portMP);
  MPI_Comm_disconnect(&newCommClient);
  printf ("\n\n === FIM THREAD!!!\n\n");
  pthread_exit(NULL);

}/* Fim funcao thread */

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
  
  char  stop_ateam = FALSE;
  int   method     = 0;

  char * buffer;		             
  int position = 0;	                 					          
  MPI_Status status;
  char server_name[256];             
  char portMP[MPI_MAX_PORT_NAME];    
 
  /* Iniciando o servidor ServerMP */
  MPI_Open_port(MPI_INFO_NULL, portMP);
  strcpy(server_name, "ServerMP");
  MPI_Publish_name(server_name, MPI_INFO_NULL, portMP);
  
  /* ------------------------------------------------------------------------- */
  /* Enviando mensagem para avisar q o servidor está liberado */ 
  	
  MPI_Comm parentcomm;
  char message[1];
  MPI_Comm_get_parent( &parentcomm );
  strcpy(message, "");
  MPI_Send(message , strlen(message), MPI_CHAR, 0, 1, parentcomm);
  
 /* ------------------------------------------------------------------------- */

  srand48(time(0));
   
  /* Leitura dos valores dos parametros do A-Team */
  InitAteamParam(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));

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
     printf ("\n Servidor MP a espera de Conexão \n");
     newCommClient = malloc(sizeof(MPI_Comm));
     /* A espera de uma conexao */
     MPI_Comm_accept(portMP, MPI_INFO_NULL, 0, MPI_COMM_SELF, newCommClient);     
            
     listParam = malloc(sizeof(ThreadParam));       
     listParam->communicator = *newCommClient;
     listParam->path1 = argv[1];
     //listParam->path2 = argv[4];
     listParam->port = portMP;
    
     /* Criando a thread para tratar a requisicao do agente conectado */
     rc = pthread_create(&threads[t], &attr, threadServerMP, (void *) listParam);
     if (rc){
       printf("ERROR; return code from pthread_create() is %d\n", rc);
       exit(-1);
     }
     t++;
     
   } /* Final do LOOP "infinito" do servidor */
  
  
  WriteStatistics(0);
  WriteUpperBound();
  fclose(fstts);
  fclose(fdump);

  MPI_Unpublish_name(server_name, MPI_INFO_NULL, portMP);
  MPI_Finalize();
  if(pthread_mutex_destroy(&mutex) != 0)
     perror("mutex destroy");
  printf("\n\n* Servidor de memoria de solucoes primais finalizado ... *\n\n");
  printf("* Melhor limite superior obtido = %f. *\n", PrimalMem[0].value);
  
}

/* ------------------------------------------------------------------------- */
void RequestPrimalSolution(char         stop_ateam,
                           char         * buffer,
                           int          sizeBuffer, 
                           int          position, 
				 	       MPI_Comm     communicator)
{
  char         best_sol    = FALSE;
  int          pos         = 0,
               probab      = 0;
  
  MPI_Unpack(buffer, sizeBuffer, &position, &best_sol, 1, MPI_CHAR, communicator); 
  if (!best_sol)
  { 
    MPI_Unpack(buffer, sizeBuffer, &position, &probab, 1, MPI_INT, communicator); 
    pos    = ChoosePosition(probab,nb_primal_sol_mem);
  }
  
  char * bufferSolutions;
  int positionSend = 0,
      sizeOfMessage = 0;
      
      
  sizeOfMessage = ((sizeof(char)) + (sizeof(char) * TotalAgents) +
                   (sizeof(int) * nb_col) + sizeof(int) + sizeof(double) +
                   sizeof(unsigned int));    
  ("\n === RESQUEST PRIMAL SIZE 1 %d", sizeOfMessage);                 
  bufferSolutions = malloc(sizeOfMessage);    
  //pthread_mutex_lock (&mutex) ;
  MPI_Pack(&stop_ateam, 1, MPI_CHAR, bufferSolutions, sizeOfMessage, &positionSend, communicator);
  MPI_Pack(PrimalMem[pos].agent, TotalAgents, MPI_CHAR, bufferSolutions, sizeOfMessage, &positionSend, communicator);
  MPI_Pack(PrimalMem[pos].var_x, nb_col, MPI_INT, bufferSolutions, sizeOfMessage, &positionSend, communicator);
  MPI_Pack(&PrimalMem[pos].proc_time, 1, MPI_INT, bufferSolutions, sizeOfMessage, &positionSend, communicator);
  MPI_Pack(&PrimalMem[pos].value, 1, MPI_DOUBLE, bufferSolutions, sizeOfMessage, &positionSend, communicator);
  MPI_Pack(&PrimalMem[pos].agent_ID, 1, MPI_UNSIGNED, bufferSolutions, sizeOfMessage, &positionSend, communicator);
  MPI_Send(bufferSolutions, positionSend, MPI_PACKED, 0, 1, communicator);
  //pthread_mutex_lock (&mutex) ;
  free(bufferSolutions);
  
}

/* ------------------------------------------------------------------------- */
void AgentSendPrimalSolution(char         stop_ateam,
			     			 char         * buffer, 
                         	 int          sizeBuffer,
                         	 int          position, 
				 	         MPI_Comm     communicator)
{
  char         sleep_ag    = FALSE;
  int          ins         = 0,
               pos         = 0;
  PrimalType   PrimalSol;
  void        *values      = NULL;
 
 
  PrimalSol.var_x = (int *) malloc(nb_col * sizeof(int));
   MPI_Unpack(buffer, sizeBuffer, &position, PrimalSol.agent, TotalAgents, MPI_CHAR, communicator);
   MPI_Unpack(buffer, sizeBuffer, &position,   PrimalSol.var_x, nb_col, MPI_INT, communicator);
   MPI_Unpack(buffer, sizeBuffer, &position, &PrimalSol.proc_time, 1, MPI_INT, communicator);
   MPI_Unpack(buffer, sizeBuffer, &position, &PrimalSol.value, 1, MPI_DOUBLE, communicator);
   MPI_Unpack(buffer, sizeBuffer, &position, &PrimalSol.agent_ID, 1, MPI_UNSIGNED, communicator);
  
  //memcpy(PrimalSol.var_x,values,nb_col * sizeof(int));
  
  /*  Insercao na memoria da solucao enviada pelo agente */
      pthread_mutex_lock (&mutex) ;
      InsertPrimalMem(&sleep_ag, &PrimalSol);
      pthread_mutex_unlock (&mutex) ;
 
  /* Servidor retorna ao agente indicativo de finalizacao ou nao do agente */ 
  char * bufferConfirm;
  int positionConfirm = 0;
  
  bufferConfirm = malloc(2 * sizeof(char));
  MPI_Pack(&stop_ateam, 1, MPI_CHAR, bufferConfirm, 3 * sizeof(char), &positionConfirm, communicator);
  MPI_Pack(&sleep_ag, 1, MPI_CHAR, bufferConfirm, 3 * sizeof(char), &positionConfirm, communicator);

  MPI_Send(bufferConfirm, positionConfirm, MPI_PACKED, 0, 1, communicator);

  free(bufferConfirm); 
}

/* ------------------------------------------------------------------------- */
void RequestEmptyPrimalMemory(char *stop_ateam, MPI_Comm communicator)
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
  
  int size = 3 * sizeof(int) + sizeof(float) + sizeof(unsigned long);
  
  /* Desempacotamento dos valores das variaveis globais */
  MPI_Unpack(buffer, size, &position, &nb_lin, 1, MPI_INT, communicator);
  MPI_Unpack(buffer, size, &position, &nb_col, 1, MPI_INT, communicator);
  MPI_Unpack(buffer, size, &position, &density, 1, MPI_FLOAT, 
                                                           communicator);
  MPI_Unpack(buffer, size, &position, &NbServMP, 1, MPI_UNSIGNED_LONG, 
                                                           communicator);
  
  
  return;
}

/* ------------------------------------------------------------------------- */

void DumpMemory()
{
  int    i = 0;
  struct tm      day;
  struct timeval now;

//HHHH  gettimeofday(&now);
  gettimeofday(&now,NULL);
  day = *localtime(&(now.tv_sec));

  WriteStatistics(0);
  fprintf(fdump,"\n\n%02i:%02i:%02i\n\n",day.tm_hour,day.tm_min,day.tm_sec);
  for (i = 0; i < nb_primal_sol_mem; i++)
    fprintf(fdump,"%3i = %f\n",i,PrimalMem[i].value);
  fflush(fdump);
}

/* ------------------------------------------------------------------------- */

void WriteUpperBound()
{
  int i = 0;

  fprintf(fdump,"\n\n\nZub = %f\n\n",PrimalMem[0].value);
  for (i = 0; i < nb_col; i++)
    if (PrimalMem[0].var_x[i] == 1)
      fprintf(fdump,"%i  ",i);
}

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */

