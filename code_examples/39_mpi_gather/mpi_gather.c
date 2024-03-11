// Page 240

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <mpi.h>
#include "util.h"

int main(int argc, char *argv[])
{

    if (argc != 4)
    {
        printf("usage: mpi_gather n m p \n");
        exit(1);
    }

    int n = atoi(argv[1]);
    int m = atoi(argv[2]);

    int rank, size; // Rank and size of the MPI world
    int root = 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double(*matrix)[n];
    matrix = (double(*)[n])malloc(m * n * size * sizeof(double));

    MPI_Comm comm = MPI_COMM_WORLD;

    MPI_Win win;
    MPI_Win_create(matrix, n * m * sizeof(double), sizeof(double), MPI_INFO_NULL, MPI_COMM_WORLD, &win);

    for (int i = 0; i < n * m; i++)
    {
        matrix[i / n][i % n] = rank;
    }

    printf("At rank %i: \n", rank);
    printMatrix(m, n, matrix);

    MPI_Datatype vec, cols;

    MPI_Type_vector(m, n, n * size, MPI_DOUBLE, &vec);
    MPI_Type_create_resized(vec, 0, n * sizeof(double), &cols);
    MPI_Type_commit(&cols);
    double(*fullmatrix)[size * n];
    if (rank == root)
    {
        fullmatrix = (double(*)[size * n]) malloc(m * n * size * sizeof(double));
    }
    MPI_Gather(matrix, m * n, MPI_DOUBLE, fullmatrix, 1, cols, root, comm);
    MPI_Type_free(&vec);
    MPI_Type_free(&cols);
    free(matrix);
    if (rank == root)
    {
        sleep(1);
        printf("---Finished---\n");
        printMatrix(m, n * size, fullmatrix);
        free(fullmatrix);
    }

    MPI_Win_free(&win);
    MPI_Finalize();
}