#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"
#include <string.h>
#include "xbt/sysdep.h"
#include "xbt/log.h"
#include "xbt/xbt_os_time.h"

int main(int argc, char *argv[]) {
    xbt_os_timer_t tm = xbt_os_timer_new();
    xbt_os_cputimer_start (tm);
    int rank, size, i, n;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int sendbuf[size];
    int recvbuf;

    for (int i=0; i<size; i++)
        sendbuf[i] = 1 + rank + size*i;

    int recvcounts[size];
    for (int i=0; i<size; i++)
        recvcounts[i] = 1;

    MPI_Reduce_scatter(sendbuf, &recvbuf, recvcounts, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
    MPI_Finalize();

    xbt_os_cputimer_stop (tm);
    if (rank == 0)
    {
        int ans = (int)1000000.0*xbt_os_timer_elapsed (tm);
        printf("%d\n", ans);
    }
    return 0;
}