#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"
#include <string.h>

// #include <simgrid/link.h>
// #include "simgrid/msg.h"

int* genArr( int n) {
  int* a = (int*)malloc(sizeof(int) * n);
  srand(time(0));
  for(int i=0;i<n;i++) {
    a[i] = rand()%100;
  }
  return a;
}

int main(int argc, char **argv) {
  int tm = clock();
  int rank;
  
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
  
  int n = atoi(argv[1]);

  int *arr = genArr(n);
  if (rank == 0) {    
      // printf("Enter a number to broadcast:\n");
	    // scanf("%d", &value);
  } else {
  	  // printf("process %d: Before MPI_Bcast, value is %d\n", rank, value); 
	}
  
  MPI_Bcast(arr, n, MPI_INT, 0, MPI_COMM_WORLD);
  
  // printf("process %d: After MPI_Bcast, value is %d\n", rank, value);
  MPI_Finalize();
  tm = clock() - tm;
  
  if (rank == 0)
    printf("%d\n",tm);
	
  return 0;
}