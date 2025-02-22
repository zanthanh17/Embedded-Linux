#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    pid_t child_pid;
    int count = 2;

    printf("counter: %d\n", count);

    child_pid = fork();

    if (child_pid >= 0)
    {
        if (child_pid == 0)
        {
            printf("\nIm the child process, counter: %d\n", ++count);
            printf("My PID is: %d, my parent PID is: %d\n", getpid(), getppid());
        }
        else
        {
            printf("\nIm the parent process, counter: %d\n", ++count);
            printf("My PID is: %d\n", getpid());
            while (1)
                ;
        }
    }
    else
    {
        printf("ERROR\n");
    }

    return 0;
}