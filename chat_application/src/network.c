#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include "../inc/network.h"
#include "../inc/connection.h"
#include "../inc/utils.h"

// Định nghĩa các biến toàn cục tại đây
int server_socket;
int listening_port;
int server_ports[MAX_SERVERS];
int server_count = 0;
pthread_mutex_t server_mutex = PTHREAD_MUTEX_INITIALIZER;

void *handle_client(void *arg) {
    int *client_fd_ptr = (int *)arg;
    int client_fd = *client_fd_ptr;
    char buffer[MAX_MESSAGE];

    // Nhận port lắng nghe từ client ngay sau khi kết nối
    int bytes_received = recv(client_fd, buffer, MAX_MESSAGE - 1, 0);
    if (bytes_received <= 0) {
        pthread_mutex_lock(&conn_mutex);
        for (int i = 0; i < connection_count; i++) {
            if (connections[i].socket_fd == client_fd) {
                connections[i].active = 0;
                close(client_fd);
                for (int j = i; j < connection_count - 1; j++) {
                    connections[j] = connections[j + 1];
                    connections[j].id = j;
                }
                connection_count--;
                break;
            }
        }
        pthread_mutex_unlock(&conn_mutex);
        free(client_fd_ptr);
        return NULL;
    }
    buffer[bytes_received] = '\0';
    int client_listening_port = atoi(buffer);

    // Cập nhật port lắng nghe của client
    char client_ip[INET_ADDRSTRLEN];
    pthread_mutex_lock(&conn_mutex);
    for (int i = 0; i < connection_count; i++) {
        if (connections[i].socket_fd == client_fd) {
            connections[i].local_port = client_listening_port;
            strcpy(client_ip, connections[i].ip);
            break;
        }
    }
    pthread_mutex_unlock(&conn_mutex);

    // Khởi tạo kết nối ngược từ server đến client
    initiate_reverse_connection(client_ip, client_listening_port, listening_port);

    // Thông báo các server khác để kết nối đến client
    notify_other_servers(client_ip, client_listening_port);

    while (1) {
        int bytes_received = recv(client_fd, buffer, MAX_MESSAGE - 1, 0);
        if (bytes_received <= 0) {
            break;
        }
        buffer[bytes_received] = '\0';
        
        pthread_mutex_lock(&conn_mutex);
        for (int i = 0; i < connection_count; i++) {
            if (connections[i].socket_fd == client_fd && connections[i].active) {
                draw_border();
                printf(ANSI_COLOR_GREEN "Received from " ANSI_COLOR_MAGENTA "%s:%d" 
                       ANSI_COLOR_RESET " - %s\n", 
                       connections[i].ip, connections[i].local_port, buffer);
                draw_border();
                break;
            }
        }
        pthread_mutex_unlock(&conn_mutex);
    }
    
    pthread_mutex_lock(&conn_mutex);
    for (int i = 0; i < connection_count; i++) {
        if (connections[i].socket_fd == client_fd) {
            connections[i].active = 0;
            close(client_fd);
            for (int j = i; j < connection_count - 1; j++) {
                connections[j] = connections[j + 1];
                connections[j].id = j;
            }
            connection_count--;
            break;
        }
    }
    pthread_mutex_unlock(&conn_mutex);
    free(client_fd_ptr);
    return NULL;
}

void *server_thread(void *arg) {
    int port = *(int *)arg;
    listening_port = port;
    struct sockaddr_in server_addr;
    
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        exit(1);
    }
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);
    
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(1);
    }
    
    if (listen(server_socket, 5) < 0) {
        perror("Listen failed");
        exit(1);
    }
    
    // Thêm port vào danh sách server
    pthread_mutex_lock(&server_mutex);
    if (server_count < MAX_SERVERS) {
        server_ports[server_count++] = port;
    }
    pthread_mutex_unlock(&server_mutex);
    
    draw_border();
    printf(ANSI_COLOR_GREEN "Application is listening on port: " 
           ANSI_COLOR_MAGENTA "%d\n" ANSI_COLOR_RESET, port);
    draw_border();
    
    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int *client_fd = malloc(sizeof(int));
        if (client_fd == NULL) {
            perror("Memory allocation failed");
            continue;
        }
        
        *client_fd = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (*client_fd < 0) {
            perror("Accept failed");
            free(client_fd);
            continue;
        }
        
        pthread_mutex_lock(&conn_mutex);
        if (connection_count < MAX_CONNECTIONS) {
            connections[connection_count].id = connection_count;
            connections[connection_count].socket_fd = *client_fd;
            inet_ntop(AF_INET, &client_addr.sin_addr, 
                     connections[connection_count].ip, INET_ADDRSTRLEN);
            connections[connection_count].local_port = 0; // Sẽ được cập nhật sau
            connections[connection_count].remote_port = port;
            connections[connection_count].active = 1;
            
            draw_border();
            printf(ANSI_COLOR_GREEN "Accepted a new connection from address: " 
                   ANSI_COLOR_MAGENTA "%s" ANSI_COLOR_RESET ", at port: " 
                   ANSI_COLOR_MAGENTA "%d\n" ANSI_COLOR_RESET, 
                   connections[connection_count].ip, ntohs(client_addr.sin_port));
            draw_border();
            
            connection_count++;
            
            pthread_t thread;
            if (pthread_create(&thread, NULL, handle_client, client_fd) != 0) {
                perror("Thread creation failed");
                close(*client_fd);
                free(client_fd);
            } else {
                pthread_detach(thread);
            }
        } else {
            close(*client_fd);
            free(client_fd);
            draw_border();
            printf(ANSI_COLOR_RED "Error: Maximum connections reached\n" ANSI_COLOR_RESET);
            draw_border();
        }
        pthread_mutex_unlock(&conn_mutex);
    }
    return NULL;
}

int initiate_reverse_connection(char *dest_ip, int dest_port, int local_port) {
    // Kiểm tra xem kết nối đã tồn tại chưa
    pthread_mutex_lock(&conn_mutex);
    for (int i = 0; i < connection_count; i++) {
        if (strcmp(connections[i].ip, dest_ip) == 0 && 
            connections[i].remote_port == dest_port && 
            connections[i].local_port == local_port && 
            connections[i].active) {
            pthread_mutex_unlock(&conn_mutex);
            return 0; // Kết nối đã tồn tại, không cần tạo mới
        }
    }
    pthread_mutex_unlock(&conn_mutex);

    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        draw_border();
        printf(ANSI_COLOR_RED "Error: Socket creation failed for reverse connection\n" ANSI_COLOR_RESET);
        draw_border();
        return -1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(dest_port);
    if (inet_pton(AF_INET, dest_ip, &server_addr.sin_addr) <= 0) {
        draw_border();
        printf(ANSI_COLOR_RED "Error: Invalid IP address for reverse connection\n" ANSI_COLOR_RESET);
        draw_border();
        close(client_fd);
        return -1;
    }

    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        draw_border();
        printf(ANSI_COLOR_RED "Error: Reverse connection failed\n" ANSI_COLOR_RESET);
        draw_border();
        close(client_fd);
        return -1;
    }

    // Gửi port lắng nghe của server đến client
    char port_buffer[10];
    snprintf(port_buffer, sizeof(port_buffer), "%d", local_port);
    if (send(client_fd, port_buffer, strlen(port_buffer), 0) < 0) {
        draw_border();
        printf(ANSI_COLOR_RED "Error: Failed to send listening port for reverse connection\n" ANSI_COLOR_RESET);
        draw_border();
        close(client_fd);
        return -1;
    }

    pthread_mutex_lock(&conn_mutex);
    if (connection_count < MAX_CONNECTIONS) {
        connections[connection_count].id = connection_count;
        connections[connection_count].socket_fd = client_fd;
        strcpy(connections[connection_count].ip, dest_ip);
        connections[connection_count].local_port = local_port;
        connections[connection_count].remote_port = dest_port;
        connections[connection_count].active = 1;

        draw_border();
        printf(ANSI_COLOR_GREEN "Reverse connection established to " ANSI_COLOR_MAGENTA "%s:%d\n" ANSI_COLOR_RESET, 
               dest_ip, dest_port);
        draw_border();

        connection_count++;

        pthread_t thread;
        int *client_fd_ptr = malloc(sizeof(int));
        *client_fd_ptr = client_fd;
        if (pthread_create(&thread, NULL, handle_client, client_fd_ptr) != 0) {
            perror("Thread creation failed for reverse connection");
            close(client_fd);
            free(client_fd_ptr);
        } else {
            pthread_detach(thread);
        }
    } else {
        close(client_fd);
        draw_border();
        printf(ANSI_COLOR_RED "Error: Maximum connections reached for reverse connection\n" ANSI_COLOR_RESET);
        draw_border();
    }
    pthread_mutex_unlock(&conn_mutex);
    return 0;
}

void notify_other_servers(char *client_ip, int client_port) {
    pthread_mutex_lock(&server_mutex);
    for (int i = 0; i < server_count; i++) {
        if (server_ports[i] != listening_port) {
            initiate_reverse_connection(client_ip, client_port, server_ports[i]);
        }
    }
    pthread_mutex_unlock(&server_mutex);
}

int connect_to_peer(char *dest_ip, int port) {
    char my_ip[INET_ADDRSTRLEN];
    get_my_ip(my_ip);

    if (strcmp(dest_ip, my_ip) == 0 && port == listening_port) {
        draw_border();
        printf(ANSI_COLOR_RED "Error: Cannot connect to self\n" ANSI_COLOR_RESET);
        draw_border();
        return -1;
    }

    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        draw_border();
        printf(ANSI_COLOR_RED "Error: Socket creation failed\n" ANSI_COLOR_RESET);
        draw_border();
        return -1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, dest_ip, &server_addr.sin_addr) <= 0) {
        draw_border();
        printf(ANSI_COLOR_RED "Error: Invalid IP address\n" ANSI_COLOR_RESET);
        draw_border();
        close(client_fd);
        return -1;
    }

    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        draw_border();
        printf(ANSI_COLOR_RED "Error: Connection failed\n" ANSI_COLOR_RESET);
        draw_border();
        close(client_fd);
        return -1;
    }

    // Gửi port lắng nghe của client đến server
    char port_buffer[10];
    snprintf(port_buffer, sizeof(port_buffer), "%d", listening_port);
    if (send(client_fd, port_buffer, strlen(port_buffer), 0) < 0) {
        draw_border();
        printf(ANSI_COLOR_RED "Error: Failed to send listening port\n" ANSI_COLOR_RESET);
        draw_border();
        close(client_fd);
        return -1;
    }

    pthread_mutex_lock(&conn_mutex);
    if (connection_count < MAX_CONNECTIONS) {
        connections[connection_count].id = connection_count;
        connections[connection_count].socket_fd = client_fd;
        strcpy(connections[connection_count].ip, dest_ip);
        connections[connection_count].local_port = listening_port;
        connections[connection_count].remote_port = port;
        connections[connection_count].active = 1;

        draw_border();
        printf(ANSI_COLOR_GREEN "Connected successfully. Ready for data transmission\n" 
               ANSI_COLOR_RESET);
        printf(ANSI_COLOR_GREEN "Connect " ANSI_COLOR_MAGENTA "%s" 
               ANSI_COLOR_GREEN ":%d to " ANSI_COLOR_MAGENTA "%s" 
               ANSI_COLOR_GREEN ":%d\n" ANSI_COLOR_RESET, 
               my_ip, listening_port, dest_ip, port);
        draw_border();

        connection_count++;

        pthread_t thread;
        int *client_fd_ptr = malloc(sizeof(int));
        *client_fd_ptr = client_fd;
        if (pthread_create(&thread, NULL, handle_client, client_fd_ptr) != 0) {
            perror("Thread creation failed");
            close(client_fd);
            free(client_fd_ptr);
        } else {
            pthread_detach(thread);
        }
    } else {
        close(client_fd);
        draw_border();
        printf(ANSI_COLOR_RED "Error: Maximum connections reached\n" ANSI_COLOR_RESET);
        draw_border();
    }
    pthread_mutex_unlock(&conn_mutex);

    return 0;
}

void send_message(int id, char *message) {
    if (strlen(message) == 0) {
        draw_border();
        printf(ANSI_COLOR_RED "Error: Message cannot be empty\n" ANSI_COLOR_RESET);
        draw_border();
        return;
    }

    if (strlen(message) > MAX_MESSAGE - 1) {
        draw_border();
        printf(ANSI_COLOR_RED "Error: Message exceeds 100 characters\n" ANSI_COLOR_RESET);
        draw_border();
        return;
    }

    pthread_mutex_lock(&conn_mutex);
    int valid_id = 0;
    for (int i = 0; i < connection_count; i++) {
        if (connections[i].id == id && connections[i].active) {
            valid_id = 1;
            if (send(connections[i].socket_fd, message, strlen(message), 0) < 0) {
                draw_border();
                printf(ANSI_COLOR_RED "Error: Failed to send message\n" ANSI_COLOR_RESET);
                draw_border();
            } else {
                draw_border();
                printf(ANSI_COLOR_GREEN "Message sent to " ANSI_COLOR_MAGENTA "%s:%d\n" 
                       ANSI_COLOR_RESET, connections[i].ip, connections[i].remote_port);
                draw_border();
            }
            break;
        }
    }
    if (!valid_id) {
        draw_border();
        printf(ANSI_COLOR_RED "Error: Invalid connection ID\n" ANSI_COLOR_RESET);
        draw_border();
    }
    pthread_mutex_unlock(&conn_mutex);
}