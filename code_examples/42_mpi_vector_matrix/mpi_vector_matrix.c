// Page 243

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;
    int m = 4; // Number of rows in the matrix
    int n = 4; // Number of columns in the matrix (size of the vector)

    MPI_Init(&argc, &argv);
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    // Check if matrix dimensions are divisible by the number of processes
    if (m % size != 0 || n % size != 0)
    {
        if (rank == 0)
        {
            printf("Error: Matrix dimensions are not divisible by the number of processes.\n");
        }
        MPI_Finalize();
        return 1;
    }

    // Allocate memory for local vector
    double *vector = (double *)malloc(n / size * sizeof(double));

    // Allocate memory for the full vector
    double *fullvector = (double *)malloc(n * sizeof(double));

    // Generate random values for the local vector
    for (int i = 0; i < n / size; i++)
    {
        vector[i] = rank + 1; // Simulating some values for demonstration
    }

    // Gather all vectors from all processes into fullvector
    MPI_Allgather(vector, n / size, MPI_DOUBLE, fullvector, n / size, MPI_DOUBLE, comm);

    // Free the memory for the local vector
    free(vector);

    // Allocate memory for the result
    double *result = (double *)malloc(m / size * sizeof(double));

    // Simulated matrix
    double matrix[4][4] = {{1, 2, 3, 4},
                           {5, 6, 7, 8},
                           {9, 10, 11, 12},
                           {13, 14, 15, 16}};

    // Perform matrix-vector multiplication
    for (int i = 0; i < m / size; i++)
    {
        result[i] = matrix[i + rank * (m / size)][0] * fullvector[0];
        for (int j = 1; j < n; j++)
        {
            result[i] += matrix[i + rank * (m / size)][j] * fullvector[j];
        }
    }

    // Gather all results to process 0
    double *gatheredResults = NULL;
    if (rank == 0)
    {
        gatheredResults = (double *)malloc(m * sizeof(double));
    }
    MPI_Gather(result, m / size, MPI_DOUBLE, gatheredResults, m / size, MPI_DOUBLE, 0, comm);

    // Printing the gathered results
    if (rank == 0)
    {
        printf("Result of Matrix-Vector Multiplication:\n");
        for (int i = 0; i < m; i++)
        {
            printf("%.2f ", gatheredResults[i]);
        }
        printf("\n");
        free(gatheredResults);
    }

    // Free memory
    free(fullvector);
    free(result);

    MPI_Finalize();

    return 0;
}
