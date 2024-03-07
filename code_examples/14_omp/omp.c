// Page 131
// REMARK I'm leaving out the snippets on pages 132, 133

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <omp.h>
#include "util.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: omp n p\n");
        exit(1);
    }
    int i;
    int n = atoi(argv[1]);
    int p = atoi(argv[2]);

    omp_set_num_threads(p);

    int t = omp_get_max_threads();
    int iter[t]; // iterations per thread
    int loop[n]; // who did iteration i; careful for large n
    for (i = 0; i < t; i++)
        iter[i] = 0;
#pragma omp parallel for schedule(static)
    for (i = 0; i < n; i++)
    {
        loop[i] = omp_get_thread_num();
        iter[omp_get_thread_num()]++; // problematic for large n
    }
    int nn = 0;
    for (i = 0; i < t; i++)
        nn += iter[i];
    assert(nn == n); // all work done

    printArrayInt(n, loop);

    return 0;
}
