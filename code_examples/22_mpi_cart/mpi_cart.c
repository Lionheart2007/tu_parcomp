// Page 186

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <mpi.h>

int main(int argc, char *argv[])
{

    if (argc != 3)
    {
        printf("usage: mpi_cart threads dimension\n");
        exit(1);
    }

    MPI_Comm comm;
    MPI_Init(&argc, &argv);
    comm = MPI_COMM_WORLD;

    int p = atoi(argv[1]);
    int d = atoi(argv[2]);

    MPI_Comm_size(comm, &p);
    for (d = 1; d <= p; d++)
    {
        int dims[d], periods[d];
        int coords[d];
        MPI_Comm cartcomm;
        int rank;
        int r, dd, i;
        for (i = 0; i < d; i++)
            dims[i] = 0;
        MPI_Dims_create(p, d, dims);
        for (i = 0; i < d; i++)
            periods[i] = 0;
        MPI_Cart_create(comm, d, dims, periods, 0, &cartcomm);
        assert(cartcomm != MPI_COMM_NULL);
        MPI_Comm_rank(cartcomm, &rank);
        MPI_Cart_coords(cartcomm, rank, d, coords);
        r = 0;
        dd = 1;
        for (i = d - 1; i >= 0; i--)
        {
            r += coords[i] * dd;
            dd *= dims[i];
        }
        assert(r == rank);
        MPI_Comm_free(&cartcomm);
    }

    MPI_Finalize();
    return 0;
}