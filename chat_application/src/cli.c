#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../inc/cli.h"
#include "../inc/connection.h"
#include "../inc/utils.h"

void show_help() {
    draw_border();
    printf(ANSI_COLOR_GREEN "Available commands:\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_CYAN "-------------------------------------------------\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_YELLOW "help              - Show this help message\n");
    printf("connect <ip> <port> - Connect to a peer\n");
    printf("send <id> <message> - Send message to a connection\n");
    printf("list              - List all active connections\n");
    printf("terminate <id>    - Terminate a connection\n");
    printf("exit              - Exit the application\n");
    printf(ANSI_COLOR_CYAN "-------------------------------------------------\n" ANSI_COLOR_RESET);
    draw_border();
}

void process_command(char *command) {
    char cmd[256];
    char arg1[256];
    char arg2[256];
    int result;

    if (sscanf(command, "%s %s %s", cmd, arg1, arg2) >= 1) {
        if (strcmp(cmd, "help") == 0) {
            show_help();
        } else if (strcmp(cmd, "connect") == 0) {
            int port = atoi(arg2);
            result = connect_to_peer(arg1, port);
            if (result == 0) {
                draw_border();
                printf(ANSI_COLOR_GREEN "Connected successfully\n" ANSI_COLOR_RESET);
                draw_border();
            }
        } else if (strcmp(cmd, "send") == 0) {
            int id = atoi(arg1);
            send_message(id, arg2);
        } else if (strcmp(cmd, "list") == 0) {
            list_connections();
        } else if (strcmp(cmd, "terminate") == 0) {
            int id = atoi(arg1);
            terminate_connection(id);
        } else if (strcmp(cmd, "exit") == 0) {
            exit(0);
        } else {
            draw_border();
            printf(ANSI_COLOR_RED "Error: Unknown command\n" ANSI_COLOR_RESET);
            draw_border();
            show_help();
        }
    }
}