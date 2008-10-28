/* ------------------------------------------------------------------------- */
/* Este arquivo contem o codigo necessario para o agente StopServer. Este a- */
/* gente é chamdo apenas quando o agente stopServer já tiver sido chamado .  */
/* Ele apenas conecta com os servidores de memória para que estes saiam do   */
/* laço de execução.                                                         */
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
      
  int flagConnectMD = 1,
      flagConnectMP = 1;
      
       
  char  portMD[MPI_MAX_PORT_NAME],
        portMP[MPI_MAX_PORT_NAME];
       
  MPI_Comm commServerMD;
  MPI_Comm commServerMP;
  
  flagConnectMD = MPI_Lookup_name("ServerMD", MPI_INFO_NULL, portMD);
  flagConnectMP = MPI_Lookup_name("ServerMP", MPI_INFO_NULL, portMP);
  
  if (!flagConnectMD)
  { 
  	MPI_Comm_connect(portMD, MPI_INFO_NULL, 0, MPI_COMM_SELF, &commServerMD);
    MPI_Comm_disconnect(&commServerMD);
  }
 
  if (!flagConnectMP)
  { 
   	MPI_Comm_connect(portMP, MPI_INFO_NULL, 0, MPI_COMM_SELF, &commServerMP);
    MPI_Comm_disconnect(&commServerMP);
  }
  
  if ((flagConnectMD) && (flagConnectMP)){
    printf("\n\n* Nenhum dos servidores de memoria do ATeam esta ativo *\n");fflush(stdout);
  }else
    printf("\n\n* Servidores de soluções FINALIZADOS ... *\n");
  
  MPI_Finalize();
  
}

/* ------------------------------------------------------------------------- */
/* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
/* ------------------------------------------------------------------------- */


