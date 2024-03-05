#include <stdio.h>
#include <stdlib.h> 

#include "util.h"


void printMatrix(int rows, int cols, double matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%f\t", matrix[i][j]);
        }
        printf("\n");
    }
}

void fillRandomMatrix(int rows, int cols, double(*matrix)[cols+2]) {
    // Seed the random number generator with current time
    srand(42);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // Generate random value between 0 and 20
            matrix[i][j] =(int) 1 % 20;
        }
    }
}


void zeroMatrix(int rows, int cols, double matrix[rows][cols]) {
    // Seed the random number generator with current time
    srand(42);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // Generate random value between 0 and 20
            matrix[i][j] = 0;
        }
    }
}