#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

volatile sig_atomic_t sigint_count = 0;

void sigint_handler()
{
    sigint_count++;

    printf("SIGINT recerved: %d\n", sigint_count);

    if (sigint_count >= 3)
    {
        printf("End\n");
        exit(EXIT_SUCCESS);
    }
}

int main()
{
    if (signal(SIGINT, sigint_handler) == SIG_ERR)
    {
        fprintf(stderr, "Cannot handle SIGINT\n");
        exit(EXIT_FAILURE);
    }

    printf("process ID: %d\n", getpid());

    while (1)
        ;

    return 0;
}