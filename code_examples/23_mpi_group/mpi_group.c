// Page 183

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    if (argc != 1)
    {
        printf("usage: mpi_group\n");
        exit(1);
    }

    MPI_Init(&argc, &argv);

    int rank,
        size;
    MPI_Comm comm = MPI_COMM_WORLD;

    MPI_Group group, workers;
    MPI_Comm work;
    int master = 0; // some arbitrary master (rank) in comm
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);
    MPI_Comm_group(comm, &group);
    // now exclude the master
    MPI_Group_excl(group, 1, &master, &workers);
    MPI_Comm_create(comm, workers, &work);
    if (rank == master)
        assert(work == MPI_COMM_NULL);
    else
    {
        int r;
        MPI_Comm_rank(work, &r);
        if (rank < master)
            assert(r == rank);
        else
            assert(r == rank - 1);
    }
    MPI_Group_free(&group);
    return 0;
}