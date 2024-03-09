// Page 197

#define TAG0 500
#define TAG1 501

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <mpi.h>
#include "util.h"

int main(int argc, char *argv[])
{

    if (argc != 1)
    {
        printf("usage: mpi_buffer\n");
        exit(1);
    }

    int rank,
        size;
    MPI_Comm comm = MPI_COMM_WORLD;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);
    assert(size > 1);

    if (rank == 0)
    {
        int buf1[500];
        MPI_Send(buf1, 500, MPI_INT, size - 1, TAG0, comm);
    }
    else if (rank == 1)
    {
        double buf2[100];
        MPI_Send(buf2, 100, MPI_DOUBLE, size - 1, TAG1, comm);
    }
    else if (rank == size - 1)
    {
        MPI_Status status;
        int buf1[1000];
        double buf2[200];
        int cc;
        MPI_Recv(buf1, 1000, MPI_INT, MPI_ANY_SOURCE, TAG0, comm, &status);
        MPI_Get_elements(&status, MPI_INT, &cc);
        assert(cc < 1000);
        assert(cc == 500);
        printf("cc1 at rank %i: %i\n", rank, cc);

        MPI_Recv(buf2, 200, MPI_DOUBLE, MPI_ANY_SOURCE, TAG1, comm, &status);
        MPI_Get_elements(&status, MPI_DOUBLE, &cc);
        assert(cc < 1000);
        assert(cc == 100);
        printf("cc2 at rank %i: %i\n", rank, cc);
    }

    MPI_Finalize();
}