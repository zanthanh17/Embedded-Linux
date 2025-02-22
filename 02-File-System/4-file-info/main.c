#include <stdio.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <string.h>

int main(int argc, char const *argv[])
{

    const char *filename = "thanh.txt";
    int fd;
    int numb_read, numb_write;
    char buf1[12] = "good\n";

    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        printf("open() thanh.txt failed\n");
    }

    numb_write = write(fd, buf1, strlen(buf1));

    if (numb_write == -1)
    {
        perror("Error writing to file");
        close(fd);
        return EXIT_FAILURE;
    }

    struct stat sb;

    if (stat(filename, &sb) == -1)
    {
        perror("Error getting file status");
        return EXIT_FAILURE;
    }

    printf("File Name: %s\n", filename);

    printf("File Type: ");
    switch (sb.st_mode & S_IFMT)
    {
    case S_IFBLK:
        printf("block device\n");
        break;
    case S_IFCHR:
        printf("character device\n");
        break;
    case S_IFDIR:
        printf("directory\n");
        break;
    case S_IFIFO:
        printf("FIFO/pipe\n");
        break;
    case S_IFLNK:
        printf("symlink\n");
        break;
    case S_IFREG:
        printf("regular file\n");
        break;
    case S_IFSOCK:
        printf("socket\n");
        break;
    default:
        printf("unknown?\n");
    }

    printf("File size: %jd bytes\n", (intmax_t)sb.st_size);

    printf("Last status change:     %s\n", ctime(&sb.st_ctime));

    return 0;
}

// Dap an:
/*File Name: thanh.txt
File Type: regular file
File size: 5 bytes
Last status change:     Tue Feb 11 17:13:12 2025*/
