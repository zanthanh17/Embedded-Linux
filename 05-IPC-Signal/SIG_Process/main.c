#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

volatile sig_atomic_t signal_count = 0;

void signal_handler(int sig)
{
    signal_count++;
    if (sig == SIGUSR1)
    {
        printf("Received signal from parent: %d\n", signal_count);
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
            while (1)
                pause();
        }
        else
        {
            for (int i = 0; i < 5; i++)
            {
                printf("Wait for the SIGUSR1 signal to be sent to the process for 2 seconds\n");
                sleep(2);
                kill(pid, SIGUSR1);
            }
            wait(NULL);
            printf("Parent process finished.\n");
        }
    }
    else
    {
        printf("fork failed\n");
    }

    return 0;
}
