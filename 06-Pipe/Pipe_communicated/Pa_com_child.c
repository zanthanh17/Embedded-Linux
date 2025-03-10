#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>

#define MSG_SIZE    100

char *msg = "You so handsome number 1!!!";
char pipe_buff[MSG_SIZE];
int fd[2], i;

int main() {

    pid_t pid;

    // create pipe
    if (pipe(fd) < 0) {
        printf("pipe() unsuccessfully\n");
        exit(1);
    }

    // child process
    pid = fork();
    
    if (pid < 0) {
        perror("fork failed");
        return 1;
    }
    
    if (pid > 0) { // parent process
        close(fd[0]); // close read of pipe
        write(fd[1], msg, MSG_SIZE); // send data
        close(fd[1]); // close write after sent 
    } else { // child process
        close(fd[1]); // close write of pipe
        read(fd[0], pipe_buff, MSG_SIZE); // read data pipe
        printf("Child received: %s\n", pipe_buff);
        close(fd[0]); // close read after read already!
    }
    
    return 0;
}