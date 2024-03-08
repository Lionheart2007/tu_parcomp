// Page 143
// REMARK sorry, unimplemented: ‘inscan’ modifier on ‘reduction’ clause not supported yet
// requires update of compiler

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <omp.h>
#include "util.h"

int primesieve(int n, int primes[])
{
    int i, j, k;
    unsigned char *mark;
    mark = (unsigned char *)malloc(n * sizeof(unsigned char));
#pragma omp parallel for private(i) schedule(static, 1024)
    for (i = 2; i < n; i++)
        mark[i] = 1; // possibly prime
    k = 0;
    for (i = 2; i * i < n; i++)
    {
        if (mark[i])
        {
            primes[k++] = i;
#pragma omp parallel for private(j) schedule(static)
            for (j = i * i; j < n; j += i)
                mark[j] = 0; // composite, not prime
        }
    }
    j = i;
#pragma omp parallel for reduction(inscan, + : k)
    for (i = j; i < n; i++)
    {
        if (mark[i])
            primes[k] = i;
#pragma omp scan exclusive(k)
        if (mark[i])
            k = k + 1;
    }
    free(mark);
    return k;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: omp_primesieve length threads\n");
        exit(1);
    }
    int n = atoi(argv[1]);
    int p = atoi(argv[2]);

    omp_set_num_threads(p);

    int a[n / 2];

    for (int i = 0; i < n / 2; i++)
        a[i] = 0;

    printf("found primes: %i\n", primesieve(n, a));

    int firstZero = 0;
    for (; firstZero < n / 2; firstZero++)
        if (!a[firstZero])
            break;

    printf("a: ");
    printArrayInt(firstZero, a);
}
