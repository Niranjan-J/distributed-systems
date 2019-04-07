#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"
#include <string.h>
#include "xbt/sysdep.h"
#include "xbt/log.h"
#include "xbt/xbt_os_time.h"

int main(int argc, char *argv[])
{
    xbt_os_timer_t tm = xbt_os_timer_new();
    xbt_os_cputimer_start (tm);
    int count = 1000;
    int *in, *out, *sol;
    int i, fnderr=0;
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    in = (int *)malloc( count * sizeof(int) );
    out = (int *)malloc( count * sizeof(int) );
    sol = (int *)malloc( count * sizeof(int) );
    for (i=0; i<count; i++)
    {
        *(in + i) = i;
        *(sol + i) = i*size;
        *(out + i) = 0;
    }
    MPI_Allreduce( in, out, count, MPI_INT, MPI_SUM, MPI_COMM_WORLD );
    for (i=0; i<count; i++)
    {
        if (*(out + i) != *(sol + i))
        {
            fnderr++;
        }
    }
    if (fnderr)
    {
        fprintf( stderr, "(%d) Error for type MPI_INT and op MPI_SUM\n", rank );
        fflush(stderr);
    }
    free( in );
    free( out );
    free( sol );
    MPI_Finalize();
    if (rank == 0)
    {
        int ans = (int)-1000000.0*xbt_os_timer_elapsed (tm);
        printf("%d\n", ans);
    }
    return fnderr;
}
