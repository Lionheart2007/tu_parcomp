// Page 221
// REMARK why unusual way of declaring matrix?
// REMARK why do we need all that space in the newmatrix, if we are only sending one column: We only need m*n space in newmatrix also.
// In the script, newmatrix is also said to be n x (m*p). Why would it?

#define MATTAG 2007

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <mpi.h>
#include "util.h"

int main(int argc, char *argv[])
{

    if (argc != 3)
    {
        printf("usage: mpi_matrix_npi n m\n");
        exit(1);
    }

    MPI_Comm comm = MPI_COMM_WORLD;

    int n = atoi(argv[1]);
    int m = atoi(argv[2]);

    int rank, size; // Rank and size of the MPI world

    int i;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Allocate memory for local matrix
    double *matrix;
    matrix = (double *)malloc(m * n * sizeof(double));

    // Fill local matrix with the rank
    for (int i = 0; i < m * n; i++)
    {
        matrix[i] = rank;
    }

    printf("Rank %i sending:\n", rank);
    // Print local matrix (optional for verification)
    for (int i = 0; i < m * n; i++)
    {
        printf("%f ", matrix[i]);
    }
    printf("\n");

    MPI_Datatype cols;
    MPI_Type_vector(m, n, n * size, MPI_DOUBLE, &cols);
    MPI_Type_commit(&cols);
    MPI_Request request;
    MPI_Isend(matrix, 1, cols, 0, MATTAG, comm, &request);

    if (rank == 0)
    {
        double *newmatrix;
        newmatrix = (double *)malloc(m * size * n * sizeof(double));
        for (i = 0; i < size; i++)
        {
            MPI_Recv(newmatrix + i * n, 1, cols, i, MATTAG, comm, MPI_STATUS_IGNORE);
        }

        printf("New matrix:\n");
        for (int i = 0; i < m * size * n; i++)
        {
            printf("%f ", newmatrix[i]);
        }
        printf("\n");
        free(newmatrix);
    }

    // Complete communication
    MPI_Wait(&request, MPI_STATUS_IGNORE);
    MPI_Type_free(&cols);

    free(matrix);

    MPI_Finalize();
}
