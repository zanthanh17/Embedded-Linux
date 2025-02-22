#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <sys/select.h>

void sigint_handler(int sig)
{
    printf("SIGINT received.\n");
}

void sigterm_handler(int sig)
{
    printf("SIGTERM received.\n");
    exit(EXIT_SUCCESS);
}

int main()
{
    char buffer[256];
    fd_set readfds;
    struct timeval timeout;

    if (signal(SIGINT, sigint_handler) == SIG_ERR)
    {
        fprintf(stderr, "Cannot handle SIGINT\n");
        exit(EXIT_FAILURE);
    }

    if (signal(SIGTERM, sigterm_handler) == SIG_ERR)
    {
        fprintf(stderr, "Cannot handle SIGTERM\n");
        exit(EXIT_FAILURE);
    }

    printf("process ID: %d\n", getpid());

    while (1)
    {

        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);

        timeout.tv_sec = 1; // 1 giây
        timeout.tv_usec = 0;

        int activity = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout);

        if (activity > 0 && FD_ISSET(STDIN_FILENO, &readfds))
        {
            if (fgets(buffer, sizeof(buffer), stdin) != NULL)
            {
                printf("You entered: %s", buffer);
            }
        }
    }
}