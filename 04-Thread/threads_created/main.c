#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Function executed by the threads
void *print_message(void *thread_id)
{
    long tid = (long)thread_id; // Convert argument to long
    printf("Thread %ld: Hello from thread!\n", tid + 1);
    pthread_exit(NULL); // Terminate the thread
}

int main()
{
    pthread_t threads[2]; // Array to store thread IDs
    int ret;
    long t;

    // Create two threads
    for (t = 0; t < 2; t++)
    {
        printf("Main: Creating thread %ld\n", t + 1);
        ret = pthread_create(&threads[t], NULL, print_message, (void *)t);
        if (ret)
        {
            printf("pthread_create() error number=%d\n", ret);
            exit(-1);
        }
    }

    // Wait for both threads to finish
    for (t = 0; t < 2; t++)
    {
        pthread_join(threads[t], NULL);
    }

    printf("Main: All threads completed. Exiting program.\n");
    return 0;
}
