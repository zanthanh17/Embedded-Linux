#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include "../inc/network.h"      // @include Header file for network-related declarations
#include "../inc/connection.h"  // @include Header file for connection structure and mutex
#include "../inc/utils.h"       // @include Header file for utility functions like draw_border

// @var Global socket for the server
int server_socket;
// @var Global port on which the server is listening
int listening_port;
// @var Global array to store server ports
int server_ports[MAX_SERVERS];
// @var Global counter for the number of active servers
int server_count = 0;
// @var Global mutex to synchronize access to the server ports array
pthread_mutex_t server_mutex = PTHREAD_MUTEX_INITIALIZER;

// @function handle_client
// @brief Handles communication with a connected client
// @param arg Pointer to the client file descriptor
// @details Manages receiving messages, updating client port, initiating reverse connections, and cleaning up
void *handle_client(void *arg) {
    int *client_fd_ptr = (int *)arg;            // @var Pointer to the client file descriptor
    int client_fd = *client_fd_ptr;             // @var Client file descriptor for communication
    char buffer[MAX_MESSAGE];                   // @var Buffer to store received messages

    // @call Receives the listening port from the client upon connection
    int bytes_received = recv(client_fd, buffer, MAX_MESSAGE - 1, 0);
    if (bytes_received <= 0) {                  // @if Checks for connection closure or error
        pthread_mutex_lock(&conn_mutex);        // @call Locks the connection mutex
        for (int i = 0; i < connection_count; i++) {  // @loop Iterates through connections
            if (connections[i].socket_fd == client_fd) {  // @if Matches the client file descriptor
                connections[i].active = 0;          // @set Marks connection as inactive
                close(client_fd);                   // @call Closes the client socket
                for (int j = i; j < connection_count - 1; j++) {  // @loop Shifts remaining connections
                    connections[j] = connections[j + 1];  // @assign Moves connection data
                    connections[j].id = j;          // @set Updates connection ID
                }
                connection_count--;                 // @decrement Decrements connection count
                break;                              // @break Exits the loop
            }
        }
        pthread_mutex_unlock(&conn_mutex);      // @call Unlocks the connection mutex
        free(client_fd_ptr);                    // @call Frees the allocated memory
        return NULL;                            // @return Terminates the thread
    }
    buffer[bytes_received] = '\0';              // @assign Null-terminates the received buffer
    int client_listening_port = atoi(buffer);   // @convert Converts buffer to client listening port

    // @call Updates the listening port of the client in the connection list
    char client_ip[INET_ADDRSTRLEN];            // @var Buffer to store client IP address
    pthread_mutex_lock(&conn_mutex);            // @call Locks the connection mutex
    for (int i = 0; i < connection_count; i++) {  // @loop Iterates through connections
        if (connections[i].socket_fd == client_fd) {  // @if Matches the client file descriptor
            connections[i].local_port = client_listening_port;  // @set Updates client local port
            strcpy(client_ip, connections[i].ip);    // @copy Copies client IP to local variable
            break;                                  // @break Exits the loop
        }
    }
    pthread_mutex_unlock(&conn_mutex);          // @call Unlocks the connection mutex

    // @call Initiates a reverse connection from server to client
    initiate_reverse_connection(client_ip, client_listening_port, listening_port);

    // @call Notifies other servers to connect to the new client
    notify_other_servers(client_ip, client_listening_port);

    while (1) {                                 // @loop Continuous reception loop
        bytes_received = recv(client_fd, buffer, MAX_MESSAGE - 1, 0);  // @call Receives data
        if (bytes_received <= 0) {              // @if Checks for connection closure or error
            break;                              // @break Exits the loop
        }
        buffer[bytes_received] = '\0';          // @assign Null-terminates the received buffer
        
        pthread_mutex_lock(&conn_mutex);        // @call Locks the connection mutex
        for (int i = 0; i < connection_count; i++) {  // @loop Iterates through connections
            if (connections[i].socket_fd == client_fd && connections[i].active) {  // @if Matches active client
                draw_border();                  // @call Draws a border
                printf(ANSI_COLOR_GREEN "Received from " ANSI_COLOR_MAGENTA "%s:%d" 
                       ANSI_COLOR_RESET " - %s\n", 
                       connections[i].ip, connections[i].local_port, buffer);  // @print Displays received message
                draw_border();                  // @call Draws a closing border
                break;                          // @break Exits the loop
            }
        }
        pthread_mutex_unlock(&conn_mutex);      // @call Unlocks the connection mutex
    }
    
    pthread_mutex_lock(&conn_mutex);            // @call Locks the connection mutex
    for (int i = 0; i < connection_count; i++) {  // @loop Iterates through connections
        if (connections[i].socket_fd == client_fd) {  // @if Matches the client file descriptor
            connections[i].active = 0;          // @set Marks connection as inactive
            close(client_fd);                   // @call Closes the client socket
            for (int j = i; j < connection_count - 1; j++) {  // @loop Shifts remaining connections
                connections[j] = connections[j + 1];  // @assign Moves connection data
                connections[j].id = j;          // @set Updates connection ID
            }
            connection_count--;                 // @decrement Decrements connection count
            break;                              // @break Exits the loop
        }
    }
    pthread_mutex_unlock(&conn_mutex);          // @call Unlocks the connection mutex
    free(client_fd_ptr);                        // @call Frees the allocated memory
    return NULL;                                // @return Terminates the thread
}

// @function server_thread
// @brief Main thread for the server to accept incoming connections
// @param arg Pointer to the port number
// @details Listens on the specified port and spawns threads for each client
void *server_thread(void *arg) {
    int port = *(int *)arg;                     // @var Port number passed as argument
    listening_port = port;                      // @assign Sets the global listening port
    struct sockaddr_in server_addr;             // @var Structure for server address

    server_socket = socket(AF_INET, SOCK_STREAM, 0);  // @call Creates a TCP socket
    if (server_socket < 0) {                    // @if Checks for socket creation failure
        perror("Socket creation failed");       // @call Prints error message
        exit(1);                                // @call Exits with failure status
    }
    
    server_addr.sin_family = AF_INET;           // @set Sets address family to IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY;   // @set Allows connection from any interface
    server_addr.sin_port = htons(port);         // @convert Converts port to network byte order

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {  // @call Binds socket to address
        perror("Bind failed");                  // @call Prints error message
        exit(1);                                // @call Exits with failure status
    }
    
    if (listen(server_socket, 5) < 0) {         // @call Sets socket to listen mode with backlog of 5
        perror("Listen failed");                // @call Prints error message
        exit(1);                                // @call Exits with failure status
    }
    
    // @call Adds the port to the server list with mutex protection
    pthread_mutex_lock(&server_mutex);          // @call Locks the server mutex
    if (server_count < MAX_SERVERS) {           // @if Checks if maximum servers not exceeded
        server_ports[server_count++] = port;    // @assign Adds port to array and increments count
    }
    pthread_mutex_unlock(&server_mutex);        // @call Unlocks the server mutex
    
    draw_border();                              // @call Draws a border
    printf(ANSI_COLOR_GREEN "Application is listening on port: " 
           ANSI_COLOR_MAGENTA "%d\n" ANSI_COLOR_RESET, port);  // @print Displays listening port
    draw_border();                              // @call Draws a closing border
    
    while (1) {                                 // @loop Continuous acceptance loop
        struct sockaddr_in client_addr;         // @var Structure for client address
        socklen_t client_len = sizeof(client_addr);  // @var Size of client address structure
        int *client_fd = malloc(sizeof(int));   // @var Pointer to client file descriptor
        if (client_fd == NULL) {                // @if Checks for memory allocation failure
            perror("Memory allocation failed"); // @call Prints error message
            continue;                           // @continue Skips to next iteration
        }
        
        *client_fd = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);  // @call Accepts a new connection
        if (*client_fd < 0) {                   // @if Checks for accept failure
            perror("Accept failed");            // @call Prints error message
            free(client_fd);                    // @call Frees allocated memory
            continue;                           // @continue Skips to next iteration
        }
        
        pthread_mutex_lock(&conn_mutex);        // @call Locks the connection mutex
        if (connection_count < MAX_CONNECTIONS) {  // @if Checks if maximum connections not exceeded
            connections[connection_count].id = connection_count;  // @set Sets connection ID
            connections[connection_count].socket_fd = *client_fd;  // @set Sets client file descriptor
            inet_ntop(AF_INET, &client_addr.sin_addr, 
                     connections[connection_count].ip, INET_ADDRSTRLEN);  // @call Converts IP to string
            connections[connection_count].local_port = 0;  // @set Initializes local port (to be updated later)
            connections[connection_count].remote_port = port;  // @set Sets remote port
            connections[connection_count].active = 1;  // @set Marks connection as active
            
            draw_border();                      // @call Draws a border
            printf(ANSI_COLOR_GREEN "Accepted a new connection from address: " 
                   ANSI_COLOR_MAGENTA "%s" ANSI_COLOR_RESET ", at port: " 
                   ANSI_COLOR_MAGENTA "%d\n" ANSI_COLOR_RESET, 
                   connections[connection_count].ip, ntohs(client_addr.sin_port));  // @print Displays connection info
            draw_border();                      // @call Draws a closing border
            
            connection_count++;                 // @increment Increments connection count
            
            pthread_t thread;                   // @var Thread for handling client
            if (pthread_create(&thread, NULL, handle_client, client_fd) != 0) {  // @call Creates client thread
                perror("Thread creation failed");  // @call Prints error message
                close(*client_fd);              // @call Closes client socket
                free(client_fd);                // @call Frees allocated memory
            } else {
                pthread_detach(thread);          // @call Detaches the thread
            }
        } else {                                // @else Handles maximum connections reached
            close(*client_fd);                  // @call Closes client socket
            free(client_fd);                    // @call Frees allocated memory
            draw_border();                      // @call Draws a border
            printf(ANSI_COLOR_RED "Error: Maximum connections reached\n" ANSI_COLOR_RESET);  // @print Error message
            draw_border();                      // @call Draws a closing border
        }
        pthread_mutex_unlock(&conn_mutex);      // @call Unlocks the connection mutex
    }
    return NULL;                                // @return Terminates the thread (unreachable)
}

// @function initiate_reverse_connection
// @brief Initiates a reverse connection from the server to a client
// @param dest_ip Destination IP address
// @param dest_port Destination port
// @param local_port Local port to send
// @return 0 on success, -1 on failure
int initiate_reverse_connection(char *dest_ip, int dest_port, int local_port) {
    // @call Checks if the connection already exists
    pthread_mutex_lock(&conn_mutex);            // @call Locks the connection mutex
    for (int i = 0; i < connection_count; i++) {  // @loop Iterates through connections
        if (strcmp(connections[i].ip, dest_ip) == 0 && 
            connections[i].remote_port == dest_port && 
            connections[i].local_port == local_port && 
            connections[i].active) {  // @if Checks for existing active connection
            pthread_mutex_unlock(&conn_mutex);  // @call Unlocks the connection mutex
            return 0;                           // @return Indicates connection exists
        }
    }
    pthread_mutex_unlock(&conn_mutex);          // @call Unlocks the connection mutex

    int client_fd = socket(AF_INET, SOCK_STREAM, 0);  // @call Creates a TCP socket
    if (client_fd < 0) {                        // @if Checks for socket creation failure
        draw_border();                          // @call Draws a border
        printf(ANSI_COLOR_RED "Error: Socket creation failed for reverse connection\n" ANSI_COLOR_RESET);  // @print Error message
        draw_border();                          // @call Draws a closing border
        return -1;                              // @return Indicates failure
    }

    struct sockaddr_in server_addr;             // @var Structure for destination address
    server_addr.sin_family = AF_INET;           // @set Sets address family to IPv4
    server_addr.sin_port = htons(dest_port);    // @convert Converts port to network byte order
    if (inet_pton(AF_INET, dest_ip, &server_addr.sin_addr) <= 0) {  // @call Converts IP to binary
        draw_border();                          // @call Draws a border
        printf(ANSI_COLOR_RED "Error: Invalid IP address for reverse connection\n" ANSI_COLOR_RESET);  // @print Error message
        draw_border();                          // @call Draws a closing border
        close(client_fd);                       // @call Closes the socket
        return -1;                              // @return Indicates failure
    }

    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {  // @call Initiates connection
        draw_border();                          // @call Draws a border
        printf(ANSI_COLOR_RED "Error: Reverse connection failed\n" ANSI_COLOR_RESET);  // @print Error message
        draw_border();                          // @call Draws a closing border
        close(client_fd);                       // @call Closes the socket
        return -1;                              // @return Indicates failure
    }

    // @call Sends the local listening port to the client
    char port_buffer[10];                       // @var Buffer for port string
    snprintf(port_buffer, sizeof(port_buffer), "%d", local_port);  // @call Formats port as string
    if (send(client_fd, port_buffer, strlen(port_buffer), 0) < 0) {  // @call Sends port to client
        draw_border();                          // @call Draws a border
        printf(ANSI_COLOR_RED "Error: Failed to send listening port for reverse connection\n" ANSI_COLOR_RESET);  // @print Error message
        draw_border();                          // @call Draws a closing border
        close(client_fd);                       // @call Closes the socket
        return -1;                              // @return Indicates failure
    }

    pthread_mutex_lock(&conn_mutex);            // @call Locks the connection mutex
    if (connection_count < MAX_CONNECTIONS) {    // @if Checks if maximum connections not exceeded
        connections[connection_count].id = connection_count;  // @set Sets connection ID
        connections[connection_count].socket_fd = client_fd;  // @set Sets client file descriptor
        strcpy(connections[connection_count].ip, dest_ip);  // @copy Copies destination IP
        connections[connection_count].local_port = local_port;  // @set Sets local port
        connections[connection_count].remote_port = dest_port;  // @set Sets remote port
        connections[connection_count].active = 1;  // @set Marks connection as active

        draw_border();                          // @call Draws a border
        printf(ANSI_COLOR_GREEN "Reverse connection established to " ANSI_COLOR_MAGENTA "%s:%d\n" ANSI_COLOR_RESET, 
               dest_ip, dest_port);  // @print Displays success message
        draw_border();                          // @call Draws a closing border

        connection_count++;                     // @increment Increments connection count

        pthread_t thread;                       // @var Thread for handling client
        int *client_fd_ptr = malloc(sizeof(int));  // @var Pointer to client file descriptor
        *client_fd_ptr = client_fd;             // @assign Assigns client file descriptor
        if (pthread_create(&thread, NULL, handle_client, client_fd_ptr) != 0) {  // @call Creates client thread
            perror("Thread creation failed for reverse connection");  // @call Prints error message
            close(client_fd);                   // @call Closes the socket
            free(client_fd_ptr);                // @call Frees allocated memory
        } else {
            pthread_detach(thread);              // @call Detaches the thread
        }
    } else {                                    // @else Handles maximum connections reached
        close(client_fd);                       // @call Closes the socket
        draw_border();                          // @call Draws a border
        printf(ANSI_COLOR_RED "Error: Maximum connections reached for reverse connection\n" ANSI_COLOR_RESET);  // @print Error message
        draw_border();                          // @call Draws a closing border
    }
    pthread_mutex_unlock(&conn_mutex);          // @call Unlocks the connection mutex
    return 0;                                   // @return Indicates success
}

// @function notify_other_servers
// @brief Notifies other servers to connect to a new client
// @param client_ip IP address of the new client
// @param client_port Port of the new client
// @details Initiates reverse connections from other servers
void notify_other_servers(char *client_ip, int client_port) {
    pthread_mutex_lock(&server_mutex);          // @call Locks the server mutex
    for (int i = 0; i < server_count; i++) {    // @loop Iterates through server ports
        if (server_ports[i] != listening_port) {  // @if Skips the current server
            initiate_reverse_connection(client_ip, client_port, server_ports[i]);  // @call Initiates reverse connection
        }
    }
    pthread_mutex_unlock(&server_mutex);        // @call Unlocks the server mutex
}

// @function connect_to_peer
// @brief Initiates a connection to a peer
// @param dest_ip Destination IP address
// @param port Destination port
// @return 0 on success, -1 on failure
int connect_to_peer(char *dest_ip, int port) {
    char my_ip[INET_ADDRSTRLEN];                // @var Buffer to store local IP address
    get_my_ip(my_ip);                           // @call Retrieves the local IP address

    if (strcmp(dest_ip, my_ip) == 0 && port == listening_port) {  // @if Checks for self-connection
        draw_border();                          // @call Draws a border
        printf(ANSI_COLOR_RED "Error: Cannot connect to self\n" ANSI_COLOR_RESET);  // @print Error message
        draw_border();                          // @call Draws a closing border
        return -1;                              // @return Indicates failure
    }

    int client_fd = socket(AF_INET, SOCK_STREAM, 0);  // @call Creates a TCP socket
    if (client_fd < 0) {                        // @if Checks for socket creation failure
        draw_border();                          // @call Draws a border
        printf(ANSI_COLOR_RED "Error: Socket creation failed\n" ANSI_COLOR_RESET);  // @print Error message
        draw_border();                          // @call Draws a closing border
        return -1;                              // @return Indicates failure
    }

    struct sockaddr_in server_addr;             // @var Structure for destination address
    server_addr.sin_family = AF_INET;           // @set Sets address family to IPv4
    server_addr.sin_port = htons(port);         // @convert Converts port to network byte order
    if (inet_pton(AF_INET, dest_ip, &server_addr.sin_addr) <= 0) {  // @call Converts IP to binary
        draw_border();                          // @call Draws a border
        printf(ANSI_COLOR_RED "Error: Invalid IP address\n" ANSI_COLOR_RESET);  // @print Error message
        draw_border();                          // @call Draws a closing border
        close(client_fd);                       // @call Closes the socket
        return -1;                              // @return Indicates failure
    }

    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {  // @call Initiates connection
        draw_border();                          // @call Draws a border
        printf(ANSI_COLOR_RED "Error: Connection failed\n" ANSI_COLOR_RESET);  // @print Error message
        draw_border();                          // @call Draws a closing border
        close(client_fd);                       // @call Closes the socket
        return -1;                              // @return Indicates failure
    }

    // @call Sends the local listening port to the server
    char port_buffer[10];                       // @var Buffer for port string
    snprintf(port_buffer, sizeof(port_buffer), "%d", listening_port);  // @call Formats port as string
    if (send(client_fd, port_buffer, strlen(port_buffer), 0) < 0) {  // @call Sends port to server
        draw_border();                          // @call Draws a border
        printf(ANSI_COLOR_RED "Error: Failed to send listening port\n" ANSI_COLOR_RESET);  // @print Error message
        draw_border();                          // @call Draws a closing border
        close(client_fd);                       // @call Closes the socket
        return -1;                              // @return Indicates failure
    }

    pthread_mutex_lock(&conn_mutex);            // @call Locks the connection mutex
    if (connection_count < MAX_CONNECTIONS) {    // @if Checks if maximum connections not exceeded
        connections[connection_count].id = connection_count;  // @set Sets connection ID
        connections[connection_count].socket_fd = client_fd;  // @set Sets client file descriptor
        strcpy(connections[connection_count].ip, dest_ip);  // @copy Copies destination IP
        connections[connection_count].local_port = listening_port;  // @set Sets local port
        connections[connection_count].remote_port = port;  // @set Sets remote port
        connections[connection_count].active = 1;  // @set Marks connection as active

        draw_border();                          // @call Draws a border
        printf(ANSI_COLOR_GREEN "Connected successfully. Ready for data transmission\n" 
               ANSI_COLOR_RESET);  // @print Success message
        printf(ANSI_COLOR_GREEN "Connect " ANSI_COLOR_MAGENTA "%s" 
               ANSI_COLOR_GREEN ":%d to " ANSI_COLOR_MAGENTA "%s" 
               ANSI_COLOR_GREEN ":%d\n" ANSI_COLOR_RESET, 
               my_ip, listening_port, dest_ip, port);  // @print Connection details
        draw_border();                          // @call Draws a closing border

        connection_count++;                     // @increment Increments connection count

        pthread_t thread;                       // @var Thread for handling client
        int *client_fd_ptr = malloc(sizeof(int));  // @var Pointer to client file descriptor
        *client_fd_ptr = client_fd;             // @assign Assigns client file descriptor
        if (pthread_create(&thread, NULL, handle_client, client_fd_ptr) != 0) {  // @call Creates client thread
            perror("Thread creation failed");   // @call Prints error message
            close(client_fd);                   // @call Closes the socket
            free(client_fd_ptr);                // @call Frees allocated memory
        } else {
            pthread_detach(thread);              // @call Detaches the thread
        }
    } else {                                    // @else Handles maximum connections reached
        close(client_fd);                       // @call Closes the socket
        draw_border();                          // @call Draws a border
        printf(ANSI_COLOR_RED "Error: Maximum connections reached\n" ANSI_COLOR_RESET);  // @print Error message
        draw_border();                          // @call Draws a closing border
    }
    pthread_mutex_unlock(&conn_mutex);          // @call Unlocks the connection mutex

    return 0;                                   // @return Indicates success
}

// @function send_message
// @brief Sends a message to a specific connection
// @param id Connection ID to send the message to
// @param message The message to send
// @details Validates and sends the message, providing feedback on success or failure
void send_message(int id, char *message) {
    if (strlen(message) == 0) {                 // @if Checks if message is empty
        draw_border();                          // @call Draws a border
        printf(ANSI_COLOR_RED "Error: Message cannot be empty\n" ANSI_COLOR_RESET);  // @print Error message
        draw_border();                          // @call Draws a closing border
        return;                                 // @return Exits the function
    }

    if (strlen(message) > MAX_MESSAGE - 1) {    // @if Checks if message exceeds maximum length
        draw_border();                          // @call Draws a border
        printf(ANSI_COLOR_RED "Error: Message exceeds 100 characters\n" ANSI_COLOR_RESET);  // @print Error message
        draw_border();                          // @call Draws a closing border
        return;                                 // @return Exits the function
    }

    pthread_mutex_lock(&conn_mutex);            // @call Locks the connection mutex
    int valid_id = 0;                           // @var Flag to track valid connection ID
    for (int i = 0; i < connection_count; i++) {  // @loop Iterates through connections
        if (connections[i].id == id && connections[i].active) {  // @if Matches active connection
            valid_id = 1;                       // @set Marks ID as valid
            if (send(connections[i].socket_fd, message, strlen(message), 0) < 0) {  // @call Sends the message
                draw_border();                  // @call Draws a border
                printf(ANSI_COLOR_RED "Error: Failed to send message\n" ANSI_COLOR_RESET);  // @print Error message
                draw_border();                  // @call Draws a closing border
            } else {
                draw_border();                  // @call Draws a border
                printf(ANSI_COLOR_GREEN "Message sent to " ANSI_COLOR_MAGENTA "%s:%d\n" 
                       ANSI_COLOR_RESET, connections[i].ip, connections[i].remote_port);  // @print Success message
                draw_border();                  // @call Draws a closing border
            }
            break;                              // @break Exits the loop
        }
    }
    if (!valid_id) {                            // @if Checks for invalid ID
        draw_border();                          // @call Draws a border
        printf(ANSI_COLOR_RED "Error: Invalid connection ID\n" ANSI_COLOR_RESET);  // @print Error message
        draw_border();                          // @call Draws a closing border
    }
    pthread_mutex_unlock(&conn_mutex);          // @call Unlocks the connection mutex
}