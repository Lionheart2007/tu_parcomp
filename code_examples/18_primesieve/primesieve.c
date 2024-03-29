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
    for (i = 2; i < n; i++)
        mark[i] = 1; // possibly prime
    k = 0;
    for (i = 2; i * i < n; i++)
    {
        if (mark[i])
        {
            primes[k++] = i;
            for (j = i * i; j < n; j += i)
                mark[j] = 0; // composite, not prime
        }
    }
    for (; i < n; i++)
    {
        if (mark[i])
            primes[k++] = i;
    }
    free(mark);
    return k;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: primesieve length\n");
        exit(1);
    }
    int n = atoi(argv[1]);

    int a[n];

    for (int i = 0; i < n; i++)
        a[i] = 0;

    printf("a: ");
    printArrayInt(n, a);

    printf("found primes: %i\n", primesieve(n, a));

    printf("a: ");
    printArrayInt(n, a);
}
