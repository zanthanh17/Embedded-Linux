#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../inc/cli.h"         // @include Header file for CLI functions
#include "../inc/connection.h"  // @include Header file for connection-related functions
#include "../inc/utils.h"       // @include Header file for utility functions
#include "../inc/network.h"

// @function show_help
// @brief Displays the list of available commands
// @details Prints a formatted help menu with command descriptions to the console
void show_help() {
    draw_border();                                      // @call Draws a visual border for output formatting
    printf(ANSI_COLOR_GREEN "Available commands:\n" ANSI_COLOR_RESET);  // @print Displays the header in green
    printf(ANSI_COLOR_CYAN "-------------------------------------------------\n" ANSI_COLOR_RESET);  // @print Separator line in cyan
    printf(ANSI_COLOR_YELLOW "help              - Show this help message\n");  // @print Help command description in yellow
    printf("myip                - Connect to a peer\n");  // @print Connect command description
    printf("myport              - Displays the IP address\n");  // @print Connect command description
    printf("connect <ip> <port> - Display port\n");  // @print Connect command description
    printf("send <id> <message> - Send message to a connection\n");  // @print Send command description
    printf("list                - List all active connections\n");  // @print List command description
    printf("terminate <id>      - Terminate a connection\n");  // @print Terminate command description
    printf("exit                - Exit the application\n");  // @print Exit command description
    printf(ANSI_COLOR_CYAN "-------------------------------------------------\n" ANSI_COLOR_RESET);  // @print Separator line in cyan
    draw_border();                                      // @call Draws a closing border
}

// @function process_command
// @brief Processes user input commands
// @param command The command string entered by the user
// @details Parses the command and executes the corresponding action based on the input
void process_command(char *command) {
    char cmd[256];          // @var Buffer to store the command string
    char arg1[256];         // @var Buffer to store the first argument (e.g., IP or ID)
    char arg2[256];         // @var Buffer to store the second argument (e.g., port or message)
    int result;             // @var Stores the result of the connect_to_peer function

    if (sscanf(command, "%s %s %s", cmd, arg1, arg2) >= 1) {  // @call Parses the command string into cmd, arg1, and arg2
        if (strcmp(cmd, "help") == 0) {                     // @if Checks if the command is "help"
            show_help();                                    // @call Displays the help menu
        }else if (strcmp(cmd, "myip") == 0) {              // @if Checks if the command is "myip"
            char my_ip[INET_ADDRSTRLEN];                    // @var Buffer to store the local IP
            get_my_ip(my_ip);                               // @call Retrieves the local IP
            draw_border();                                  // @call Draws a border
            printf(ANSI_COLOR_GREEN "Local IP: " ANSI_COLOR_MAGENTA "%s\n" ANSI_COLOR_RESET, my_ip);  // @print Displays the IP
            draw_border();                                  // @call Draws a closing border
        } else if (strcmp(cmd, "myport") == 0) {            // @if Checks if the command is "myport"
            draw_border();                                  // @call Draws a border
            printf(ANSI_COLOR_GREEN "Listening on port: " ANSI_COLOR_MAGENTA "%d\n" ANSI_COLOR_RESET, listening_port);  // @print Displays the port
            draw_border();
        } else if (strcmp(cmd, "connect") == 0) {           // @if Checks if the command is "connect"
            int port = atoi(arg2);                          // @convert Converts the second argument to an integer port
            result = connect_to_peer(arg1, port);           // @call Initiates a connection to the specified peer
            if (result == 0) {                              // @if Checks if the connection was successful
                draw_border();                              // @call Draws a border
                printf(ANSI_COLOR_GREEN "Connected successfully\n" ANSI_COLOR_RESET);  // @print Success message in green
                draw_border();                              // @call Draws a closing border
            }
        } else if (strcmp(cmd, "send") == 0) {              // @if Checks if the command is "send"
            int id = atoi(arg1);                            // @convert Converts the first argument to an integer ID
            send_message(id, arg2);                         // @call Sends a message to the specified connection
        } else if (strcmp(cmd, "list") == 0) {              // @if Checks if the command is "list"
            list_connections();                             // @call Lists all active connections
        } else if (strcmp(cmd, "terminate") == 0) {         // @if Checks if the command is "terminate"
            int id = atoi(arg1);                            // @convert Converts the first argument to an integer ID
            terminate_connection(id);                       // @call Terminates the specified connection
        } else if (strcmp(cmd, "exit") == 0) {              // @if Checks if the command is "exit"
            exit(0);                                        // @call Exits the application with status 0
        } else {                                            // @else Handles unknown commands
            draw_border();                                  // @call Draws a border
            printf(ANSI_COLOR_RED "Error: Unknown command\n" ANSI_COLOR_RESET);  // @print Error message in red
            draw_border();                                  // @call Draws a closing border
            show_help();                                    // @call Displays the help menu
        }
    }
}