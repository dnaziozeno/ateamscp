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
/* Este arquivo contem o codigo referente as rotinas que fazem a comunicacao */
/* com os servidores de memoria. Essas rotinas sao usadas nos varios agentes */
/* do ateam distribuido para o Set Covering Problem.                         */
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */

#include "globais.h"    /* Inclusao das definicoes de tipos, macros e  cons- */
			            /* tantes utilizadas em todos os arquivos  do A-Team */
                        /* distribuido para o Set Covering Problem.          */
#include "mpi.h"
extern int   nb_lin;
extern int   nb_col;
extern int   max_lin;
extern int   CutsofSol;
extern int   MaxCutGen;
extern int   MaxExeTime;
extern long  lost_time_cpu;
extern long  total_time_cpu;
extern long  lost_time_cron;
extern long  total_time_cron;
extern Lin  *line;
extern Col  *column;

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */
void RequestDualSol(char        best_sol,
		    char       *stop,
		    int         probab,
		    int        *cut,
		    double     *var_u,
		    double     *red_cost,
		    DualType   *DualSol,
		    Agents      requester,
		    MPI_Comm   communicator)
{
  void        *values      = NULL;
  int  positionSend = 0,
       method = REQUEST_SOLUTION, 
       sizeOfBuffer = 0;
  char * buffer;
  
  
  sizeOfBuffer =   (sizeof(int) + sizeof(char) + sizeof(int) + sizeof(unsigned int));
  buffer = malloc(sizeOfBuffer);
  
  MPI_Pack(&method, 1, MPI_INT, buffer, sizeOfBuffer, &positionSend, communicator);
  MPI_Pack(&best_sol, 1, MPI_CHAR, buffer, sizeOfBuffer, &positionSend, communicator);
  MPI_Pack(&probab, 1, MPI_INT, buffer, sizeOfBuffer, &positionSend, communicator);
  MPI_Pack(&requester, 1, MPI_UNSIGNED, buffer, sizeOfBuffer, &positionSend, communicator);
   
   
  MPI_Send(buffer, positionSend, MPI_PACKED, 0, 1, communicator); 
  
  free(buffer); 
  
  char * bufferRecv;
  int positionRecv = 0,
      sizeMessage;
  MPI_Status status;
  
  /* A espera das solucoes*/
  MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, communicator, &status);
  MPI_Get_count(&status, MPI_PACKED, &sizeMessage);
 
  bufferRecv = malloc(sizeMessage);
  MPI_Recv(bufferRecv, sizeMessage, MPI_PACKED, MPI_ANY_SOURCE, MPI_ANY_TAG, communicator, &status);
 
  /* Iniciando Desempacotamento */
  MPI_Unpack(bufferRecv, sizeMessage, &positionRecv, stop, 1, MPI_CHAR, communicator);
  
  
  if (!*stop)
   { 
   	  
     MPI_Unpack(bufferRecv, sizeMessage, &positionRecv, &DualSol->improved_sol, 1, MPI_CHAR, communicator);
     MPI_Unpack(bufferRecv, sizeMessage, &positionRecv, DualSol->agent, TotalAgents, MPI_CHAR, communicator);
     MPI_Unpack(bufferRecv, sizeMessage, &positionRecv, &DualSol->nb_cuts, 1, MPI_INT, communicator);
     
     if (DualSol->nb_cuts > 0){
     	 // nao MPI_Unpack(bufferRecv, sizeMessage, &positionRecv, DualSol->cut, DualSol->nb_cuts, MPI_INT, communicator);      
         //MPI_Unpack(bufferRecv, sizeMessage, &positionRecv, cut, DualSol->nb_cuts, MPI_INT, communicator);      
         MPI_Unpack(bufferRecv, sizeMessage, &positionRecv, cut, CutsofSol, MPI_INT, communicator);      
         DualSol->cut = cut;
     }
     MPI_Unpack(bufferRecv, sizeMessage, &positionRecv, &DualSol->proc_time, 1, MPI_LONG, communicator);
     MPI_Unpack(bufferRecv, sizeMessage, &positionRecv, &DualSol->value, 1, MPI_DOUBLE, communicator);
     
     
     //MPI_Unpack(bufferRecv, sizeMessage, &positionRecv, DualSol->red_cost, 1, MPI_DOUBLE, communicator); 
     //MPI_Unpack(bufferRecv, sizeMessage, &positionRecv, red_cost, 1, MPI_DOUBLE, communicator);
     MPI_Unpack(bufferRecv, sizeMessage, &positionRecv, red_cost, nb_col, MPI_DOUBLE, communicator);
     
     DualSol->red_cost = red_cost;
     
     int sizeVar_u = 0;
     //sizeVar_u = nb_lin + DualSol->nb_cuts;
     sizeVar_u = nb_lin + CutsofSol;
     
     //MPI_Unpack(bufferRecv, sizeMessage, &positionRecv, DualSol->var_u, sizeVar_u, MPI_DOUBLE, communicator);
     MPI_Unpack(bufferRecv, sizeMessage, &positionRecv, var_u, sizeVar_u, MPI_DOUBLE, communicator);
     DualSol->var_u = var_u;
     
     MPI_Unpack(bufferRecv, sizeMessage, &positionRecv, &DualSol->agent_ID, 1, MPI_UNSIGNED, communicator);
     
   }
   //printf("\n === Fim RequestDualSol");  
   free(bufferRecv);
   
}

/* ------------------------------------------------------------------------- */

void SendDualSolutiontoServer(char       *stop,
			      			  char       *sleep_ag,
			                  char       *new_problem,
			                  DualType   *DualSol,
			                  MPI_Comm   communicator)
{
  int position = 0;
  int sizeOfBuffer = 0;
  
  
  sizeOfBuffer = sizeof(int) +
  				 sizeof(char) + 
                 sizeof(double) +
                 nb_col * sizeof(double) +
                 max_lin * sizeof(double) + 
                 sizeof(unsigned int) + 
                 sizeof(char) + 
                 TotalAgents * sizeof(char) +
                 sizeof(int) +
                 sizeof(long);  
  if(DualSol->nb_cuts > 0) 
  	sizeOfBuffer = sizeOfBuffer + (CutsofSol * sizeof(int));
  
  char * buffer;
  int  method = AGENT_SEND_SOLUTION;
  //printf("\n\n size buffer agentrot : %d \n\n", sizeOfBuffer);
  
 
  buffer = malloc(sizeOfBuffer);
  
  
  
  MPI_Pack(&method, 1, MPI_INT, buffer, sizeOfBuffer, &position, communicator);
  MPI_Pack(stop, 1, MPI_CHAR, buffer, sizeOfBuffer, &position, communicator);
  MPI_Pack(&DualSol->value, 1, MPI_DOUBLE, buffer, sizeOfBuffer, &position, communicator);
  MPI_Pack(DualSol->red_cost, nb_col, MPI_DOUBLE, buffer, sizeOfBuffer, &position, communicator);
  MPI_Pack(DualSol->var_u, nb_lin, MPI_DOUBLE, buffer, sizeOfBuffer, &position, communicator);
  MPI_Pack(&DualSol->agent_ID, 1, MPI_UNSIGNED, buffer, sizeOfBuffer, &position, communicator);
  MPI_Pack(&DualSol->improved_sol, 1, MPI_CHAR, buffer, sizeOfBuffer, &position, communicator);
  MPI_Pack(DualSol->agent, TotalAgents, MPI_CHAR, buffer, sizeOfBuffer, &position, communicator);
  MPI_Pack(&DualSol->nb_cuts, 1, MPI_INT, buffer, sizeOfBuffer, &position, communicator);
  
  //printf("\n cut %d", DualSol->nb_cuts);
  if (DualSol->nb_cuts > 0)
  {  	
   	 MPI_Pack(DualSol->cut, CutsofSol, MPI_DOUBLE, buffer, sizeOfBuffer, &position, communicator);
  }
  //else DualSol->cut = NULL;
   
  MPI_Pack(&DualSol->proc_time, 1, MPI_LONG, buffer, sizeOfBuffer, &position, communicator); 
  MPI_Send(buffer, position, MPI_PACKED, 0, 1, communicator);
  
  //printf("Enviado pacote root \n\n");

  free(buffer);
  
  
  // O servidor de memoria de solucoes duais retorna um indicativo de finali-
  // zacao ou nao do agente.
  char messageConfirm[3 * sizeof(char)];
  MPI_Status status;
  int positionConfirm = 0;
  
  //printf("root antes recv \n\n");
  
  MPI_Recv(messageConfirm, 3, MPI_PACKED, MPI_ANY_SOURCE, MPI_ANY_TAG, communicator, &status);
  
  //printf("root depois do revc  \n\n");
  MPI_Unpack(messageConfirm, 3 * sizeof(char), &positionConfirm, stop, 1, MPI_CHAR, communicator);
  MPI_Unpack(messageConfirm, 3 * sizeof(char), &positionConfirm, sleep_ag, 1, MPI_CHAR, communicator);
  MPI_Unpack(messageConfirm, 3 * sizeof(char), &positionConfirm, new_problem, 1, MPI_CHAR, communicator);
 
  //printf("\n\n === Fim agentrot SendDual \n\n");
}

/* ------------------------------------------------------------------------- */
void RequestPrimalSol(char        best_sol,
		      char       *stop,
		      int         probab,
		      int        *var_x,
		      PrimalType *PrimalSol,
		      MPI_Comm communicator)
{
  void        *values      = NULL;
  int method = REQUEST_SOLUTION,
      sizeOfBuffer = 0,
      position = 0;
             
  char *buffer;
  
  
  sizeOfBuffer = ((sizeof(int))  + (sizeof(char)) + 
                  (sizeof(int))); 
  
  buffer = malloc(sizeOfBuffer);
  
  MPI_Pack(&method, 1, MPI_INT, buffer, sizeOfBuffer, &position, communicator);
  MPI_Pack(&best_sol, 1, MPI_CHAR, buffer, sizeOfBuffer, &position, communicator);
  MPI_Pack(&probab, 1, MPI_INT, buffer, sizeOfBuffer, &position, communicator);

  MPI_Send(buffer, position, MPI_PACKED, 0, 1, communicator);
  

  free(buffer);
  
  
  MPI_Status status;
  int positionRecv = 0;
  int sizeOfMessage = 0;
  
  MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, communicator, &status);
  MPI_Get_count(&status, MPI_PACKED, &sizeOfMessage);
  
  
  buffer = malloc(sizeOfMessage);
  
  
  MPI_Recv(buffer, sizeOfMessage, MPI_PACKED, MPI_ANY_SOURCE, MPI_ANY_TAG, communicator, &status);
  
  MPI_Unpack(buffer, sizeOfMessage, &positionRecv, stop, 1, MPI_CHAR, communicator);
  
  if (!*stop)
  { MPI_Unpack(buffer, sizeOfMessage, &positionRecv, PrimalSol->agent, TotalAgents, MPI_CHAR, communicator);
    MPI_Unpack(buffer, sizeOfMessage, &positionRecv, var_x, nb_col, MPI_INT, communicator);
    PrimalSol->var_x = var_x;
    MPI_Unpack(buffer, sizeOfMessage, &positionRecv, &PrimalSol->proc_time, 1, MPI_INT, communicator);
    MPI_Unpack(buffer, sizeOfMessage, &positionRecv, &PrimalSol->value, 1, MPI_DOUBLE, communicator);
    MPI_Unpack(buffer, sizeOfMessage, &positionRecv, &PrimalSol->agent_ID, 1, MPI_UNSIGNED, communicator);
  }
  free(buffer);
  //printf("\n\n === Fim o empacotamento em agentrot RequestPrimal \n\n");
}

/* ------------------------------------------------------------------------- */
void SendPrimalSolutiontoServer(char       *stop,
				                char       *sleep_ag,
				                PrimalType *PrimalSol,
				                MPI_Comm communicator)
{
 
  int method = AGENT_SEND_SOLUTION,
      sizeOfBuffer = 0,
      position = 0;
             
  char *buffer;
  
  
  sizeOfBuffer = ( (sizeof(int)) + 
                   ((sizeof(char)) * TotalAgents) + 
                   ((sizeof(int)) * nb_col) +
                   (sizeof(int)) + 
                   (sizeof(double)) + 
                   sizeof(unsigned int)
                  ); 
   
  buffer = malloc(sizeOfBuffer);
  
  MPI_Pack(&method, 1, MPI_INT, buffer, sizeOfBuffer, &position, communicator);
  MPI_Pack(PrimalSol->agent, TotalAgents, MPI_CHAR, buffer, sizeOfBuffer, &position, communicator);
  MPI_Pack(PrimalSol->var_x, nb_col, MPI_INT, buffer, sizeOfBuffer, &position, communicator);
  MPI_Pack(&PrimalSol->proc_time, 1, MPI_INT, buffer, sizeOfBuffer, &position, communicator);
  MPI_Pack(&PrimalSol->value, 1, MPI_DOUBLE, buffer, sizeOfBuffer, &position, communicator);  
  MPI_Pack(&PrimalSol->agent_ID, 1, MPI_UNSIGNED, buffer, sizeOfBuffer, &position, communicator);
  
  // O agente envia a solucao gerada ou modificada por um dos agentes primais
  //para o servidor de memoria de solucoes primais. 
  
  MPI_Send(buffer, position, MPI_PACKED, 0, 1, communicator);
  
  
  char messageConfirm[2 * sizeof(char)];
  MPI_Status status;
  int positionConfirm = 0;
  
  MPI_Recv(messageConfirm, 2, MPI_PACKED, MPI_ANY_SOURCE, MPI_ANY_TAG, communicator, &status);
  MPI_Unpack(messageConfirm, 2 * sizeof(char), &positionConfirm, stop, 1, MPI_CHAR, communicator);
  MPI_Unpack(messageConfirm, 2 * sizeof(char), &positionConfirm, sleep_ag, 1, MPI_CHAR, communicator);
  
  free(buffer);
  //printf("\n\n === Fim agentrot SendPrimal \n\n");
}
/* ------------------------------------------------------------------------- */

void RequestNCutstoServer(char       *stop,
			  		      int        *nb_cuts,
			  			  CutType    *cut,
			              MPI_Comm   communicator)
{
	*nb_cuts    = 0;
  /*short       *coef        = NULL,
              *cut_set     = NULL;
  int          i           = 0;
  CutType     *new_cuts    = NULL;
  DPSK_SLOT    method("Function",REQUEST_CUTS),
               Rec0("Stop"),
               Rec1("NCuts"),
               Rec2("Cut"),
               Rec3("Coef"),
              *aux         = NULL;
  DPSK_OBJECT *MethodParam = NULL,
              *Result      = NULL;

  *nb_cuts    = 0;

  // O agente requisita ao servidor de cortes um certo nr. de cortes 
  MethodParam = new DPSK_OBJECT ("Param", &method, NULL);
  Result      = ServMDClass -> CallMethod ("TALK", MethodParam);
  if (Result)
   { aux         = Result -> GetSlot (&Rec0);
     *stop       = *(char *) (aux -> GetValue());
     delete(aux);
     if (!*stop)
      { aux      = Result -> GetSlot (&Rec1);
	*nb_cuts = *(int *) (aux -> GetValue());
	delete(aux);

	aux      = Result -> GetSlot (&Rec2);
	new_cuts = (CutType *) (aux -> GetValue());
	delete(aux);

	aux      = Result -> GetSlot (&Rec3);
	cut_set  = (short *) (aux -> GetValue());
	delete(aux);
	for (i = 0; i < *nb_cuts; i++)
	 { coef        = cut[i].coef;
	   memcpy(coef,&cut_set[i * nb_col],nb_col * sizeof(short));
	   cut[i]      = new_cuts[i];
	   cut[i].coef = coef;
         }
      }
     delete(Result);
   }
  delete(MethodParam);
  */ 
}

/* ------------------------------------------------------------------------- */
void RequestCutsofSoltoServer(char       *stop,
			      CutType    *cut,
			      DualType   *DualSol,
			      MPI_Comm   communicator)
{
  /*short       *coef        = NULL,
              *cut_set     = NULL;
  int          i           = 0;
  CutType     *new_cuts    = NULL;
  
  */
  /*
  char * buffer,
       * bufferRecv;
  int  position = 0,
       positionRecv = 0;
  int  sizeOfBuffer = 0,
       sizeMessage = 0;  
  int  method = REQUEST_CUTS_OF_SOL;
  MPI_Status status;
  
  sizeOfBuffer = (2 * sizeof(int)) + (CutsofSol * sizeof(int)); 
  buffer = malloc(sizeOfBuffer);
  
  printf("\n ==== agentrot: Iniciando o empacotamento em RequestCutsofSoltoServer \n\n");
  MPI_Pack(&method, 1, MPI_INT, buffer, sizeOfBuffer, &position, communicator);
  MPI_Pack(&(DualSol->nb_cuts), 1, MPI_INT, buffer, sizeOfBuffer, &position, communicator);
  MPI_Pack(DualSol->cut, 1, MPI_INT, buffer, sizeOfBuffer, &position, communicator);
  MPI_Send(buffer, position, MPI_PACKED, 0, 1, communicator); 
  
  free(buffer);
  */
  /* A espera das solucoes*/
  /* MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, communicator, &status);
  MPI_Get_count(&status, MPI_PACKED, &sizeMessage);
  bufferRecv = malloc(sizeMessage);
  */
  
  /*
   * 
  MPI_Pack(&stop_ateam, 1, MPI_INT, bufferSend, sizeMessage, &positionSend, communicator);
  MPI_Pack(&cut_of_sol->used, 1, MPI_CHAR, bufferSend, sizeMessage, &positionSend, communicator);
  MPI_Pack(&cut_of_sol->chosen, 1, MPI_CHAR, bufferSend, sizeMessage, &positionSend, communicator);
  MPI_Pack(&cut_of_sol->rhs, 1, MPI_SHORT, bufferSend, sizeMessage, &positionSend, communicator);
  MPI_Pack(cut_of_sol->coef, 1, MPI_SHORT, bufferSend, sizeMessage, &positionSend, communicator);
  MPI_Pack(&cut_of_sol->col_cut, 1, MPI_INT, bufferSend, sizeMessage, &positionSend, communicator);
  MPI_Pack(&cut_of_sol->tab_pos, 1, MPI_INT, bufferSend, sizeMessage, &positionSend, communicator);
  MPI_Pack(&cut_of_sol->nb_sol, 1, MPI_INT, bufferSend, sizeMessage, &positionSend, communicator);
  MPI_Pack(&cut_of_sol->next, 1, MPI_INT, bufferSend, sizeMessage, &positionSend, communicator);
  MPI_Pack(&cut_of_sol->key, 1, MPI_INT, bufferSend, sizeMessage, &positionSend, communicator);
  MPI_Pack(cut_set, CutsofSol * nb_col, MPI_SHORT, bufferSend, sizeMessage, &positionSend, communicator);
  
   * */
  /*
   * MPI_Unpack(bufferRecv, sizeMessage, &positionRecv, stop, 1, MPI_CHAR, communicator);
  if (!*stop)
  {
  	MPI_Unpack(bufferRecv, sizeMessage, &positionRecv, stop, 1, MPI_CHAR, communicator);
  	MPI_Unpack(bufferRecv, sizeMessage, &positionRecv, &new_cuts->used, 1, MPI_CHAR, communicator);
    MPI_Unpack(bufferRecv, sizeMessage, &positionRecv, &new_cuts->chosen, 1, MPI_CHAR, communicator);
    MPI_Unpack(bufferRecv, sizeMessage, &positionRecv, &new_cuts->rhs, 1, MPI_SHORT, communicator);
    MPI_Unpack(bufferRecv, sizeMessage, &positionRecv, new_cuts->coef, 1, MPI_SHORT, communicator);
    MPI_Unpack(bufferRecv, sizeMessage, &positionRecv, &new_cuts->col_cut, 1, MPI_INT, communicator);
    MPI_Unpack(bufferRecv, sizeMessage, &positionRecv, &new_cuts->tab_pos, 1, MPI_INT, communicator);
    MPI_Unpack(bufferRecv, sizeMessage, &positionRecv, &new_cuts->nb_sol, 1, MPI_INT, communicator);
    MPI_Unpack(bufferRecv, sizeMessage, &positionRecv, &new_cuts->next, 1, MPI_INT, communicator);
    MPI_Unpack(bufferRecv, sizeMessage, &positionRecv, &new_cuts->key, 1, MPI_LONG, communicator);
    MPI_Unpack(bufferRecv, sizeMessage, &positionRecv, cut_set, CutsofSol * nb_col, MPI_SHORT, communicator);
  
    for (i = 0; i < DualSol->nb_cuts; i++)
    { 
        coef = cut[i].coef;
	    memcpy(coef, &cut_set[i * nb_col], nb_col * sizeof(short));
	    cut[i]      = new_cuts[i];
	    cut[i].coef = coef;
    }
  
  }
  
  free(bufferRecv);
  // O servidor de cortes retorna ao agente os cortes requisitados ou um indi-
  // cativo de finalizacao do agente 
  /*aux   = Result -> GetSlot (&RRec0);
  *stop = *(char *) (aux -> GetValue());
  delete(aux);
  if (!*stop)
   { aux      = Result -> GetSlot (&RRec1);
     new_cuts = (CutType *) (aux -> GetValue());
     delete(aux);
     aux     = Result -> GetSlot (&RRec2);
     cut_set = (short *) (aux -> GetValue());
     delete(aux);
     for (i = 0; i < DualSol->nb_cuts; i++)
      { coef = cut[i].coef;
	memcpy(coef,&cut_set[i * nb_col],nb_col * sizeof(short));
	cut[i]      = new_cuts[i];
	cut[i].coef = coef;
      }
   }
  delete(MethodParam);
  delete(Result);
  delete(Rec1);
  delete(Rec2);
  */ 
}

/* ------------------------------------------------------------------------- */
/*
void SendCutstoServer(char       *stop,
		      char       *sleep_ag,
		      int         nb_cuts,
		      CutType    *cut,
		      DPSK_CLASS *ServMDClass)
{
  short       *cut_set     = NULL;
  int          i           = 0;
  DPSK_SLOT    method("Function",AGENT_SEND_CUTS),
               ReturnStop("Stop"),
               ReturnSleep("Sleep"),
              *Rec1        = NULL,
              *Rec2        = NULL,
              *Rec3        = NULL,
              *aux         = NULL;
  DPSK_OBJECT *MethodParam = NULL,
              *Result      = NULL;

  //O agente envia os novos cortes gerados pelo procedimento GenCut para o
     servidor de memoria de cortes. 
  cut_set = (short *) malloc(MaxCutGen * nb_col * sizeof(short));
  for (i = 0; i < nb_cuts; i++)
    memcpy(&cut_set[i * nb_col],cut[i].coef,nb_col * sizeof(short));

  Rec1 = new DPSK_SLOT("NCuts",(void *)(&nb_cuts),sizeof(int));
  Rec2 = new DPSK_SLOT("Cut",(void *)(cut),MaxCutGen * sizeof(CutType));
  Rec3 = new DPSK_SLOT("Coef",(void *)(cut_set),
		       MaxCutGen * nb_col * sizeof(short));
  MethodParam = new DPSK_OBJECT("Param", &method, Rec1, Rec2, Rec3, NULL);
  Result = ServMDClass->CallMethod("TALK",MethodParam);

  // O servidor de memoria de cortes retorna um indicativo de finalizacao ou
     nao do agente. 
  aux       = Result -> GetSlot(&ReturnStop);
  *stop     = *(char *) (aux -> GetValue());
  delete(aux);
  aux       = Result -> GetSlot(&ReturnSleep);
  *sleep_ag = *(char *) (aux -> GetValue());
  delete(aux);

  delete(MethodParam);
  delete(Result);
  delete(Rec1);
  delete(Rec2);
  delete(Rec3);
  free(cut_set);
}

/* ------------------------------------------------------------------------- */

void AddCutstoProblem(int       new_cuts,
		      CutType  *cut,
		      DualType *DualSol)
{/*
  int    j   = 0,
         k   = 0;
  DList *aux = NULL;

  if (new_cuts > 0)
    DualSol->nb_cuts = new_cuts;
  for (k = 0; k < DualSol->nb_cuts; k++)
   { if (new_cuts > 0)
       DualSol->cut[k] = cut[k].tab_pos;
     line[nb_lin] = (Lin) {NULL,NULL,FALSE,cut[k].rhs,cut[k].col_cut};
     for (j = 0; j < nb_col; j++)
      { if (cut[k].coef[j] == 0)
	  continue;
	aux  = (DList *) malloc(sizeof(DList));
	*aux = (DList) {NULL,NULL,NULL,cut[k].coef[j],nb_lin,j};
	if (line[nb_lin].first)
	  line[nb_lin].last->right = aux;
	else
	  line[nb_lin].first = aux;
	line[nb_lin].last = aux;
	if (column[aux->col].first)
	  column[aux->col].last->down = aux;
	else
	  column[aux->col].first = aux;
	aux->up = column[aux->col].last;
	column[aux->col].last = aux;
	column[aux->col].elem_col++;
      }
     nb_lin++;
   }
   */ 
}

/* ------------------------------------------------------------------------- */

void ExcludeCutsofProblem(int nb_cuts)
{
 /* int    i   = 0,
         end = 0;
  DList *Ji  = NULL,
        *aux = NULL;

  end = nb_lin - nb_cuts;
  for (i = nb_lin-1; i >= end; i--)
   { Ji = line[i].first;
     while (Ji)
      { aux = Ji;
	Ji  = Ji->right;
	if (aux->up)
	  aux->up->down = NULL;
	column[aux->col].last = aux->up;
	column[aux->col].elem_col--;
	free(aux);
      }
   }
  nb_lin = end;
  */ 
}

/* ------------------------------------------------------------------------- */

void StopAteam(MPI_Comm commMD, MPI_Comm commMP)
{
  int  method = 0,
       position = 0;
  char bufferMD[10],
       bufferMP[10];
  
  method = STOP_ATEAM;
  
  if (commMD){
    MPI_Pack(&method, 1, MPI_INT, bufferMD, 10, &position, commMD);
    MPI_Send(bufferMD, position, MPI_PACKED, 0, 1, commMD);
  }
  position = 0;
  if (commMP){
    MPI_Pack(&method, 1, MPI_INT, bufferMP, 10, &position, commMP);
    MPI_Send(bufferMP, position, MPI_PACKED, 0, 1, commMP);
  }
}

/* ------------------------------------------------------------------------- */
/*
void FinishInitServer(DPSK_CLASS *ServMemClass)
{
  DPSK_SLOT    method("Function",CHANGE_MEM_INSERTION);
  DPSK_OBJECT *MethodParam = NULL,
              *Result      = NULL;

  MethodParam = new DPSK_OBJECT("Param", &method, NULL);
  Result      = ServMemClass->CallMethod("TALK",MethodParam);
  delete(MethodParam);
  delete(Result);
}

/* ------------------------------------------------------------------------- */
void GetResultsPath(char *path, MPI_Comm communicator)
{
  char *path_aux    = NULL;
  int  path_len    = 0,
       method = GET_RESULTS_PATH;
  
  char buffer[sizeof(int)]; 
  int  position = 0;
  MPI_Status status;
  
  
  MPI_Pack(&method, 1, MPI_INT, buffer, sizeof(int), &position, communicator);
  MPI_Send(buffer, position, MPI_PACKED, 0, 1, communicator);
  
  //printf("\n == first \n");
  
  char * bufferRecv;
  int positionRecv = 0;
  int sizeOfMessage = 0;
  
  MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, communicator, &status);
  MPI_Get_count(&status, MPI_PACKED, &sizeOfMessage);
  
  
  bufferRecv = malloc(sizeOfMessage);
  
  MPI_Recv(bufferRecv, sizeOfMessage, MPI_PACKED, MPI_ANY_SOURCE, MPI_ANY_TAG, communicator, &status);
  MPI_Unpack(bufferRecv, sizeOfMessage, &positionRecv, &path_len, 1, MPI_INT, communicator);
  
  path_aux =  malloc(sizeof(char) * path_len);
  MPI_Unpack(bufferRecv, sizeOfMessage, &positionRecv, path_aux, path_len, MPI_CHAR, communicator);
  
  strncpy(path, path_aux,path_len);
  
  free(bufferRecv);
  free(path_aux);
}

/* ------------------------------------------------------------------------- */

void GetTimes(char AddTime)
{
  long int tt_sec;
  struct rusage ProcTime;
  
  getrusage(RUSAGE_SELF,&ProcTime);
  tt_sec = ProcTime.ru_utime.tv_sec;
  if (!AddTime)
    lost_time_cpu += (tt_sec - total_time_cpu);
  total_time_cpu = tt_sec;
}

/* ------------------------------------------------------------------------- */

void LocalTime(long *cron_time)
{
  struct tm      day;
  struct timeval now;

//HHHH  gettimeofday(&now);
  gettimeofday(&now,NULL);
  day        = *localtime(&(now.tv_sec));
  *cron_time = (long) (day.tm_sec + (60 * day.tm_min) + (3600 * day.tm_hour));
}

/* ------------------------------------------------------------------------- */

char MaxExeTimeCron()
{
  LocalTime(&total_time_cron);
  if (total_time_cron < lost_time_cron)
    (total_time_cron += 86400);
  total_time_cron -= lost_time_cron;
  return(total_time_cron < MaxExeTime);
}

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */
