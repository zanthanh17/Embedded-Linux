#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>

#define MSG_SIZE 100

int pipe1[2], pipe2[2]; // pipe1: parent -> child 1, pipe2: child 1 -> child 2
char *msg = "You so handsome";
char pipe_buff1[MSG_SIZE], pipe_buff2[MSG_SIZE];

int main() {

    pid_t pid1, pid2;

    // Create two pipes
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        printf("pipe() unsuccessfully\n");
        exit(1);
    }

    // Create child process 1
    pid1 = fork();

    if (pid1 < 0) {
        printf("fork() unsuccessfully\n");
        exit(1);
    }

    if (pid1 == 0) { // Child process 1
        close(pipe1[1]); // Close write end of pipe1
        read(pipe1[0], pipe_buff1, MSG_SIZE); // Read data from parent
        close(pipe1[0]); // Close read end after reading

        // Modify the message (append text)
        strcat(pipe_buff1, " -> number 1");

        close(pipe2[0]); // Close read end of pipe2
        write(pipe2[1], pipe_buff1, MSG_SIZE); // Send data to child 2
        close(pipe2[1]); // Close write end after writing
        exit(0);
    }

    // Create child process 2
    pid2 = fork();
    if (pid2 < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid2 == 0) { // Child process 2
        close(pipe2[1]); // Close write end of pipe2
        read(pipe2[0], pipe_buff2, MSG_SIZE); // Read data from child 1
        close(pipe2[0]); // Close read end after reading

        printf("Child 2 received: %s\n", pipe_buff2); // Print received message

        exit(0);
    }

    // Parent process
    close(pipe1[0]); // Close read end of pipe1
    write(pipe1[1], msg, MSG_SIZE); // Send data to child 1
    close(pipe1[1]); // Close write end after writing

    // Wait for both child processes to finish
    while (1);

    return 0;
}
