#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int NUM_SPAWNS = 1;
    MPI_Status status;
    int errcodes[NUM_SPAWNS];
    
    
    MPI_Comm intercomm;
    MPI_Init (NULL, NULL);	/* starts MPI */
    
    char *argv[] = {"1", "/home/gisele/Documentos/PFC/Ateam/Testes-Threads/ateam/20_40_30", NULL};     
    
    MPI_Comm_spawn
    ( 
      "/home/gisele/Documentos/PFC/Ateam/Testes-Threads/ateam/src/ag_3opt",
	  argv, 
	  1,
	  MPI_INFO_NULL,
	  0,  
	  MPI_COMM_SELF, 
	  &intercomm, 
	  errcodes 
	);

/* ----------------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

	int sendMsg = 123;
	MPI_Request request;
	int position = 0;
	char * buffer;
	
	int size = (sizeof(int)*3 + sizeof(char));
	buffer = malloc(size);
	
	char change[1];
	change[0] = 'P';
	
	int param[3];
	param[0] = 1;
	param[1] = 6;
	param[2] = 11;
	
	
    MPI_Pack(change, 1, MPI_CHAR, buffer, size, &position, intercomm);
	MPI_Pack(&param[0], 1, MPI_INT, buffer, size, &position, intercomm);
	MPI_Pack(&param[1], 1, MPI_INT, buffer, size, &position, intercomm);
	MPI_Pack(&param[2], 1, MPI_INT, buffer, size, &position, intercomm);
	
	
	MPI_Send(buffer, position, MPI_PACKED, 0, 1, intercomm); 

    free(buffer);
	
    position = 0;
    size = sizeof(char);
	buffer = malloc(size);
	
	
	change[0] = 'S';
	MPI_Pack(change, 1, MPI_CHAR, buffer, size, &position, intercomm);
    MPI_Send(buffer, position, MPI_PACKED, 0, 1, intercomm); 
	free(buffer);
	
	printf("\n INTERFACE : Mensagem Enviada \n");
	
/* ----------------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */	
	
	int teste = 0;
	MPI_Recv(&teste, 1, MPI_INT, 0, MPI_ANY_TAG, intercomm, &status);
	

	if(teste == 1){
	  printf("\n%d", teste);
	  printf("\n ========= INTERFACE Agente 3opt FINALIZADO! \n");
	}
   

    MPI_Finalize();
    return 0;
}

