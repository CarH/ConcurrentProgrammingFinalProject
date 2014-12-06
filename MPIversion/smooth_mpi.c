#include "mpi.h"
#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <stddef.h>



main(int argc, char **argv )
{
  int message[20];
  int  i, rank, size, type = 99;
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  /// Lê a matriz de pixels:
  for (i = 0; i < 20; ++i)
  {
  	message[i] = i*2;
  }


  if (rank == 0)
  {
  	printf("Num processes: %d\n", size);
    // strcpy(message, "Hello, world.");
    for (i = 1; i < size; i++) {
       	MPI_Send(message, 16, MPI_INT, i, type, MPI_COMM_WORLD);
  	}
  } 
  else{
    
    MPI_Recv(message, 20, MPI_INT, 0, type, MPI_COMM_WORLD, &status);


  	printf( "Message from process = %d :, rank: %d\n", 0, rank);
  	for (i = 0; i < 20; ++i)
	  {
	  	printf("M[%d]: %d | ",i, message[i]);
	  }

  } 

  MPI_Finalize();
}