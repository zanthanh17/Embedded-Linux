#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void sigtstp_handler(int sig)
{
    printf("SIGTSTP ignored \n");
}

int main()
{
    if (signal(20, sigtstp_handler) == SIG_ERR)
    {
        fprintf(stderr, "Cannot handle SIGTSTP\n");
        exit(EXIT_FAILURE);
    }

    printf("process ID: %d\n", getpid());

    printf("Program running. Press Ctrl+Z to send SIGTSTP.\n");

    while (1)
        ;

    return 0;
}