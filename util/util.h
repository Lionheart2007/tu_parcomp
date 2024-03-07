#ifndef UTIL_H
#define UTIL_H

#define max(a, b) \
    ({ __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b; })

#define min(a, b) \
    ({ __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b; })

void printMatrix(int rows, int cols, double matrix[rows][cols]);
void fillRandomMatrix(int rows, int cols, double matrix[rows][cols]);
void fillMatrixRandom(int rows, int cols, double matrix[rows][cols]);
void zeroMatrix(int rows, int cols, double matrix[rows][cols]);
void printArray(int length, double arr[length]);
void printArrayInt(int length, int arr[length]);
void zeroArray(int length, double arr[length]);
void fillRandomArray(int length, double arr[length]);
void fillRandomArrayInt(int length, int arr[length]);
void fillRandomArrayIntMax(int length, int arr[length], int max);

#endif
