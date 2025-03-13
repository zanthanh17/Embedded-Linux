#ifndef NETWORK_H
#define NETWORK_H

#define MAX_SERVERS 10 // Số lượng server tối đa

// Chỉ khai báo các biến toàn cục bằng extern
extern int server_socket;
extern int listening_port;
extern int server_ports[MAX_SERVERS]; // Danh sách các port của server
extern int server_count; // Số lượng server đang chạy
extern pthread_mutex_t server_mutex; // Mutex để bảo vệ danh sách server

void *server_thread(void *arg);
int connect_to_peer(char *dest_ip, int port);
void send_message(int id, char *message);
int initiate_reverse_connection(char *dest_ip, int dest_port, int local_port);
void notify_other_servers(char *client_ip, int client_port);

#endif