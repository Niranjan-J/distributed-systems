#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"
#include <string.h>
#include "xbt/sysdep.h"
#include "xbt/log.h"
#include "xbt/xbt_os_time.h"

int* genArr( int n)
{
  int* a = (int*)malloc(sizeof(int) * n);
  srand(time(0));
  for(int i=0;i<n;i++)  a[i] = rand()%100;
  return a;
}

int main(int argc, char **argv) {
    xbt_os_timer_t tm = xbt_os_timer_new();
    xbt_os_cputimer_start (tm);
    int size, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n = atoi(argv[1]);
    int *arr = genArr(n);

    int globaldata[10];/*wants to declare array this way*/
    int localdata;/*without using pointers*/

    int i;
    if (rank == 0) {

        for (i=0; i<size; i++)
            globaldata[i] = i;

        // printf("1. Processor %d has data: ", rank);
        // for (i=0; i<size; i++)
        //     printf("%d ", globaldata[i]);
        // printf("\n");
    }

    MPI_Scatter(globaldata, n, MPI_INT, &localdata, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // printf("2. Processor %d has data %d\n", rank, localdata);
    localdata= 5;
    // printf("3. Processor %d now has %d\n", rank, localdata);

    MPI_Gather(&localdata, n, MPI_INT, globaldata, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Finalize();
    xbt_os_cputimer_stop (tm);
    if (rank == 0) {
        // printf("4. Processor %d has data: ", rank);
        // for (i=0; i<size; i++)
        //     printf("%d ", globaldata[i]);
        // printf("\n");
        int ans = (int)1000000.0*xbt_os_timer_elapsed (tm);
        printf("%d\n", ans);
    }
    return 0;
}