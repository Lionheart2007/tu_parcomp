#include <stdlib.h>
#include <stdio.h>
#include "util.h"

void Scan(int A[], int n)
{
    printf("n: %i\na: ", n);
    printArrayInt(n, A);
    if (n == 1)
        return;
    int B[n / 2]; // careful with stack allocation of larger n
    int i;
    for (i = 0; i < n / 2; i++)
        B[i] = A[2 * i] + A[2 * i + 1];

    Scan(B, n / 2);
    A[1] = B[0]; // A[0] is always in place (correct)
    for (i = 1; i < n / 2; i++)
    {
        A[2 * i] = B[i - 1] + A[2 * i];
        A[2 * i + 1] = B[i];
    }

    if (n % 2 == 1)
        A[n - 1] = B[n / 2 - 1] + A[n - 1];

    printf("Out: ");
    printArrayInt(n, A);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: recursive_scan n v\n");
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