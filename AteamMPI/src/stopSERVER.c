/* ------------------------------------------------------------------------- */
/* Este arquivo contem o codigo necessario para o agente StopServer. Este a- */
/* gente é chamdo apenas quando não existe mais nenhum agente em execução.   */
/* Ele finaliza os servidores de memória.                                    */
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
  
  method = STOP_SERVER;
  
  if (!flagConnectMD)
  { 
  	MPI_Comm_connect(portMD, MPI_INFO_NULL, 0, MPI_COMM_SELF, &commServerMD);
    MPI_Pack(&method, 1, MPI_INT, bufferMD, 100, &position, commServerMD);
    MPI_Send(bufferMD, position, MPI_PACKED, 0, 1, commServerMD);
    MPI_Comm_disconnect(&commServerMD);
  }
  position = 0;
  if (!flagConnectMP)
  { 
   	MPI_Comm_connect(portMP, MPI_INFO_NULL, 0, MPI_COMM_SELF, &commServerMP);
    MPI_Pack(&method, 1, MPI_INT, bufferMP, 100, &position, commServerMP);
    MPI_Send(bufferMP, position, MPI_PACKED, 0, 1, commServerMP);
    MPI_Comm_disconnect(&commServerMP);
  }
  
  if ((flagConnectMD) && (flagConnectMP)){
    printf("\n\n* Nenhum dos servidores de memoria do ATeam esta ativo *\n");fflush(stdout);
  }else
    printf("\n\n* Servidores de soluções serão finalizados ... *\n");
  
  MPI_Finalize();
  
}

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */

