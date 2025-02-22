#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char *filename = argv[1];
    int numb_read, numb_write;
    int num_bytes = atoi(argv[2]);
    char mode = argv[3][0];
    char *data = argv[4];

    if (argc != 5)
    {
        fprintf(stderr, "Usage: %s filename num-bytes [r/w] \"Hello\"\n", argv[0]);
    }

    if (mode == 'r') // Read Mode
    {
        int fd = open(filename, O_RDONLY);
        if (fd == -1)
        {
            printf("Error opening file");
        }

        char buf[num_bytes + 1];
        numb_read = read(fd, buf, num_bytes);

        if (numb_read == -1)
        {
            printf("Error reading file");
            close(fd);
        }

        buf[numb_read] = '\0'; // End Buffer

        printf("Read from file: %s\n", buf);

        close(fd);
    }
    else if (mode == 'w') // Write Mode
    {
        int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);

        if (fd == -1)
        {
            printf("Error opening file");
        }

        numb_write = write(fd, data, num_bytes);

        if (numb_write == -1)
        {
            printf("Error writing file");
            close(fd);
        }

        printf("Written to file: %s\n", data);

        close(fd);
    }
    else
    {
        printf("Invalid mode! Use 'r' for read or 'w' for write.\n");
    }

    return 0;
}
