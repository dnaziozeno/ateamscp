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
/* Este arquivo contem o codigo necessario para o agente StopATEAM.  Este a- */
/* gente avisa aos servidores de solucoes duais e primais que os agentes que */
/* se comunicam com ele nao devem mais retirar solucoes da memoria.  Os ser- */
/* vidores vao entao iniciar um processo sucessivo de divisao  por  dois  do */
/* tempo de espera do servidor por chamadas de um dos seus metodos.          */
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */

#include "globais.h"   /* Inclusao das definicoes de tipos, macros e constan-*/
                       /* tes utilizadas em todos os arquivos do A-Team dis- */
                       /* tribuido para o Set Covering Problem.              */

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
   O unico parametro a ser passado para esse agente e' o numero do processa-
   dor em que esta executando o 'ns'.
   ------------------------------------------------------------------------- */
   
#include <mpi.h>

main(int argc,char *argv[])
{
  int size,          
      rank;
  MPI_Init(&argc, &argv);               
  MPI_Comm_size(MPI_COMM_WORLD, &size); 
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
      
  int method = 0,
      flagConnectMD = 1,
      flagConnectMP = 1,
      position = 0;
       
  char  portMD[MPI_MAX_PORT_NAME],
        portMP[MPI_MAX_PORT_NAME],
        bufferMD[100],
        bufferMP[100];
       
  MPI_Comm commServerMD;
  MPI_Comm commServerMP;
  
  flagConnectMD = MPI_Lookup_name("ServerMD", MPI_INFO_NULL, portMD);
  flagConnectMP = MPI_Lookup_name("ServerMP", MPI_INFO_NULL, portMP);
  
  method = STOP_ATEAM;
  int i = 0;
  int message = 1224;
   
  if (!flagConnectMD)
  { 
  	MPI_Comm_connect(portMD, MPI_INFO_NULL, 0, MPI_COMM_SELF, &commServerMD);
    MPI_Pack(&method, 1, MPI_INT, bufferMD, 100, &position, commServerMD);
    MPI_Send(bufferMD, position, MPI_PACKED, 0, 1, commServerMD);
    MPI_Send(&message, 1, MPI_INT, 0, 1, commServerMD);
    MPI_Comm_disconnect(&commServerMD);
  }
  position = 0;
  if (!flagConnectMP)
  { 
   	MPI_Comm_connect(portMP, MPI_INFO_NULL, 0, MPI_COMM_SELF, &commServerMP);
    MPI_Pack(&method, 1, MPI_INT, bufferMP, 100, &position, commServerMP);
    MPI_Send(bufferMP, position, MPI_PACKED, 0, 1, commServerMP);
    MPI_Send(&message, 1, MPI_INT, 0, 1, commServerMD);
    MPI_Comm_disconnect(&commServerMP);
  }
  
  if ((flagConnectMD) && (flagConnectMP)){
    printf("\n\n* Nenhum dos servidores de memoria do ATeam esta ativo *\n");fflush(stdout);
  }else
    printf("\n\n* O ATeam sera' finalizado em 5 minutos no maximo ... *\n");
  
  MPI_Finalize();
  
}

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */
