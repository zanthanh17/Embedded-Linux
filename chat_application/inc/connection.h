#ifndef CONNECTION_H
#define CONNECTION_H

#include <arpa/inet.h>

#define MAX_CONNECTIONS 10    // @define Maximum number of connections allowed
#define MAX_MESSAGE 100       // @define Maximum length of a message in characters
#define MAX_CLIENTS 100       // @define Maximum number of clients supported

// @struct Connection Structure to hold connection details
typedef struct {
    int id;                 // @var ID of the connection
    int socket_fd;          // @var File descriptor for the socket
    char ip[INET_ADDRSTRLEN]; // @var IP address of the peer
    int local_port;         // @var Local port on which the client is listening
    int remote_port;        // @var Remote port to which the client connects
    int active;             // @var Connection status (1: active, 0: inactive)
} Connection;

// @var Global array of connections
extern Connection connections[MAX_CONNECTIONS];
// @var Global counter for the number of active connections
extern int connection_count;
// @var Global mutex for synchronizing access to connections
extern pthread_mutex_t conn_mutex;

// @function Initializes the connection list
void init_connections(void);
// @function Terminates a specific connection by ID
void terminate_connection(int id);
// @function Lists all active connections
void list_connections(void);

#endif