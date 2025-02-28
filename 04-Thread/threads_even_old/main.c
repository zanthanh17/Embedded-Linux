#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define SIZE 100

int numbers[SIZE];
int even_count = 0, odd_count = 0;

void *even_handle(void *arg)
{
    for (int i = 0; i < SIZE; i++)
    {
        if (numbers[i] % 2 == 0)
        {
            even_count++;
            printf("even: %d\n", numbers[i]);
        }
    }
    pthread_exit(NULL);
}

void *odd_handle(void *arg)
{
    for (int i = 0; i < SIZE; i++)
    {
        if (numbers[i] % 2 != 0)
        {
            odd_count++;
            printf("odd: %d\n", numbers[i]);
        }
    }
    pthread_exit(NULL);
}

int main()
{
    srand(time(NULL));

    for (int i = 0; i < SIZE; i++)
    {
        numbers[i] = rand() % 100 + 1;
    }

    pthread_t even_thread, odd_thread;

    pthread_create(&even_thread, NULL, even_handle, NULL);
    pthread_create(&odd_thread, NULL, odd_handle, NULL);

    pthread_join(even_thread, NULL);
    pthread_join(odd_thread, NULL);

    printf("Total even numbers: %d\n", even_count);
    printf("Total odd numbers: %d\n", odd_count);

    return 0;
}
