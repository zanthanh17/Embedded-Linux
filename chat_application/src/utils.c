#include <stdio.h>
#include <string.h>      // Thêm header cho strcpy
#include <ifaddrs.h>
#include <arpa/inet.h>   // Thêm header cho inet_ntop
#include <netinet/in.h>  // Thêm header cho struct sockaddr_in
#include "../inc/utils.h"

void draw_border() {
    printf(ANSI_COLOR_CYAN "****************************\n" ANSI_COLOR_RESET);
}

void show_title() {
    draw_border();
    printf(ANSI_COLOR_YELLOW "       CHAT APPLICATION     \n" ANSI_COLOR_RESET);
    draw_border();
}

void get_my_ip(char *ip_buffer) {
    struct ifaddrs *ifaddr, *ifa;
    if (getifaddrs(&ifaddr) == -1) {
        strcpy(ip_buffer, "Unable to get IP"); // Sử dụng strcpy sau khi include string.h
        return;
    }
    
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET) {
            struct sockaddr_in *addr = (struct sockaddr_in *)ifa->ifa_addr; // Định nghĩa struct sockaddr_in
            inet_ntop(AF_INET, &addr->sin_addr, ip_buffer, INET_ADDRSTRLEN); // Sử dụng inet_ntop sau khi include arpa/inet.h
            break;
        }
    }
    freeifaddrs(ifaddr);
}