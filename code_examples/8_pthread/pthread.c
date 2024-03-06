#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct
{ // the real arguments to thread functions
    int rank;
} realargs;

int test = 0;

void *hello(void *arguments)
{
    realargs *args = (realargs *)arguments;
    // a classic race; try it, and see later
    printf("Thread %d starting\n", args->rank);
    test += 1;
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: pthreads p\n");
        exit(1);
    }

    int p = atoi(argv[1]); // (small) number of threads
    int i;
    pthread_t thread[p];
    realargs threadargs[p];
    for (i = 0; i < p; i++)
    {
        threadargs[i].rank = i;
        pthread_create(&thread[i], NULL, hello, &threadargs[i]);
    }
    for (i = 0; i < p; i++)
    {
        pthread_join(thread[i], NULL);
    }

    printf("%i\n", test);
    return 0;
}
