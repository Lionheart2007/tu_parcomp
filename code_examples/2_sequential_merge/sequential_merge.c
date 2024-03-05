#include <stdlib.h>
#include <stdio.h>
#include "util.h"

void seq_merge(double A[], int n, double B[], int m, double C[]) {
    int i, j, k;
    i = 0; j = 0; k = 0;
    while (i<n&&j<m) {
        C[k++] = (A[i] <= B[j]) ? A[i++] : B[j++];
    }
    while (i<n) C[k++] = A[i++];
    while (j<m) C[k++] = B[j++];
}

int main(int argc, char * argv[]) {
    if (argc != 2) {
        printf("Usage: sequential_merge n\n");
        exit(1);
    }

    int n = atoi(argv[1]);

    double a[n];
    double b[n];
    double c[n*2];

    for(int i = 0; i<n; i++) a[i] = i*2;
    for(int i = 0; i<n; i++) b[i] = i*2+1;

    printArray(n, a);
    printArray(n, b);

    seq_merge(a, n, b, n, c);

    printArray(n*2, c);
}

