#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#include "world1.h"

int teste_mpich(void)
{
    int rank, size;
    char message[20];
    MPI_Status status;
    int errcodes[NUM_SPAWNS];
    int *argc1 = (int *) malloc(sizeof(int));
    //int 

    
    MPI_Comm parentcomm, intercomm, child, newintracomm, intercomm2;
    MPI_Init (NULL, NULL);	/* starts MPI */
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);	/* get current process id */
    MPI_Comm_size (MPI_COMM_WORLD, &size);	/* get number of processes */
  
    if (rank == ROOT_RANK) {
    	printf( "Hello world from process %d of %d\n", rank, size );
    	printf("RAIZ \n");
	
	
	MPI_Comm_spawn( "/home/gisele/Documentos/PFC/Testes-MPI-PFC/servidor2",
	MPI_ARGV_NULL, 2,
	MPI_INFO_NULL,
	0,  MPI_COMM_SELF, &intercomm, errcodes );
	
	MPI_Comm_spawn( "/home/gisele/Documentos/PFC/Testes-MPI-PFC/servidor3",
	MPI_ARGV_NULL, 2,
	MPI_INFO_NULL, 0,  MPI_COMM_SELF, &intercomm2, errcodes );
	
	
	 MPI_Recv(message, 20, MPI_CHAR, 0, MPI_ANY_TAG,intercomm, &status);
	printf(message);
	printf("\nmeu filho mandou\n");
	
	MPI_Recv(message, 20, MPI_CHAR, 0, MPI_ANY_TAG,intercomm2, &status);
	printf(message);
	printf("\nmeu filho mandou 3\n");
	
	
	
	
    }  
  
    MPI_Finalize();
    return 0;
}
