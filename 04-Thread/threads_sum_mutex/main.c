#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 1000000 // Array size
#define THREADS 4    // Number of threads

int array[SIZE];         // Array containing 1 million integers
long long total_sum = 0; // Global total sum variable
pthread_mutex_t mutex;   // Mutex for synchronization

// Function to calculate the sum of a portion of the array
void *thr_sum_handle(void *arg)
{
    int index = *((int *)arg);            // Get the current thread index
    long long local_sum = 0;              // Local sum variable for each thread
    int start = index * (SIZE / THREADS); // Determine the start index of the assigned portion
    int end = start + (SIZE / THREADS);   // Determine the end index of the assigned portion

    // Compute the sum of the assigned portion of the array
    for (int i = start; i < end; i++)
    {
        local_sum += array[i];
    }

    pthread_mutex_lock(&mutex);   // Lock mutex to ensure synchronization when updating the global sum
    total_sum += local_sum;       // Add local sum to the global total sum
    pthread_mutex_unlock(&mutex); // Unlock mutex

    pthread_exit(NULL); // Exit thread
}

int main()
{
    pthread_t threads[THREADS]; // Array to store threads
    int thread_ids[THREADS];    // Array to store thread IDs

    // Initialize the array with random values
    for (int i = 0; i < SIZE; i++)
    {
        array[i] = rand() % 100; // Assign random values between 0 and 99
    }

    pthread_mutex_init(&mutex, NULL); // Initialize mutex

    // Create threads to compute the sum of different parts of the array
    for (int i = 0; i < THREADS; i++)
    {
        thread_ids[i] = i;                                                 // Assign ID to each thread
        pthread_create(&threads[i], NULL, thr_sum_handle, &thread_ids[i]); // Create thread
    }

    // Wait for all threads to complete
    for (int i = 0; i < THREADS; i++)
    {
        pthread_join(threads[i], NULL); // Wait for thread to finish
    }

    pthread_mutex_destroy(&mutex); // Destroy mutex after completion

    printf("Total sum: %lld\n", total_sum); // Print the final sum of the array
    return 0;                               //