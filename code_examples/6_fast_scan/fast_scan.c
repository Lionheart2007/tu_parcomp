//Page 70

#include <stdlib.h>
#include <stdio.h>
#include "util.h"

void Scan(int A[], int n)
{
    int B[n];
    int i, k;
    int *a, *b, *t;

    a = A; b = B;
    k = 1;
    while (k<n) {
    // update into B
    for (i=0; i<k; i++) b[i] = a[i];
    for (i=k; i<n; i++) b[i] = a[i-k]+a[i];
        k <<= 1; // double
        // swap
        t = a; a = b; b = t;
    }
    if (a!=A) {
        for (i=0; i<n; i++) A[i] = B[i]; // copy back when necessary
    }
}

int main(int argc, char * argv[]) {
    if (argc != 3)
    {
        printf("Usage: fast_scan n v\n");
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