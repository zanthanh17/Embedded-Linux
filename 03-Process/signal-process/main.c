#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void signal_handler(int sig)
{
    if (sig == SIGUSR1)
    {
        printf("The progress you receive SIGUSR1!\n");
    }
}

int main()
{
    pid_t pid = fork();

    if (pid >= 0)
    {

        if (pid == 0)
        {
            signal(SIGUSR1, signal_handler);
            printf("The sub-process is waiting for the signal...\n");
            while (1)
                ;
        }
        else
        {
            printf("Wait for the SIGUSR1 signal to be sent to the process for 3 seconds\n");
            sleep(3);
            kill(pid, SIGUSR1);
            wait(NULL);
        }
    }
    else
    {
        printf("fork failed\n");
    }

    return 0;
}
