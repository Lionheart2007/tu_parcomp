// Page 249

// In the loop: Why do you assign j=0 explicitly?

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;
    int m = 6; // Number of rows in the matrix
    int n = 6; // Number of columns in the matrix (assuming it matches the size of the vector)
    double matrix[6][2] = {{1, 2},
                           {3, 4},
                           {5, 6},
                           {7, 8},
                           {9, 10},
                           {11, 12}};
    double vector[6] = {1, 1};
    double *partial;

    MPI_Init(&argc, &argv);
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    if (m % size != 0 || n % size != 0)
    {
        if (rank == 0)
            printf("m and n must be divisible by the number of processes.\n");
        MPI_Finalize();
        return 1;
    }

    partial = (double *)malloc(m * sizeof(double));
    for (int i = 0; i < m; i++)
    {
        partial[i] = matrix[i][0] * vector[0];
        for (int j = 1; j < n / size; j++)
        {
            partial[i] += matrix[i][j] * vector[j];
        }
    }

    double result[m / size];
    MPI_Reduce_scatter_block(partial, result, m / size, MPI_DOUBLE, MPI_SUM, comm);

    printf("Rank: ");
    for (int i = 0; i < m / size; i++)
    {
        printf("%.2f ", result[i]);
    }
    printf("\n");

    free(partial);
    MPI_Finalize();

    return 0;
}
