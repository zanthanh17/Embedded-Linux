#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include "../inc/connection.h"
#include "../inc/utils.h"

// Định nghĩa các biến toàn cục tại đây
Connection connections[MAX_CONNECTIONS];
int connection_count = 0;
pthread_mutex_t conn_mutex = PTHREAD_MUTEX_INITIALIZER;

void init_connections() {
    connection_count = 0;
}

void terminate_connection(int id) {
    pthread_mutex_lock(&conn_mutex);
    if (id >= 0 && id < connection_count && connections[id].active) {
        close(connections[id].socket_fd);
        connections[id].active = 0;
        for (int j = id; j < connection_count - 1; j++) {
            connections[j] = connections[j + 1];
            connections[j].id = j;
        }
        connection_count--;
        draw_border();
        printf(ANSI_COLOR_GREEN "Connection %d terminated\n" ANSI_COLOR_RESET, id);
        draw_border();
    } else {
        draw_border();
        printf(ANSI_COLOR_RED "Error: Invalid connection ID\n" ANSI_COLOR_RESET);
        draw_border();
    }
    pthread_mutex_unlock(&conn_mutex);
}

void list_connections() {
    draw_border();
    printf(ANSI_COLOR_GREEN "Connection List:\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_CYAN "-------------------------------------------------\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_YELLOW "%-5s %-20s %-10s %-10s %-10s\n" ANSI_COLOR_RESET, 
           "ID", "IP Address", "Local Port", "Remote Port", "Status");
    printf(ANSI_COLOR_CYAN "-------------------------------------------------\n" ANSI_COLOR_RESET);
    
    pthread_mutex_lock(&conn_mutex);
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
    pthread_mutex_unlock(&conn_mutex);
    draw_border();
}