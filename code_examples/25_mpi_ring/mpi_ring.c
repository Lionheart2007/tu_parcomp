// Page 193

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

    printf("rank: %i ", rank);

    assert(size > 1);
    int *buffer = (int *)malloc(sizeof(int) * length);

    for (int i = 0; i < length; i++)
    {
        buffer[i] = rank;
    }

    printArrayInt(length, buffer);

    if (rank == root)
    {
        MPI_Send(buffer, count, MPI_INT, (rank + 1) % size, TAG, comm);
    }
    else if (rank == (root - 1 + size) % size)
    {
        MPI_Status status;
        MPI_Recv(buffer, count, MPI_INT, (rank - 1 + size) % size, TAG,
                 comm, &status);
    }
    else
    {
        MPI_Status status;
        MPI_Recv(buffer, count, MPI_INT, (rank - 1 + size) % size, TAG,
                 comm, &status);
        MPI_Send(buffer, count, MPI_INT, (rank + 1) % size, TAG, comm);
    }

    printf("rank: %i ", rank);

    printArrayInt(length, buffer);
    MPI_Finalize();
}