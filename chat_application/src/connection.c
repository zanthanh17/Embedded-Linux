#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include "../inc/connection.h"
#include "../inc/utils.h"

// @var Global array to store connection details
Connection connections[MAX_CONNECTIONS];
// @var Global counter for the number of active connections
int connection_count = 0;
// @var Global mutex to synchronize access to the connections array
pthread_mutex_t conn_mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * @function init_connections
 * @brief Initializes the connection list by resetting the connection count to zero
 * @details Also initializes the mutex for thread safety
 */
void init_connections(void) {
    connection_count = 0;
    pthread_mutex_init(&conn_mutex, NULL); // Initialize mutex
}

/**
 * @function terminate_connection
 * @brief Terminates a connection identified by its ID
 * @param id The ID of the connection to terminate
 * @details Closes the socket, updates the connections array, and decrements the count
 */
void terminate_connection(int id) {
    pthread_mutex_lock(&conn_mutex); // Lock mutex to ensure thread safety
    if (id >= 0 && id < connection_count && connections[id].active) {
        close(connections[id].socket_fd); // Close the socket to free resources
        connections[id].active = 0; // Mark connection as inactive
        for (int j = id; j < connection_count - 1; j++) {
            connections[j] = connections[j + 1]; // Shift connections down
            connections[j].id = j; // Update IDs
        }
        connection_count--; // Decrease the connection count
        draw_border();
        printf(ANSI_COLOR_GREEN "Connection %d terminated\n" ANSI_COLOR_RESET, id);
        draw_border();
    } else {
        draw_border();
        printf(ANSI_COLOR_RED "Error: Invalid connection ID %d\n" ANSI_COLOR_RESET, id);
        draw_border();
    }
    pthread_mutex_unlock(&conn_mutex); // Unlock mutex
}

/**
 * @function list_connections
 * @brief Displays a formatted list of all active connections
 * @details Locks the mutex, iterates through the connections array, and prints details
 */
void list_connections(void) {
    draw_border();
    printf(ANSI_COLOR_GREEN "Connection List:\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_CYAN "-------------------------------------------------\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_YELLOW "%-5s %-20s %-10s %-10s %-10s\n" ANSI_COLOR_RESET, 
           "ID", "IP Address", "Local Port", "Remote Port", "Status");
    printf(ANSI_COLOR_CYAN "-------------------------------------------------\n" ANSI_COLOR_RESET);
    
    pthread_mutex_lock(&conn_mutex); // Lock mutex for safe access
    for (int i = 0; i < connection_count; i++) {
        if (connections[i].active) {
            printf("%-5d %-20s %-10d %-10d %-10s\n", 
                   connections[i].id, 
                   connections[i].ip, 
                   connections[i].local_port,
                   connections[i].remote_port,
                   ANSI_COLOR_GREEN "Active" ANSI_COLOR_RESET);
        }
    }
    pthread_mutex_unlock(&conn_mutex); // Unlock mutex
    draw_border();
}