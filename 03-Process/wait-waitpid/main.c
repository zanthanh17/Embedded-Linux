#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    pid_t child_pid;
    int status, rv;

    child_pid = fork();

    if (child_pid >= 0)
    {
        if (0 == child_pid)
        {
            printf("Im the child process, my PID: %d\n", getpid());
            printf("Child process terminate after 5 seconds\n");
            sleep(5);
            exit(111);
        }
        else
        {
            // while(1);
            rv = wait(&status);

            if (rv == -1)
            {
                printf("wait() unsuccessful\n");
            }

            printf("\nIm the parent process, PID child process: %d\n", rv);

            if (WIFEXITED(status))
            {
                printf("Normally termination, status = %d\n", WEXITSTATUS(status));
            }
            else
            {
                printf("Child did NOT exit normally\n");
            }
        }
    }
    else
    {
        printf("fork() unsuccessfully\n");
    }

    return 0;
}