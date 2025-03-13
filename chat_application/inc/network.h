#ifndef NETWORK_H
#define NETWORK_H

#define MAX_SERVERS 10         // @define Maximum number of servers allowed
#define TIMEOUT_SECONDS 5     // @define Timeout duration in seconds for unresponsive peers

// Declare global variables (without defining them here)
extern int server_socket;                // @var Global socket for the server
extern int listening_port;               // @var Global port on which the server is listening
extern int server_ports[MAX_SERVERS];    // @var Global array to store server ports
extern int server_count;                 // @var Global counter for the number of active servers
extern pthread_mutex_t server_mutex;     // @var Global mutex to synchronize access to the server ports array

// Function declarations
void *server_thread(void *arg);             // @function Main server thread
int connect_to_peer(char *dest_ip, int port); // @function Initiates a connection to a peer
void send_message(int id, char *message);   // @function Sends a message to a connection
int initiate_reverse_connection(char *dest_ip, int dest_port, int local_port); // @function Sets up a reverse connection
void notify_other_servers(char *client_ip, int client_port); // @function Notifies other servers of a new client

#endif