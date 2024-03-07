// Page 131
// REMARK sorry, unimplemented: ‘inscan’ modifier on ‘reduction’ clause not supported yet
// requires update of compiler

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <omp.h>
#include "util.h"

int occurs(int x, int a[], int n)
{
    if (n == 1)
    {
        return (a[0] == x) ? 1 : 0;
    }
    else
    {
        int s0, s1; // private variables for executing thread
#pragma omp task shared(s0, a)
        s0 = occurs(x, a, n / 2);
#pragma omp task shared(s1, a)
        s1 = occurs(x, a + n / 2, n - n / 2);
#pragma omp taskwait
        return s0 + s1;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Usage: omp_task length threads value\n");
        exit(1);
    }
    int n = atoi(argv[1]);
    int p = atoi(argv[2]);
    int x = atoi(argv[3]);

    omp_set_num_threads(p);

    int a[n];
    fillRandomArrayInt(n, a);

    printf("a: ");
    printArrayInt(n, a);

    int s;

#pragma omp parallel shared(x) shared(s) shared(a)
    {
#pragma omp single
        s = occurs(x, a, n);
    }

    printf("s: %i\n", s);

    return 0;
}
