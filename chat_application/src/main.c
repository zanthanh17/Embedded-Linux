#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "../inc/cli.h"
#include "../inc/network.h"
#include "../inc/connection.h"
#include "../inc/utils.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        return 1;
    }
    
    int port = atoi(argv[1]);
    pthread_t server;
    if (pthread_create(&server, NULL, server_thread, &port) != 0) {
        perror("Server thread creation failed");
        exit(1);
    }
    
    init_connections();
    sleep(1);
    show_title();
    
    char command[256];
    while (1) {
        printf(ANSI_COLOR_BLUE "Enter your command: " ANSI_COLOR_RESET);
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0;
        process_command(command);
    }
    return 0;
}