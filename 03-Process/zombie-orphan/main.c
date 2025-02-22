#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void zombie()
{
    pid_t zombie_pid = fork();

    if (zombie_pid >= 0)
    {
        if (zombie_pid == 0)
        {
            printf("[Zombie] Im the child process, my PID: %d\n", getpid());
        }
        else
        {
            printf("[Zombie] Im the parent process, My PID is: %d\n", getpid());
            printf("Create zombie process\n");
            while (1)
                ;
        }
    }
    else
    {
        printf("[Zombie] fork failed");
    }
}

void orphan()
{
    pid_t orphan_pid = fork();

    if (orphan_pid >= 0)
    {
        if (orphan_pid == 0)
        {
            sleep(3);
            printf("[Orphan] Im the child process, My PID is: %d, my parent PID is: %d\n", getpid(), getppid());
        }
        else
        {
            printf("[Orphan] Im the parent process, My PID is: %d\n", getpid());
            printf("Create orphan process\n");
            exit(0);
        }
    }
    else
    {
        printf("[Orphan] fork failed");
    }
}

int main()
{
    zombie();
    printf("\n");
    orphan();
    return 0;
}