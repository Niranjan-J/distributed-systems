#include <stdio.h>
#include "mpi.h"
// #include <simgrid/link.h>
#include "simgrid/msg.h"

int main(int argc, char **argv) {
  
  int rank, value;
  
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
  
  TRACE_link_variable_declare("link");
  TRACE_category_with_color("all","0.7 0.7 0.7");
  
  if (rank == 0) {    
      printf("Enter a number to broadcast:\n");
	    // scanf("%d", &value);
      value = 90;
  } else {
  	  printf("process %d: Before MPI_Bcast, value is %d\n", rank, value); 
	}
  
  MPI_Bcast(&value, 1, MPI_INT, 0, MPI_COMM_WORLD);
  
  printf("process %d: After MPI_Bcast, value is %d\n", rank, value);
  MPI_Finalize();

	return 0;
}