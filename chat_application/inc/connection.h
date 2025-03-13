#ifndef CONNECTION_H
#define CONNECTION_H

#include <arpa/inet.h>

#define MAX_CONNECTIONS 10
#define MAX_MESSAGE 100
#define MAX_CLIENTS 100

typedef struct {
    int id;
    int socket_fd;
    char ip[INET_ADDRSTRLEN];
    int local_port;  // Port lắng nghe của client
    int remote_port; // Port đích mà client kết nối đến
    int active;
} Connection;

// Khai báo (extern) các biến toàn cục
extern Connection connections[MAX_CONNECTIONS];
extern int connection_count;
extern pthread_mutex_t conn_mutex;

// Khai báo prototype của các hàm
void init_connections();
void terminate_connection(int id);
void list_connections();

#endif