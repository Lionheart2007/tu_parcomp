// Page 232
// REMARK Why suddenly MPI_FLOAT?
// REMARK Some info on what which variable does would be helpful

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <mpi.h>
#include "util.h"

int main(int argc, char *argv[])
{

    if (argc != 4)
    {
        printf("usage: mpi_binary_search local_size search_for p \n");
        exit(1);
    }

    int n = atoi(argv[1]);
    int x = atoi(argv[2]);
    int p = atoi(argv[3]);

    int rank, size; // Rank and size of the MPI world

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    float localArr[n];

    for (int i = 0; i < n; i++)
    {
        localArr[i] = (rank * size + i) * 2;
    }

    printf("Thread %i: ", rank);
    for (int i = 0; i < n; i++)
    {
        printf("%f ", localArr[i]);
    }
    printf("\n");

    MPI_Win win;
    MPI_Win_create(localArr, n * sizeof(float), sizeof(float), MPI_INFO_NULL, MPI_COMM_WORLD, &win);

    int l, u, m;
    int target, locali;
    float ma;
    l = -1;
    u = n * p; // total size of distributed array

    do
    {
        m = (l + u) / 2;
        target = m / n;
        locali = m % n;
        MPI_Win_lock(MPI_LOCK_SHARED, target, 0, win);
        MPI_Get(&ma, 1, MPI_FLOAT,
                target, locali, 1, MPI_FLOAT, win); // get middle element
        MPI_Win_unlock(target, win);
        if (x < ma)
            u = m;
        else
            l = m;
    } while (l + 1 < u);

    if (rank == 0)
    {
        sleep(1);
        printf("rank: %i, m: %i, l: %i, u: %i, ma: %f\n", rank, m, l, u, ma);
    }

    MPI_Win_free(&win);
    MPI_Finalize();
}