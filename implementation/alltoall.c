#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"
#include "xbt/sysdep.h"
#include "xbt/log.h"
#include "xbt/xbt_os_time.h"
#include <string.h>
#include <errno.h>

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#endif


int* genArr( int n)
{
  int* a = (int*)malloc(sizeof(int) * n);
  srand(time(0));
  for(int i=0;i<n;i++)  a[i] = rand()%100;
  return a;
}

int main( int argc, char *argv[] )
{
    xbt_os_timer_t tm = xbt_os_timer_new();
    xbt_os_cputimer_start (tm);
    int rank, size;
    int chunk = atoi(argv[1]);
    int i;
    int *sb;
    int *rb;
    int status, gstatus;

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    for ( i=1 ; i < argc ; ++i ) {
        if ( argv[i][0] != '-' )
            continue;
        switch(argv[i][1]) {
            case 'm':
                chunk = atoi(argv[++i]);
                break;
            default:
                fprintf(stderr, "Unrecognized argument %s\n", argv[i]);fflush(stderr);
                MPI_Abort(MPI_COMM_WORLD,EXIT_FAILURE);
        }
    }
    sb = genArr(chunk);
    rb = genArr(chunk);

    status = MPI_Alltoall(sb, chunk, MPI_INT, rb, chunk, MPI_INT, MPI_COMM_WORLD);
    MPI_Allreduce( &status, &gstatus, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD );
    /*if (rank == 0) {
        if (gstatus != 0) {
            printf("all_to_all returned %d\n",gstatus);
        }
        else printf("%d\n", gstatus);
    }*/
    MPI_Finalize();
    xbt_os_cputimer_stop (tm);
    if (rank == 0)
    {
        int ans = (int)1000000.0*xbt_os_timer_elapsed (tm);
        printf("%d\n", ans);
    }
    return(EXIT_SUCCESS);
}