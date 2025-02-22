#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

volatile sig_atomic_t second_count = 0;

void sigalrm_handler()
{
    second_count++;

    printf("Timer: %d second\n", second_count);

    if (second_count < 10)
    {
        alarm(1);
    }
    else
    {
        exit(EXIT_SUCCESS);
    }
}

int main()
{
    if (signal(14, sigalrm_handler) == SIG_ERR)
    {
        fprintf(stderr, "Cannot handle SIGALRM\n");
        exit(EXIT_FAILURE);
    }

    alarm(1);

    printf("process ID: %d\n", getpid());

    while (1)
        ;

    return 0;
}