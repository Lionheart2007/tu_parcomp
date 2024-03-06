// Page 116

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <stdatomic.h>

typedef struct
{
    int cnt0;
    atomic_int cnt1, cnt2;
} count3;
void *updates(void *arguments)
{
    count3 *counters = (count3 *)arguments;
    int i;
    int c1, c2;
    for (i = 0; i < 1000; i++)
    {
        counters->cnt0++;
        c1 = atomic_fetch_add(&(counters->cnt1), 1);
        c2 = atomic_fetch_add(&(counters->cnt2), 1);
        assert(c1 == c2);
        // assert(counters->cnt1 == counters->cnt2);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    { //
        printf("Usage: pthreads p\n");
        exit(1);
    }

    int p = atoi(argv[1]); // (small) number of threads
    int i;
    pthread_t thread[p];
    count3 threadargs = {0, 0, 0};

    for (i = 0; i < p; i++)
    {

        pthread_create(&thread[i], NULL, updates, &threadargs);
    }
    for (i = 0; i < p; i++)
    {
        pthread_join(thread[i], NULL);
    }

    return 0;
}
