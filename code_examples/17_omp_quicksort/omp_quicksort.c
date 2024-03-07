// Page 138
// REMARK sorry, unimplemented: ‘inscan’ modifier on ‘reduction’ clause not supported yet
// requires update of compiler

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <omp.h>
#include "util.h"

void Quicksort(int a[], int n)
{
    int i, j;
    int aa;
    if (n < 2)
        return; // recursion all the way down
    // partition
    int pivot = a[0]; // choose an element (non-randomly...)
    i = 0;
    j = n;
    while (1)
    {
        while (++i < j && a[i] < pivot)
            ;
        while (a[--j] > pivot)
            ;
        if (i >= j)
            break;
        aa = a[i];
        a[i] = a[j];
        a[j] = aa;
    }
    // swap pivot
    aa = a[0];
    a[0] = a[j];
    a[j] = aa;
#pragma omp task shared(a)
    Quicksort(a, j);
#pragma omp task shared(a)
    Quicksort(a + j + 1, n - j - 1);
    // #pragma omp taskwait - not needed
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: omp_task length threads\n");
        exit(1);
    }
    int n = atoi(argv[1]);
    int p = atoi(argv[2]);

    omp_set_num_threads(p);

    int a[n];
    fillRandomArrayIntMax(n, a, 1000);

    // printf("a: ");
    // printArrayInt(n, a);

    double start = omp_get_wtime();
#pragma omp parallel
    {
#pragma omp single nowait
        Quicksort(a, n);
        // #pragma omp taskwait
    }
    double stop = omp_get_wtime();

    // printf("a: ");
    //  printArrayInt(n, a);
    printf("time: %f\n", stop - start);
}
