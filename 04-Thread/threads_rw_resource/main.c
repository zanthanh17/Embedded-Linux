#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define READERS 5 // Number of reader threads
#define WRITERS 2 // Number of writer threads

int data = 0;            // Shared data variable
pthread_rwlock_t rwlock; // Read-write lock

// Function executed by reader threads
void *reader_handle(void *arg)
{
    int id = *((int *)arg);
    for (int i = 0; i < 5; i++)
    {
        pthread_rwlock_rdlock(&rwlock); // Acquire read lock (multiple readers allowed)
        printf("Reader %d reads data: %d\n", id, data);
        pthread_rwlock_unlock(&rwlock); // Release read lock
        usleep(100000);                 // Sleep to simulate processing delay
    }
    pthread_exit(NULL);
}

// Function executed by writer threads
void *writer_handle(void *arg)
{
    int id = *((int *)arg);
    for (int i = 0; i < 5; i++)
    {
        pthread_rwlock_wrlock(&rwlock); // Acquire write lock (only one writer allowed)
        data++;
        printf("Writer %d updates data to: %d\n", id, data);
        pthread_rwlock_unlock(&rwlock); // Release write lock
        usleep(200000);                 // Sleep to simulate processing delay
    }
    pthread_exit(NULL);
}

int main()
{
    pthread_t readers[READERS], writers[WRITERS];
    int ids;

    pthread_rwlock_init(&rwlock, NULL); // Initialize the read-write lock

    // Create reader threads
    for (int i = 0; i < READERS; i++)
    {
        ids = i + 1;
        pthread_create(&readers[i], NULL, reader_handle, &ids);
    }

    // Create writer threads
    for (int i = 0; i < WRITERS; i++)
    {
        ids = i + 1;
        pthread_create(&writers[i], NULL, writer_handle, &ids);
    }

    // Wait for all reader threads to complete
    for (int i = 0; i < READERS; i++)
    {
        pthread_join(readers[i], NULL);
    }

    // Wait for all writer threads to complete
    for (int i = 0; i < WRITERS; i++)
    {
        pthread_join(writers[i], NULL);
    }

    pthread_rwlock_destroy(&rwlock);        // Destroy the read-write lock
    printf("Final data value: %d\n", data); // Print the final value of data
    return 0;
}
