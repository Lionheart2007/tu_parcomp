//Page 67

#include <stdlib.h>
#include <stdio.h>
#include "util.h"

void Scan(int A[], int n)
{
    int k, kk;
    int i;
    // up-phase
    printf("---UP---\n");
    for (k=1; k<n; k=kk) {
        kk = k<<1; // double the loop increment
        for (i=kk-1; i<n; i+=kk) A[i] = A[i-k]+A[i];
        printf("k: %i, kk: %i ", k, kk);
        printArrayInt(n, A);
    }

    printf("---DOWN---\n");
    // down-phase
    for (k=k>>1; k>1; k=kk) {
        kk = k>>1; // halve the loop increment
        for (i=k-1; i<n-kk; i+=k) A[i+kk] = A[i]+A[i+kk];
        printf("k: %i, kk: %i ", k, kk);
        printArrayInt(n, A);
    }
}

int main(int argc, char * argv[]) {
    if (argc != 3)
    {
        printf("Usage: iterative_scan n v\n");
        exit(1);
    }

    int n = atoi(argv[1]);
    int v = atoi(argv[2]);

    int a[n];

    for (int i = 0; i < n; i++)
        a[i] = v;

    Scan(a, n);

    printArrayInt(n,a);
}