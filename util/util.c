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

void printArray(int length, double arr[length]) {
    printf("[");
    for(int i = 0; i<length; i++) {
        printf("%f", arr[i]);
        
        if (i != length-1) printf(",");
    }
    printf("]\n");
}

void zeroArray(int length, double arr[length]) {
    for(int i = 0; i<length; i++) {
        arr[i] = 0;
    }
}

void fillRandomArray(int length, double arr[length]) {
    for(int i = 0; i<length; i++) {
        arr[i] = (int) rand() % 20;
    }
}