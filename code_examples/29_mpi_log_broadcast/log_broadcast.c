// Page 203

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
    int dist, virt;

    virt = (rank - root + size) % size;
    // buffer of some type from somewhere
    if (virt != 0)
    {
        int d = 1;
        while (virt >= d)
        {
            dist = d;
            d <<= 1; // multiply by two
        }
        MPI_Recv(buffer, count, MPI_INT, (virt - dist + root + size) % size, TAG, comm,
                 MPI_STATUS_IGNORE);
        dist = d;
    }
    else
        dist = 1;
    while (virt + dist < size)
    {
        MPI_Send(buffer, count, MPI_INT, (virt + dist + root) % size, TAG, comm);
        dist <<= 1; // multiply by two
    }

    printf("rank out: %i ", rank);

    printArrayInt(length, buffer);

    MPI_Finalize();
}