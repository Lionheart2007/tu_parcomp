// Page 116
// REMARK is it really instructive in any way to look at the thread local found?
// Knowing whether the primes a thread reports are in any way off would require that
// I know how many it should report. Which I don't. Neither do I know which numbers it looked at,
// nor how many primes are among the numbers I looked at. The only thing I can do is to sum the found counts and to check,
// but that is bothresome and also requires googling (how much the sum should be).

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <stdatomic.h>

typedef struct
{
    int rank;
    int limit;
    int *next; // shared counter
    int found;
} realargs;

int isprime(int n)
{
    int isprime = 1;

    if (n < 2)
        return 0;

    for (int i = 2; i * n < n * n; i++)
    {
        if (n % i == 0)
        {
            isprime = 0;
            break;
        }
    }

    return isprime;
}

void *primes_race(void *arguments)
{
    int i;
    realargs *next = (realargs *)arguments;
    printf("HELLO FROM %i\n", next->rank);

    do
    {
        i = atomic_fetch_add(next->next, 1);
        if (i < next->limit)
        {
            if (isprime(i))
            { // prime found, take action
                next->found++;
                // printf("%i Found prime %i\n", next->rank, i);
            }
        }
        else
            break;
    } while (1);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: pthreads_primes_race_atomic p l\n");
        exit(1);
    }

    int p = atoi(argv[1]); // (small) number of threads
    int l = atoi(argv[2]);

    int i;
    pthread_t thread[p];

    int counter = 0;
    realargs threadargs[p];

    for (i = 0; i < p; i++)
    {
        threadargs[i].rank = i;
        threadargs[i].limit = l;
        threadargs[i].next = &counter;
        threadargs[i].found = 0;

        pthread_create(&thread[i], NULL, primes_race, &threadargs[i]);
    }
    for (i = 0; i < p; i++)
    {
        pthread_join(thread[i], NULL);
        printf("Thread %i found %i\n", threadargs[i].rank, threadargs[i].found);
    }

    return 0;
}
