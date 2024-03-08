// Page 194

#define TAG 1000
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <mpi.h>
#include "util.h"

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        printf("usage: mpi_ring length\n");
        exit(1);
    }

    int length = atoi(argv[1]);
    int root = 0;
    int rank,
        size;
    MPI_Comm comm = MPI_COMM_WORLD;

    int count = length;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    printf("rank in: %i ", rank);

    assert(size > 1);
    int *buffer = (int *)malloc(sizeof(int) * length);

    for (int i = 0; i < length; i++)
    {
        buffer[i] = rank;
    }

    printArrayInt(length, buffer);

    if (rank == root)
    {
        int i;
        for (i = 0; i < size; i++)
        {
            if (i == root)
                continue;
            MPI_Send(buffer, count, MPI_INT, i, TAG, comm);
        }
    }
    else
    {
        MPI_Recv(buffer, count, MPI_INT, root, TAG, comm, MPI_STATUS_IGNORE);
    }

    printf("rank out: %i ", rank);

    printArrayInt(length, buffer);

    MPI_Finalize();
}