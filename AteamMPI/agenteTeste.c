#include "mpi.h"
#include "globais.h"
#include <stdio.h>
#include <stdlib.h>
#define MSG_TAG 42
#define NUM_SPAWNS 2
#define ROOT_RANK 0
int main( int argc, char *argv[] )
{

	
       int myrank, numprocs, tamanho_nome_maquina;
       
   MPI_Status status;
    	
	int merr;
   	char port[MPI_MAX_PORT_NAME];
	
    MPI_Comm comm2;
	
    
    
    
    
    MPI_Init( &argc, &argv );
    
     char buffer[100];
     int position = 0;
  
   
    if (myrank == ROOT_RANK) {
    	printf("--------------------------------------------------------------\n", myrank);
    	printf("Servidor 3 - Processo %d \n", myrank);
	
	
	    merr = MPI_Lookup_name( "ServerMD", MPI_INFO_NULL, port );
        if (merr) {
           
            printf( "Error " );fflush(stdout);
        }
        else {
            
           int method = REQUEST_CUTS;
            
            printf("\n Espera de conexao \n ");
            MPI_Comm_connect(port, MPI_INFO_NULL, 0, MPI_COMM_SELF, &comm2);
	        printf("conectado\n");
	        
	        MPI_Pack(&method, 1, MPI_INT, buffer, 100, &position, comm2);
	  
		    MPI_Send(buffer, position, MPI_PACKED, 0, 1, comm2);
	        
	        int msg = 1224;
	        
	        printf("\n \n agenteTeste - Messagen 1224 %d: \n", msg);
   
   			MPI_Send(&msg, 1, MPI_INT, 0, 1, comm2);
 
   			MPI_Comm_disconnect(&comm2);
        }
        
        
    }  
    MPI_Finalize();
    return 0;
}
