#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>

#define MSG_SIZE 100

int fd[2]; // Pipe file descriptors
char *msg = "VoVanThanh21DT1";
char pipe_buff[MSG_SIZE];

int main() {

    pid_t pid;

    // Create a pipe
    if (pipe(fd) == -1) {
        printf("pipe() unsuccessfully\n");
        exit(1);
    }

    // Create a child process
    pid = fork();

    if (pid < 0) {
        printf("fork() unsuccessfully\n");
        exit(1);
    }

    if (pid > 0) { // Parent process
        close(fd[0]); // Close read end

        // Write the message to the pipe
        write(fd[1], msg, strlen(msg) + 1);
        close(fd[1]); // Close write end

        // Wait for the child process to complete
        while (1);
    } else { // Child process
        close(fd[1]); // Close write end

        // Read the message from the pipe
        read(fd[0], pipe_buff, MSG_SIZE);
        close(fd[0]); // Close read end

        // Count characters and print the result
        printf("Received string: \"%s\"\n", pipe_buff);
        printf("Number of characters: %ld\n", strlen(pipe_buff));

        exit(0);
    }

    return 0;
}
