#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 3
#define INCREMENT 1000000

// Global counter
long counter = 0;

// Mutex for thread synchronization
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// Function executed by threads
void *handle_thr(void *arg)
{
    for (long i = 0; i < INCREMENT; i++)
    {
        pthread_mutex_lock(&lock);   // Lock the mutex
        counter++;                   // Critical section
        pthread_mutex_unlock(&lock); // Unlock the mutex
    }

    pthread_exit(NULL);
}

int main()
{
    pthread_t threads[NUM_THREADS];
    int ret;

    // Create threads
    for (long t = 0; t < NUM_THREADS; t++)
    {
        ret = pthread_create(&threads[t], NULL, handle_thr, NULL);
        if (ret)
        {
            printf("pthread_create() error number=%d\n", ret);
            exit(-1);
        }
    }

    // Wait for all threads to complete
    for (long t = 0; t < NUM_THREADS; t++)
    {
        pthread_join(threads[t], NULL);
    }

    // Print final counter value
    printf("Final counter value: %ld\n", counter);

    return 0;
}
