// Page 101

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>

typedef struct
{ // the real arguments to thread functions
    int rank;
} realargs;

typedef struct
{
    int readers;         // count number of readers
    int waiting, writer; // writers waiting
    pthread_cond_t read_ok, write_ok;
    pthread_mutex_t gatekeeper;
} rwlock_t;

rwlock_t lock;

void lock_read(rwlock_t *rwlock)
{
    pthread_mutex_lock(&rwlock->gatekeeper);
    while (rwlock->waiting > 0 || rwlock->writer)
    {
        pthread_cond_wait(&rwlock->read_ok, &rwlock->gatekeeper);
    }
    // acquired for read (possibly more than one)
    rwlock->readers++;
    pthread_mutex_unlock(&rwlock->gatekeeper);
    assert(rwlock->writer == 0); // at any time before unlock
}

void lock_write(rwlock_t *rwlock)
{
    pthread_mutex_lock(&rwlock->gatekeeper);
    rwlock->waiting++;
    while (rwlock->writer || rwlock->readers > 0)
    {
        pthread_cond_wait(&rwlock->write_ok, &rwlock->gatekeeper);
    }
    // acquired for read (possibly more than one)
    rwlock->waiting--;
    rwlock->writer = 1; // acquired for write
    pthread_mutex_unlock(&rwlock->gatekeeper);
    assert(rwlock->readers == 0); // at any time before unlock
}

void unlock_readwrite(rwlock_t *rwlock)
{
    pthread_mutex_lock(&rwlock->gatekeeper);
    if (rwlock->writer)
        rwlock->writer = 0; // done writing
    else
        rwlock->readers--; // one less reading
    pthread_mutex_unlock(&rwlock->gatekeeper);
    // resume possibly waiting threads
    if (rwlock->readers == 0 && rwlock->waiting > 0)
    {
        pthread_cond_signal(&rwlock->write_ok); // wake up write
    }
    else
        pthread_cond_broadcast(&rwlock->read_ok); // wake up readers
}

void *hello(void *arguments)
{
    realargs *args = (realargs *)arguments;
    // a classic race; try it, and see later
    lock_read(&lock);
    sleep(1);
    printf("Thread %d starting\n", args->rank);
    unlock_readwrite(&lock);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: pthreads p\n");
        exit(1);
    }

    lock.readers = 0;
    lock.waiting = 0;
    lock.writer = 0;
    pthread_mutex_init(&lock.gatekeeper, NULL);
    pthread_cond_init(&lock.read_ok, NULL);
    pthread_cond_init(&lock.write_ok, NULL);

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

    return 0;
}
