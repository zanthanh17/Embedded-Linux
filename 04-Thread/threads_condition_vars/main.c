#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define ITERATIONS 10

int data = 0;  // Shared resource
int ready = 0; // Flag to indicate data availability

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// Producer function
void *producer(void *arg)
{
    for (int i = 0; i < ITERATIONS; i++)
    {
        sleep(1); // Simulate data production
        pthread_mutex_lock(&lock);

        data = rand() % 10 + 1; // Generate a number between 1 and 10
        ready = 1;
        printf("Producer produced [%d]: %d\n", i + 1, data);

        pthread_cond_signal(&cond); // Signal consumer
        pthread_mutex_unlock(&lock);
    }
    pthread_exit(NULL); // exit or return;
}

// Consumer function
void *consumer(void *arg)
{
    for (int i = 0; i < ITERATIONS; i++)
    {
        pthread_mutex_lock(&lock);

        while (ready == 0)
        { // Wait until data is ready
            pthread_cond_wait(&cond, &lock);
        }

        printf("Consumer consumed [%d]: %d\n", i + 1, data);
        ready = 0; // Reset flag

        pthread_mutex_unlock(&lock);
    }
    pthread_exit(NULL); // exit or return;
}

int main()
{
    pthread_t producer_thr, consumer_thr;
    int ret;

    // srand(time(NULL)); // Seed random number generator

    if (ret = pthread_create(&producer_thr, NULL, producer, NULL))
    {
        printf("pthread_create() error number=%d\n", ret);
        return -1;
    }

    if (ret = pthread_create(&consumer_thr, NULL, consumer, NULL))
    {
        printf("pthread_create() error number=%d\n", ret);
        return -1;
    }

    pthread_join(producer_thr, NULL);
    pthread_join(consumer_thr, NULL);

    return 0;
}
