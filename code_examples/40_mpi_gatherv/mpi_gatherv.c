// Page 243

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <mpi.h>
#include "util.h"

int main(int argc, char *argv[])
{

    if (argc != 1)
    {
        printf("usage: mpi_gatherv \n");
        exit(1);
    }

    int rank, size; // Rank and size of the MPI world
    int root = 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int count = rank;
    int sendbuf[count];

    for (int i = 0; i < rank; i++)
    {
        sendbuf[i] = rank;
    }

    MPI_Comm comm = MPI_COMM_WORLD;

    MPI_Win winCount;
    MPI_Win_create(&count, sizeof(int), sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &winCount);
    MPI_Win winGather;
    MPI_Win_create(sendbuf, rank * sizeof(int), sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &winGather);

    int recvcounts[size];

    int recvbuf[(size * (size - 1)) / 2];

    // gather counts from all processes
    MPI_Gather(&count, 1, MPI_INT, recvcounts, 1, MPI_INT, root, comm);

    if (rank == root)
    {
        printArrayInt(size, recvcounts);
    }

    int recvdispls[size];
    if (rank == root)
    {
        // compute displacements, on root only
        recvdispls[0] = 0;
        for (int i = 1; i < size; i++)
        { // data received consecutively, prefix-sums
            recvdispls[i] = recvdispls[i - 1] + recvcounts[i - 1];
        }
    }
    // gather the possibly different amounts of data from all processes
    MPI_Gatherv(sendbuf, count, MPI_INT, recvbuf,
                recvcounts, recvdispls, MPI_INT, root, comm);

    if (rank == root)
    {
        printArrayInt((size * (size - 1)) / 2, recvbuf);
    }

    MPI_Win_free(&winCount);
    MPI_Win_free(&winGather);

    MPI_Finalize();
}