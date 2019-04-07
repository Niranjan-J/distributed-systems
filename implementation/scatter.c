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

    int globaldata[10];
    int localdata;

    int i;
    if (rank == 0) {

        for (i=0; i<size; i++)
            globaldata[i] = rand()%100;
    }

    MPI_Scatter(globaldata, n, MPI_INT, &localdata, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Finalize();
    xbt_os_cputimer_stop (tm);
    if (rank == 0)
    {
        int ans = (int)1000000.0*xbt_os_timer_elapsed (tm);
        printf("%d\n", ans);
    }
    return 0;
}