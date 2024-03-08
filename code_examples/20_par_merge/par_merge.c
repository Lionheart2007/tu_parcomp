// Page 147
// REMARK // determine rank of A[m] in B is WRONG it is A[r]

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <omp.h>
#include "util.h"

void par_copy(int C[], int A[], int n)
{
#pragma omp parallel for
    for (int i = 0; i < n; i++)
        C[i] = A[i];
}

int rank(int toRank, int B[], int m)
{
    int rank = 0;
    int previous = B[0];
    int next = B[0];

    // Handling boundary cases
    if (toRank <= previous)
        return 0; // toRank is smaller than the smallest value in B
    if (toRank > B[m - 1])
        return m; // toRank is greater than the largest value in B

    // Iterate through the array
    for (; rank < m - 1; rank++)
    {
        next = B[rank + 1];
        if (previous < toRank && toRank <= next)
            break;

        previous = next;
    }

    // Return the rank (position) of toRank
    return rank + 1;
}

void par_merge(int A[], int n, int B[], int m, int C[])
{
    if (n < m)
    { // for the bounds, it must hold that n>=m
        int k;
        int *X;
        k = n;
        n = m;
        m = k;
        X = A;
        A = B;
        B = X;
    }
    if (m == 0)
    {
        par_copy(C, A, n); // copy in parallel
        return;
    }
    int r = n / 2;            // it holds that n>=m
    int s = rank(A[r], B, m); // determine rank of A[m] in B
    C[r + s] = A[r];

    par_merge(A, r, B, s, C);
    par_merge(A + r + 1, n - r - 1, B + s, m - s, C + r + s + 1);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: par_merge length threads\n");
        exit(1);
    }
    int n = atoi(argv[1]);
    int p = atoi(argv[2]);

    omp_set_num_threads(p);

    int a[n];
    int b[n];
    int c[2 * n];

    for (int i = 0; i < n; i++)
    {
        a[i] = i * 2 + 1;
        b[i] = i * 2;
    }

    printf("a: ");
    printArrayInt(n, a);
    printf("b: ");
    printArrayInt(n, b);

    par_merge(a, n, b, n, c);

    printf("c: ");
    printArrayInt(2 * n, c);
}
