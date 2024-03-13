// Page 267

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "util.h"
#include <assert.h>

void fmma(int m, int l, int n, int (*C)[n], int (*A)[l], int (*B)[n])
{
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            C[i][j] = 0;
            for (int k = 0; k < l; k++)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main(int argc, char *argv[])
{

    if (argc != 1)
    {
        printf("usage: mpi_regular_semantics\n");
        exit(1);
    }

    int rank, size;

    MPI_Comm comm = MPI_COMM_WORLD;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);
    int n = 2;
    int buffer[n + 1];
    int root = size - 1;
    if (rank == root)
    {
        buffer[0] = size;
        buffer[1] = 0;
        buffer[2] = -rank - 1;
    }
    else
    {
        buffer[0] = -rank - 1;
        buffer[1] = -rank - 1;
        buffer[2] = -rank - 1;
    }
    MPI_Bcast(buffer, n, MPI_INT, root, comm);
    assert(buffer[0] == size);
    assert(buffer[1] == 0);
    assert(buffer[2] == -rank - 1);
    if (rank == 0)
    {
        printf("Rank %d: buffer=[%d,%d,%d]\n",
               rank, buffer[0], buffer[1], buffer[2]);
    }

    MPI_Finalize();
    return 0;
}
