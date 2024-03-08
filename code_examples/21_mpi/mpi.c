#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;
    MPI_Comm comm, mmoc, evodcomm, workcomm;
    int result;
    MPI_Init(&argc, &argv);
    comm = MPI_COMM_WORLD;
    MPI_Comm_compare(comm, MPI_COMM_WORLD, &result);
    assert(result == MPI_IDENT);
    MPI_Comm_dup(MPI_COMM_WORLD, &comm);
    MPI_Comm_compare(MPI_COMM_WORLD, comm, &result);
    assert(result == MPI_CONGRUENT);
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);
    MPI_Comm_split(comm, 0, size - rank, &mmoc);
    MPI_Comm_compare(comm, mmoc, &result);

    assert(size == 1 || result == MPI_SIMILAR);
    MPI_Comm_split(comm, rank % 2, 0, &evodcomm);
    MPI_Comm_compare(comm, evodcomm, &result);
    assert(size == 1 || result == MPI_UNEQUAL);
    MPI_Comm_free(&mmoc);
    MPI_Comm_free(&evodcomm);
    MPI_Comm_split(comm, (rank == size - 1 ? MPI_UNDEFINED : 1), 0,
                   &workcomm);
    if (workcomm != MPI_COMM_NULL)
    {
        MPI_Comm_compare(comm, workcomm, &result);
        assert(result == MPI_UNEQUAL);
        MPI_Comm_free(&workcomm);
    }
    MPI_Comm_free(&comm);
    MPI_Finalize();
    return 0;
}