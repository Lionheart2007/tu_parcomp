// Page 231
// REMARK You need to adapt the displacements here, right? Please include the updated
// displacements in the code example.

#define STENTAG 11

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
        printf("usage: mpi_fences_synchronized n m\n");
        exit(1);
    }

    int n = atoi(argv[1]);
    int m = atoi(argv[2]);

    int dims[2];    // Dimensions of the Cartesian grid
    int periods[2]; // Periodicity in each dimension (0: non-periodic, 1: periodic)
    int coords[2];  // Coordinates of the current process in the Cartesian grid
    int rank, size; // Rank and size of the MPI world

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Define the dimensions of the Cartesian grid
    dims[0] = 2 /* Number of processes in the first dimension */;
    dims[1] = 2 /* Number of processes in the second dimension */;

    // Define the periodicity of the Cartesian grid
    periods[0] = 1 /* Whether the grid is periodic in the first dimension (0 or 1) */;
    periods[1] = 1 /* Whether the grid is periodic in the second dimension (0 or 1) */;

    // Create the Cartesian communicator
    MPI_Comm cartcomm;
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &cartcomm);

    // Get the coordinates of the current process in the Cartesian grid
    MPI_Cart_coords(cartcomm, rank, 2, coords);

    printf("Rank %i has coordinates: ", rank);
    printArrayInt(2, coords);

    double(*matrix)[n + 2];
    double(*freeMatrix)[n + 2];
    matrix = (double(*)[n + 2]) malloc((m + 2) * (n + 2) * sizeof(double));
    freeMatrix = matrix;
    matrix = (double(*)[n + 2])((char *)matrix + (n + 2 + 1) * sizeof(double));

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            matrix[i][j] = rank;
        }
    }

    printMatrix(m + 2, n + 2, freeMatrix);

    int left, right;
    int up, down;
    MPI_Cart_shift(cartcomm, 1, 1, &left, &right);
    MPI_Cart_shift(cartcomm, 0, 1, &up, &down);

    MPI_Datatype column;
    MPI_Type_vector(m, 1, n + 2, MPI_DOUBLE, &column);
    MPI_Type_commit(&column);
    double *out_left, *out_right, *out_up, *out_down;

    out_left = &matrix[0][0];
    out_right = &matrix[0][n - 1];
    out_up = &matrix[0][0];
    out_down = &matrix[m - 1][0];

    printf("Rank %i sending to rank %i ", rank, left);
    printArray(n, out_left);
    printf("Rank %i sending to rank %i ", rank, right);
    printArray(n, out_right);
    printf("Rank %i sending to rank %i ", rank, up);
    printArray(n, out_up);
    printf("Rank %i sending to rank %i ", rank, down);
    printArray(n, out_down);

    MPI_Win win;
    MPI_Win_create((double *)matrix - (n + 2 + 1), (m + 2) * (n + 2) * sizeof(double), sizeof(double),
                   MPI_INFO_NULL, cartcomm, &win);
    int disp_left, disp_right, disp_up, disp_down;
    disp_left = (n + 2);
    disp_right = 2 * (n + 2) - 1;
    disp_up = 1;
    disp_down = (m + 1) * (n + 2) + 1;
    int neighbors[4];
    MPI_Group group;
    MPI_Group accessexposure;
    MPI_Comm_group(cartcomm, &group);
    int k = 0;
    if (left != MPI_PROC_NULL)
        neighbors[k++] = left;
    if (right != MPI_PROC_NULL)
        neighbors[k++] = right;
    if (up != MPI_PROC_NULL)
        neighbors[k++] = up;
    if (down != MPI_PROC_NULL)
        neighbors[k++] = down;
    MPI_Group_incl(group, k, neighbors, &accessexposure);

    int done = 0;
    while (!done)
    { // iterate until convergence
        MPI_Win_post(accessexposure, 0, win);
        MPI_Win_start(accessexposure, 0, win);
        MPI_Put(out_left, 1, column, left,
                disp_left, 1, column, win);
        MPI_Put(out_right, 1, column, right,
                disp_right, 1, column, win);
        MPI_Put(out_up, n, MPI_DOUBLE, up,
                disp_up, n, MPI_DOUBLE, win);
        MPI_Put(out_down, n, MPI_DOUBLE, down,
                disp_down, n, MPI_DOUBLE, win);
        MPI_Win_complete(win);
        MPI_Win_wait(win);
        done = 1; // some termination criterion
    }
    MPI_Win_free(&win);
    MPI_Type_free(&column);

    // I know that printing the columns like this is stupid; had to painfully find out :(
    printf("Rank %i received from rank %i ", rank, left);
    printArray(n, out_left);
    printf("Rank %i received from rank %i ", rank, right);
    printArray(n, out_right);
    printf("Rank %i received from rank %i ", rank, up);
    printArray(n, out_up);
    printf("Rank %i received from rank %i ", rank, down);
    printArray(n, out_down);

    printMatrix(m + 2, n + 2, freeMatrix);

    MPI_Comm_free(&cartcomm); // Free the Cartesian communicator
    free(freeMatrix);
    MPI_Finalize();
}