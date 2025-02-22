#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main()
{
    const char *filename = "thanh.txt";
    int fd;
    int numb_read, numb_write;
    char buf1[12] = "good\n";

    fd = open(filename, O_WRONLY | O_APPEND);

    if (fd == -1)
    {
        printf("open() thanh.txt failed\n");
    }

    lseek(fd, 0, SEEK_SET);

    write(fd, buf1, strlen(buf1));

    close(fd);

    printf("Done!\n");

    return 0;
}
