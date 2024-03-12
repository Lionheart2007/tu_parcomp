// Page 246

#define STENTAG 11

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <mpi.h>
#include "util.h"

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        printf("usage: mpi_safe_stencil n\n");
        exit(1);
    }

    int n = atoi(argv[1]);

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

    int left, right;
    int up, down;
    MPI_Cart_shift(cartcomm, 1, 1, &left, &right);
    MPI_Cart_shift(cartcomm, 0, 1, &up, &down);

    double *out_left = (double *)malloc(n * sizeof(double));
    double *out_right = (double *)malloc(n * sizeof(double));
    double *out_up = (double *)malloc(n * sizeof(double));
    double *out_down = (double *)malloc(n * sizeof(double));
    double *in_left = (double *)malloc(n * sizeof(double));
    double *in_right = (double *)malloc(n * sizeof(double));
    double *in_up = (double *)malloc(n * sizeof(double));
    double *in_down = (double *)malloc(n * sizeof(double));

    for (int i = 0; i < n; i++)
    {
        out_left[i] = (double)rank;
        out_right[i] = (double)rank;
        out_up[i] = (double)rank;
        out_down[i] = (double)rank;
    }

    int done = 0;
    MPI_Request request[8];
    while (!done)
    { // iterate until convergence
        MPI_Isend(out_left, n, MPI_DOUBLE, left, STENTAG, cartcomm, &request[0]);
        MPI_Isend(out_right, n, MPI_DOUBLE, right, STENTAG, cartcomm, &request[1]);
        MPI_Isend(out_up, n, MPI_DOUBLE, up, STENTAG, cartcomm, &request[2]);
        MPI_Isend(out_down, n, MPI_DOUBLE, down, STENTAG, cartcomm, &request[3]);
        MPI_Irecv(in_left, n, MPI_DOUBLE, left, STENTAG, cartcomm, &request[4]);
        MPI_Irecv(in_right, n, MPI_DOUBLE, right, STENTAG, cartcomm, &request[5]);
        MPI_Irecv(in_up, n, MPI_DOUBLE, up, STENTAG, cartcomm, &request[6]);
        MPI_Irecv(in_down, n, MPI_DOUBLE, down, STENTAG, cartcomm, &request[7]);
        MPI_Waitall(8, request, MPI_STATUSES_IGNORE);
        done = 1; // some termination criterion

        MPI_Allreduce(MPI_IN_PLACE, &done, 1, MPI_INT, MPI_LAND, cartcomm);
    }

    printf("Rank %i received from rank %i ", rank, left);
    printArray(n, in_left);
    printf("Rank %i received from rank %i ", rank, right);
    printArray(n, in_right);
    printf("Rank %i received from rank %i ", rank, up);
    printArray(n, in_up);
    printf("Rank %i received from rank %i ", rank, down);
    printArray(n, in_down);

    free(out_left);
    free(out_right);
    free(out_up);
    free(out_down);
    free(in_left);
    free(in_right);
    free(in_up);
    free(in_down);

    MPI_Comm_free(&cartcomm); // Free the Cartesian communicator
    MPI_Finalize();
}