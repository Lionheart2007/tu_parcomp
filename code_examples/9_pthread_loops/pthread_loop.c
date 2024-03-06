// Page 101

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "util.h"

typedef struct
{
    int rank;
    int size;
    int n;
    double *a; // pointer to shared array
} realargs;    // argument structure
void *loop(void *arguments)
{
    realargs *args = (realargs *)arguments;
    int i;
    int s, nn; // start index and number of iterations
    double *a = args->a;
    nn = (args->n) / args->size;

    s = nn * args->rank;
    if (args->rank == args->size - 1)
        nn = args->n - s;
    // part of loop for thread
    for (i = s; i < s + nn; i++)
        a[i] = args->rank; // some loop body
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: pthreads_loop p l\n");
        exit(1);
    }

    int p = atoi(argv[1]); // (small) number of threads
    int l = atoi(argv[2]); // (small) number of threads

    double *a = calloc(l, sizeof(double));

    printf("before: ");
    printArray(l, a);

    int i;
    pthread_t thread[p];
    realargs threadargs[p];

    for (i = 0; i < p; i++)
    {
        threadargs[i].rank = i;
        threadargs[i].size = p;
        threadargs[i].n = l;
        threadargs[i].a = a;
        pthread_create(&thread[i], NULL, loop, &threadargs[i]);
    }
    for (i = 0; i < p; i++)
    {
        pthread_join(thread[i], NULL);
    }

    printf("after: ");
    printArray(l, a);
    free(a);
    return 0;
}
