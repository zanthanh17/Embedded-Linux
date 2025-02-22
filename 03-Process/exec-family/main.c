#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <1|2>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "1") == 0)
    {
        execlp("/bin/ls", "ls", "-l", "-a", "-h", NULL);
    }
    else if (strcmp(argv[1], "2") == 0)
    {
        execlp("/bin/date", "date", NULL);
    }
    else
    {
        printf("ERROR\n");
    }

    return 0;
}