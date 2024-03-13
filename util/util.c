#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "util.h"

void printMatrix(int rows, int cols, double matrix[rows][cols])
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            // printf("%i %i %f\n", i, j, matrix[i][j]);
            printf("%f\t", matrix[i][j]);
        }
        printf("\n");
    }
}

void printMatrixInt(int rows, int cols, int matrix[rows][cols])
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            // printf("%i %i %f\n", i, j, matrix[i][j]);
            printf("%i\t", matrix[i][j]);
        }
        printf("\n");
    }
}

void fillMatrixRandom(int rows, int cols, double matrix[rows][cols])
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            // Generate random value between 0 and 20
            matrix[i][j] = (int)(rand() % 20) + 1;
        }
    }
}

void fillRandomMatrix(int rows, int cols, double (*matrix)[cols + 2])
{
    // Seed the random number generator with current time
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            // Generate random value between 0 and 20
            matrix[i][j] = (int)(rand() % 20) + 1;
        }
    }
}

void zeroMatrix(int rows, int cols, double matrix[rows][cols])
{
    // Seed the random number generator with current time

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            // Generate random value between 0 and 20
            matrix[i][j] = 0;
        }
    }
}

void printArray(int length, double arr[length])
{
    printf("[");
    for (int i = 0; i < length; i++)
    {
        printf("%f", arr[i]);

        if (i != length - 1)
            printf(",");
    }
    printf("]\n");
}

void printArrayInt(int length, int arr[length])
{
    printf("[");
    for (int i = 0; i < length; i++)
    {
        printf("%i", arr[i]);

        if (i != length - 1)
            printf(",");
    }
    printf("]\n");
}

void zeroArray(int length, double arr[length])
{
    for (int i = 0; i < length; i++)
    {
        arr[i] = 0;
    }
}

void fillRandomArray(int length, double arr[length])
{
    for (int i = 0; i < length; i++)
    {
        arr[i] = (int)rand() % 20;
    }
}

void fillRandomArrayInt(int length, int arr[length])
{
    fillRandomArrayIntMax(length, arr, 20);
}

void fillRandomArrayIntMax(int length, int arr[length], int max)
{
    for (int i = 0; i < length; i++)
    {
        arr[i] = (int)rand() % max;
    }
}