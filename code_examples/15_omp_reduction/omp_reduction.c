// Page 135
// REMARK sorry, unimplemented: ‘inscan’ modifier on ‘reduction’ clause not supported yet
// requires update of compiler

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <omp.h>
#include "util.h"

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Usage: omp_reduction length threads value\n");
        exit(1);
    }
    int i;
    int n = atoi(argv[1]);
    int p = atoi(argv[2]);
    int v = atoi(argv[3]);

    int x = 1;
    int a[n];
    int b[n];

    omp_set_num_threads(p);

#pragma omp parallel for
    for (int i = 0; i < n; i++)
    {
        a[i] = b[i] = v;
    }

    printf("a: ");
    printArrayInt(n, a);

    printf("b: ");
    printArrayInt(n, b);

#pragma omp parallel for reduction(inscan, * : x)
    for (i = 0; i < n; i++)
    {
        x += a[i]; // reduce
#pragma omp scan inclusive(x)
        b[i] = x; // and save the prefix (current value)
    }

    printf("x: %i\n", x);
    printf("b: ");
    printArrayInt(n, b);

    x = 1;
#pragma omp parallel for reduction(inscan, * : x)
    for (i = 0; i < n; i++)
    {
        b[i] = x; // save the prefix
#pragma omp scan exclusive(x)
        x += a[i]; // and reduce for next iteration
    }

    printf("x: %i\n", x);
    printf("b: ");
    printArrayInt(n, b);

    return 0;
}
