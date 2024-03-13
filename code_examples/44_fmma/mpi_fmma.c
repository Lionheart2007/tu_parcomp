// Page 251

// REMARK Double declaration of rowrank, colrank
// REMARK Why not usual matrix declaration?
// REMARK I have spent about 2h on this now and didn't get it to run.
// This is far from copy-paste. Maybe helping with the matrix allocations would help.

// REMARK I'm leaving away the quicksort copy step from 254, since it is just the copy step
// I think it would be a great idea to include the full code for both this algorithm and the one on page 254.
// (Maybe in the appendix?)

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

    if (argc != 4)
    {
        printf("usage: fmma m l n\n");
        exit(1);
    }

    int m = atoi(argv[1]);
    int l = atoi(argv[2]);
    int n = atoi(argv[3]);

    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    int(*localMatrixA)[l / size] = (int(*)[l / size]) malloc((m / size) * (l / size) * sizeof(int));
    int(*localMatrixB)[n / size] = (int(*)[n / size]) malloc((l / size) * (n / size) * sizeof(int));
    int(*localMatrixC)[n / size] = (int(*)[n / size]) malloc((m / size) * (n / size) * sizeof(int));

    for (int i = 0; i < m / size; i++)
    {
        for (int j = 0; j < l / size; j++)
        {
            localMatrixA[i][j] = rank;
        }
    }

    for (int i = 0; i < l / size; i++)
    {
        for (int j = 0; j < n / size; j++)
        {
            localMatrixB[i][j] = 1;
        }
    }

    printf("Rank %i, Matrix a: \n", rank);
    printMatrixInt(m / size, l / size, localMatrixA);
    printf("Rank %i, Matrix b: \n", rank);
    printMatrixInt(l / size, n / size, localMatrixB);

    int rc[2]; // row-column factorization
    int period[2];
    int coords[2]; // coordinates of process
    int reorder;
    MPI_Comm colcomm, cartcomm, rowcomm;
    rc[0] = 0;
    rc[1] = 0;
    MPI_Dims_create(size, 2, rc);
    assert(rc[0] == rc[1]); // number of processes must be square
    period[0] = 0;
    period[1] = 0;
    reorder = 0;
    MPI_Cart_create(comm, 2, rc, period, reorder, &cartcomm);
    MPI_Cart_coords(cartcomm, rank, 2, coords);
    MPI_Comm_split(cartcomm, coords[0], 0, &rowcomm);
    MPI_Comm_split(cartcomm, coords[1], 0, &colcomm);
    int rowrank, colrank;
    MPI_Comm_rank(rowcomm, &rowrank);
    MPI_Comm_rank(colcomm, &colrank);
    assert(rowrank == coords[1]);
    assert(colrank == coords[0]);

    int rowsize, colsize;
    MPI_Comm_rank(rowcomm, &rowrank);
    MPI_Comm_rank(colcomm, &colrank);
    MPI_Comm_size(rowcomm, &rowsize);
    MPI_Comm_size(colcomm, &colsize);
    assert(rowsize == colsize); // size is square

    int(*Atmp)[l / rowsize] = malloc((m / colsize) * (l / rowsize) * sizeof(int));
    int(*Btmp)[n / rowsize] = malloc((l / colsize) * (n / rowsize) * sizeof(int));

    int i;
    for (i = 0; i < rowsize; i++)
    {
        int(*AA)[l];
        int(*BB)[n];
        AA = (i == rowrank) ? localMatrixA : Atmp;
        MPI_Bcast(AA[0], m / rowsize * l / rowsize, MPI_INT, i, rowcomm);
        BB = (i == colrank) ? localMatrixB : Btmp;
        MPI_Bcast(BB[0], l / rowsize * n / rowsize, MPI_INT, i, colcomm);

        printf("AA: \n");
        printMatrixInt(m / rowsize, l / rowsize, AA);

        fmma(m / rowsize, l / rowsize, n / rowsize, localMatrixC, AA, BB);
    }

    printf("Rank: %i, C: \n", i);
    printMatrixInt((m / size), (n / size), localMatrixC);

    MPI_Finalize();
    return 0;
}
